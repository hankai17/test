#include <unistd.h>
#include <sys/socket.h>
#include <assert.h>
#include <sys/stat.h>
#include <signal.h>

#include "fd.h"
#include "config.h"
#include "comm.h"
#include "util.h"
#include "mem.h"
#include "cache/files.h"
#include "access_log.h"
#include "http_request.h"
#include "http_reply.h"
#include "cache/tools.h"
#include "cache/siteinfo.h"
#include "siteinfo_get.h"
#include "http_stream.h"
#include "lib/rfc1123.h"

extern int sys_start;
extern config_t Config;
extern int multi_port_index;
extern int g_pid;
#define cfg_http_buf_size (8*1024) 
#define mid_http_buf_size (64*1024) 

typedef struct _siteinfo_t {
        int fileid;
        FILE* fd;
        int pub_len;
        int pri_len;
        http_stream_t* s;
} siteinfo;

LPTHREAD_ROUTINE get_siteinfo_svc(void* arg);
static http_stream_t* stream_create(int sock, struct sockaddr_in* paddr);
static int create_socket(unsigned long lbe_ip, unsigned short lbe_port);
static int stream_flush_out(http_stream_t *s, int size);
static int stream_feed_out(http_stream_t *s);
static void siteinfo_download_established(int sock, int read_event, int write_event, void *data);
static void download_set_event_in_out(siteinfo *s);
static void close_stream(siteinfo *si, int reason);
static int do_siteinfo_update(siteinfo* si, int size);
//static int do_siteinfo_update1(int size);

static time_t g_last_modified_time;

void init_get_siteinfo_task() {
        pthread_t tid;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        if(multi_port_index == 0) {
                pthread_create(&tid, &attr, (LPTHREAD_ROUTINE)get_siteinfo_svc, NULL);
        }
        return;
}

LPTHREAD_ROUTINE get_siteinfo_svc(void* arg) {
        //g_last_modified_time = time(NULL);
        g_last_modified_time = 0;
        while(1) {
                if(sys_start && g_configure.cfg_get_siteinfo_from_lbe) {
                        if(g_configure.cfg_lbe_table[0].lbe_addr != 0 && g_configure.cfg_lbe_table[0].lbe_siteinfo_get_port != 0) {
                                unsigned long lbe_ip = g_configure.cfg_lbe_table[0].lbe_addr;
                                unsigned short lbe_port = g_configure.cfg_lbe_table[0].lbe_siteinfo_get_port;
                                create_socket(lbe_ip, ntohs(lbe_port));
                        }
                }
                usleep(g_configure.cfg_get_siteinfo_from_lbe_interval * 1000 * 1000); //default 5 * 60s
        }
}

static http_stream_t* stream_create(int sock, struct sockaddr_in* paddr) {
        http_stream_t* s;
        s = mem_alloc("stream", sizeof(http_stream_t));
        memset(s, 0, sizeof(http_stream_t));
        s->data_start = time(NULL);
        s->data_in = 0;
        s->data_out = 0;

        struct timeval now;
        gettimeofday(&now, NULL); 
        s->data_begin = ((long)now.tv_sec) * 1000 + (long)now.tv_usec / 1000; 

        s->server_sock = sock;
        memcpy(&s->client_addr, paddr, sizeof(struct sockaddr_in));

        s->in_buf = mem_alloc("http_stream_in_buf",sizeof(buf_t));
        s->in_buf->buffer = mem_alloc("http_stream_in_buf_data", cfg_http_buf_size);
        s->in_buf->buffer[0] = 0;
        s->in_buf->buf_len = cfg_http_buf_size;
        s->in_buf->data_len = 0;

        s->out_buf = mem_alloc("http_stream_out_buf",sizeof(buf_t));
        s->out_buf->buffer = mem_alloc("http_stream_out_buf_data_4", mid_http_buf_size);
        s->out_buf->buf_len = mid_http_buf_size;
        s->out_buf->data_len = 0;

        return s;
}

static void siteinfo_download_timeout(int fd, void* data) {
        siteinfo* s = (siteinfo*)data;
        assert(fd == s->s->server_sock);
        NEED_DEUBG_MODULE(DEBUG_LV_1,DEBUG_FREE)print_log("siteinfo_download_timeout");
        close_stream(s, HCT_INSIDE_TIMEOUT_1);
}

static void format_request(char* buf, int len, unsigned long lbe_ip, unsigned short lbe_port) {
        char str_ip[64] = {0};
        struct in_addr addr;
        addr.s_addr = lbe_ip;
        inet_ntop(AF_INET, &addr, str_ip, sizeof(str_ip));

        snprintf(buf, len, "GET /siteinfo.xml HTTP/1.1\r\nHost: %s:%hu\r\nUser-Agent: PPC\r\nIf-Modified-Since: %s\r\nConnection: close\r\n\r\n",
                                                                        str_ip, 
                                                                        lbe_port, 
                                                                        mkrfc1123(g_last_modified_time)
                                                                        );
}

static int create_socket(unsigned long lbe_ip, unsigned short lbe_port) {
        int r = 0;
        int sock = 0;
        struct sockaddr_in peeraddr;
        int status;
        char siteinfo_req[1024] = {0};

        format_request(siteinfo_req, sizeof(siteinfo_req) - 1, lbe_ip, lbe_port);
        //printf("siteinfo_req: %s\n", siteinfo_req);

        memset(&peeraddr, 0, sizeof(peeraddr));
        peeraddr.sin_family = AF_INET;
        peeraddr.sin_addr.s_addr = lbe_ip;
        peeraddr.sin_port = htons(lbe_port);

        sock = socket(AF_INET,SOCK_STREAM,0);
        if(sock < 0) {
                NEED_DEUBG_MODULE(DEBUG_LV_1,DEBUG_FREE)print_log("create_socket: socket failed: %s\n", xstrerror());
                return 0;
        }
        struct linger l;
        l.l_onoff = 1;
        l.l_linger = 0;
        setsockopt(sock, SOL_SOCKET, SO_LINGER, (char *)&l, sizeof(l));

        siteinfo* si = (siteinfo *)mem_alloc("siteinfo", sizeof(siteinfo)); //every stream has a si
        memset(si, 0x0, sizeof(si));
        si->pub_len = -1;
        si->pri_len = -1;

        http_stream_t* s = stream_create(sock, &peeraddr);
        si->s = s;
        s->server_sock = sock;
        fd_open(sock, "HTTP Requesto");
        s->flags.server_closed = 0;
        buf_put(s->in_buf, siteinfo_req, strlen(siteinfo_req));

        http_request_t* request = http_request_parse(s->in_buf, &status);
        if(request != NULL) {
                s->request = request;
                s->request->method = METHOD_GET;
        } else {
                NEED_DEUBG_MODULE(DEBUG_LV_1,DEBUG_FREE)print_log("create_socket: http_request_parse failed: %s\n", xstrerror());
                close_stream(si, HCT_SOCKET_ERROR_2);
                return 0;
        }

        if(comm_set_nonblock(sock) < 0) {
               NEED_DEUBG_MODULE(DEBUG_LV_1,DEBUG_FREE)print_log("create_socket: comm_set_nonblock failed: %s\n", xstrerror());
               close_stream(si, HCT_SOCKET_ERROR_2);
               return 0;
        }
        r = connect_nonb(sock, (struct sockaddr *)&peeraddr);
        if(r == -1) {
                close_stream(si,  HCT_SOCKET_ERROR_3);
                NEED_DEUBG_MODULE(DEBUG_LV_1,DEBUG_FREE)print_log("create_socket: connect failed: %s\n", xstrerror());
                return 0;
        }

        comm_set_select(sock, COMM_SELECT_WRITE, siteinfo_download_established, si);
        comm_set_timeout(s->server_sock, Config.timeout.read_server, siteinfo_download_timeout, (void *)si);
        return 0;
}

static int stream_flush_out(http_stream_t *s, int size) {
        int r;
        int t = 0;
        int cur_size = size;

         if(s->server_sock < 0) return -1;
         if(buf_data_size(s->in_buf) < size || size <= 0 )
                cur_size = buf_data_size(s->in_buf);

        while(cur_size > 0) {
                r = write(s->server_sock, s->in_buf->buffer, cur_size);
                if(r < 0) {
                        if(errno == EINTR) {
                                continue;
                        }
                        if(errno == EWOULDBLOCK || errno == EAGAIN) {
                                return t;
                        } else return -1;
                } else if ( r == 0 ) {
                        return t;
                } else {
                        buf_pick(s->in_buf, r);
                        cur_size -= r;
                        t += r;
                }
        }
        return t;
}

static int stream_feed_out(http_stream_t *s) {
        int r;
        int t = 0;
        if(s->server_sock < 0 ) return -1;
        while(buf_free_space(s->out_buf) > 0) {
                r = read(s->server_sock, buf_end_position(s->out_buf), buf_free_space(s->out_buf));
                if(r < 0) {
                        if(errno == EINTR ) continue;
                        if(errno == EWOULDBLOCK || errno == EAGAIN) {
                                return t;
                        } else {
                                return -1;
                        }
                } else if( r == 0) { //peer active closed. Its possiable that there were remain datas in buf !
                        s->flags.server_closed = 1;
                        return -1;
                } else {
                        s->out_buf->data_len += r;
                        t += r;
                        //return t;
                        return t;
                }
        }
        s->data_in += t;
        return t;
}

static int do_siteinfo_update(siteinfo* si, int size) {
        char sp[PPCCFG_MAX_PATH_LEN] = {0};
        char new_sp[PPCCFG_MAX_PATH_LEN] = {0};
        char pri_sp[PPCCFG_MAX_PATH_LEN] = {0};
        char tmp_sp[PPCCFG_MAX_PATH_LEN] = {0};

        char backup_pub_dir[PPCCFG_MAX_PATH_LEN] = {0};
        char backup_pri_dir[PPCCFG_MAX_PATH_LEN] = {0};
        char buf[16] = {0};

        if(IsDir(cfg_work_root) ) {
                NEED_DEUBG_MODULE(DEBUG_LV_1,DEBUG_FREE)print_log("do_siteinfo_update: cfg_work_root is not a dir\n");
                return -1;
        }
        snprintf(sp, PPCCFG_MAX_PATH_LEN, "%s/etc/http/siteinfo.xml", cfg_work_root);
        snprintf(new_sp, PPCCFG_MAX_PATH_LEN, "%s/etc/http/siteinfo.xml.tmp", cfg_work_root);
        snprintf(pri_sp, PPCCFG_MAX_PATH_LEN, "%s/etc/pri/http/siteinfo.xml", cfg_work_root);
        snprintf(backup_pub_dir, PPCCFG_MAX_PATH_LEN, "%s/etc/backup/comm/http", cfg_work_root);
        snprintf(backup_pri_dir, PPCCFG_MAX_PATH_LEN, "%s/etc/backup/pri/http", cfg_work_root);
        if(IsDir(backup_pub_dir) || IsDir(backup_pri_dir)) {
                NEED_DEUBG_MODULE(DEBUG_LV_1,DEBUG_FREE)print_log("do_siteinfo_update: backup_pub/pri_dir is not a dir\n");
                if(makeDir(backup_pub_dir) < 0 || makeDir(backup_pri_dir) < 0) {
                        NEED_DEUBG_MODULE(DEBUG_LV_1,DEBUG_FREE)print_log("do_siteinfo_update: can not mkdir backup_pub/pri_dir\n");
                        return -1;
                }
        }

        time_t raw_time;
        time(&raw_time);
        struct tm* info = localtime(&raw_time);
        strftime(buf, sizeof(buf), "%Y%m%d%H%M%S", info);

        if(si->pub_len > 0) {
                snprintf(tmp_sp, PPCCFG_MAX_PATH_LEN, "%s/etc/backup/comm/http/siteinfo.xml.%s", cfg_work_root, buf);
                rename(sp, tmp_sp); //backup old pub conf
                move_file(new_sp, sp, 0, si->pub_len); //update new pub conf
        }

        memset(tmp_sp, 0x0, PPCCFG_MAX_PATH_LEN);

        if(si->pri_len > 0) {
                snprintf(tmp_sp, PPCCFG_MAX_PATH_LEN, "%s/etc/backup/pri/http/siteinfo.xml.%s", cfg_work_root, buf);
                rename(pri_sp, tmp_sp); //backup old pri conf
                move_file(new_sp, pri_sp, 0 + si->pub_len, si->pri_len); //update new pub conf
        }
        //unlink(new_sp);

        //update
        if(site_Init() < 0) {
                NEED_DEUBG_MODULE(DEBUG_LV_1, DEBUG_FREE)print_log("siteinfo_download update siteinfo.xml err\n");
                return -1;
        }
        //update other
        int pid_array[32] = {0};
        int pid_array_num = 0;
        int j = 0;
        getPidByName("http_ppc", pid_array, &pid_array_num);
        if(pid_array_num > 1) {
                for(j = 0; j < pid_array_num; j++) {
                        if(pid_array[j] != g_pid) {
                                if( 0 != kill(pid_array[j], DEBUG_REREAD_SITEINFO_CONFIG_SIGN)) {
                                        NEED_DEUBG_MODULE(DEBUG_LV_1, DEBUG_FREE)print_log("siteinfo_download update siteinfo.xml: %d send signal err\n", pid_array[j]);
                                } else {
                                        //NEED_DEUBG_MODULE(DEBUG_LV_1, DEBUG_FREE)print_log("siteinfo_download update siteinfo.xml send signal ok\n");
                                }
                        }
                }
        }

        NEED_DEUBG_MODULE(DEBUG_LV_1, DEBUG_FREE)print_log("siteinfo_download update siteinfo.xml ok\n");
        return 0;
}

/*
static int do_siteinfo_update1(int size) {
        char sp[PPCCFG_MAX_PATH_LEN] = {0};
        char new_sp[PPCCFG_MAX_PATH_LEN] = {0};
        char pri_sp[PPCCFG_MAX_PATH_LEN] = {0};
        char tmp_sp[PPCCFG_MAX_PATH_LEN] = {0};
        char buf[16] = {0};

        if(IsDir(cfg_work_root) ) {
                NEED_DEUBG_MODULE(DEBUG_LV_1,DEBUG_FREE)print_log("do_siteinfo_update: cfg_work_root is not a dir\n");
                return -1;
        }
        snprintf(sp, PPCCFG_MAX_PATH_LEN, "%s/etc/http/siteinfo.xml", cfg_work_root);
        snprintf(new_sp, PPCCFG_MAX_PATH_LEN, "%s/etc/http/siteinfo.xml.tmp", cfg_work_root);
        snprintf(pri_sp, PPCCFG_MAX_PATH_LEN, "%s/etc/pri/http/siteinfo.xml", cfg_work_root);

        time_t raw_time;
        time(&raw_time);
        struct tm* info = localtime(&raw_time);
        strftime(buf, sizeof(buf), "%Y%m%d%H%M%S", info);


        if(g_configure.cfg_use_merge == 1) {
                //backup old pri siteinfo.xml
                snprintf(tmp_sp, PPCCFG_MAX_PATH_LEN, "%s/etc/pri/http/siteinfo.xml.%s", cfg_work_root, buf);
                rename(pri_sp, tmp_sp);
                //cp into pri
                if(move_file(new_sp, pri_sp, 0, size) == -1) {
                        NEED_DEUBG_MODULE(DEBUG_LV_1,DEBUG_FREE)print_log("do_siteinfo_update: move file err\n");
                        return -1;
                }
        } else {
                snprintf(tmp_sp, PPCCFG_MAX_PATH_LEN, "%s/etc/http/siteinfo.xml.%s", cfg_work_root, buf);
                //backup old siteinfo.xml
                rename(sp, tmp_sp);

                //mv siteinfo.xml.tmp to siteinfo.xml
                rename(new_sp, sp);
        }

        //update
        if(site_Init() < 0) {
                NEED_DEUBG_MODULE(DEBUG_LV_1, DEBUG_FREE)print_log("siteinfo_download update siteinfo.xml err\n");
                return -1;
        }
        NEED_DEUBG_MODULE(DEBUG_LV_1, DEBUG_FREE)print_log("siteinfo_download update siteinfo.xml ok\n");
        return 0;
}
*/

static void siteinfo_download_established(int sock, int read_event, int write_event, void *data) {
        siteinfo* si = (siteinfo*)data;
        http_stream_t* s = si->s;
        http_reply_t* reply;
        assert(sock == s->server_sock);
        if(read_event == 0 && write_event == 0) {
                NEED_DEUBG_MODULE(DEBUG_LV_1,DEBUG_FREE)print_log("siteinfo_download_established: read & write event are 0\n");
                close_stream(si, HCT_SERVER_CLOSE_2);
                return;
        }
        comm_set_timeout(s->server_sock, -1, NULL, NULL);
        int size = 0;

        if(write_event) { //send req
                if(stream_flush_out(s, -1) < 0) {
                        close_stream(si, HCT_SERVER_CLOSE_4);
                        NEED_DEUBG_MODULE(DEBUG_LV_1,DEBUG_FREE)print_log("siteinfo_download_established: lbe close when send req\n");
                        return;
                }
        }

        if(read_event) { //recv resp
                assert(buf_data_size(s->out_buf) == 0);
                if((stream_feed_out(s)) < 0) { //peer closed or other err
                        if(buf_data_size(s->out_buf) == 0) {
                                close_stream(si, HCT_SERVER_CLOSE_3);
                                NEED_DEUBG_MODULE(DEBUG_LV_1,DEBUG_FREE)print_log("siteinfo_download_established: lbe closed and buf data size is 0\n");
                                return;
                        }
                        /*
                        if(buf_data_size(s->out_buf) > 0) {
                                NEED_DEUBG_MODULE(DEBUG_LV_1,DEBUG_FREE)print_log("siteinfo_download_established: lbe closed and exists buf data\n");
                        }
                        */
                }
                if(s->out_buf->data_len < s->out_buf->buf_len) {
                        char* enddata = s->out_buf->buffer + s->out_buf->data_len;
                        *enddata = 0;
                }
                if(s->reply_body_size_left > 0) {
                        size = buf_data_size(s->out_buf);
                        IO(s->out_buf->buffer, http_reply_body_size(s->request->method, s->reply) - s->reply_body_size_left, size, 1, si->fd);
                        s->reply_body_size_left -= size;
                        clear_space(s->out_buf);
                        if(s->reply_body_size_left == 0 || s->flags.server_closed) { //recv done. update siteinfo.xml & update g_last_modified_time & active close
                                do_siteinfo_update(si, http_reply_body_size(s->request->method, s->reply));
                                g_last_modified_time = s->reply->last_modified;
                                //printf("s->reply->last_modified: %ld\n", (long)s->reply->last_modified);
                                close_stream(si, HCT_OK_1);
                        }

                } else if(s->reply_body_size_left == 0 && !s->reply) {
                        reply = http_reply_parse(s->out_buf, 0);
                        /* how to do in this case ? */
                        if(reply == NULL || reply->content_length == 0) {
                                NEED_DEUBG_MODULE(DEBUG_LV_1,DEBUG_FREE)print_log("siteinfo_download_established: http_reply_parse return nul\n");
                                close_stream(si, HCT_REPLY_PARSE_ERROR);
                                return;
                        }
                        if(reply->sline.status != 200) {
                                NEED_DEUBG_MODULE(DEBUG_LV_1, DEBUG_FREE)print_log("siteinfo_download_established: http code: %d, end stream\n", reply->sline.status);
                                close_stream(si, HCT_REPLY_PARSE_ERROR);
                                return;
                        }

                        int pub_len = http_header_get_size(&reply->header, HDR_CL_COMM);
                        int pri_len = http_header_get_size(&reply->header, HDR_CL_PRI);
                        if(pub_len == -1 && pri_len == -1) {
                                NEED_DEUBG_MODULE(DEBUG_LV_1,DEBUG_FREE)print_log("siteinfo_download_established: http_reply_parse return nul, no hdr_cl_comm/pri\n");
                                close_stream(si, HCT_REPLY_PARSE_ERROR);
                                return;
                        }
                        if(http_reply_body_size(s->request->method, reply) != (pub_len == -1 ? 0 : pub_len) + (pri_len == -1 ? 0 : pri_len) ) {
                                NEED_DEUBG_MODULE(DEBUG_LV_1,DEBUG_FREE)print_log("siteinfo_download_established: http_reply_parse cl: %d != comm: %d + pri: %d\n", http_reply_body_size(s->request->method, reply), pub_len, pri_len);
                                close_stream(si, HCT_REPLY_PARSE_ERROR);
                                return;
                        }

                        si->pub_len = pub_len;
                        si->pri_len = pri_len;

                        s->reply = reply;


                        if(reply->last_modified == 0) {
                                NEED_DEUBG_MODULE(DEBUG_LV_1, DEBUG_FREE)print_log("siteinfo_download_established: http code: %d, not found last modify, end stream\n", reply->sline.status);
                                close_stream(si, HCT_REPLY_PARSE_ERROR);
                                return;
                        } 

                        NEED_DEUBG_MODULE(DEBUG_LV_1, DEBUG_FREE)print_log("siteinfo_download_established: http code: %d, begin stream--->\n", reply->sline.status);
                        if(si->fd == NULL) {
                                char file[PPCCFG_MAX_PATH_LEN] = {0};
                                if(IsDir(cfg_work_root) ) {
                                        NEED_DEUBG_MODULE(DEBUG_LV_1,DEBUG_FREE)print_log("siteinfo_download_established: cfg_work_root is not a dir\n");
                                        close_stream(si, HCT_REPLY_PARSE_ERROR);
                                        return;
                                }
                                snprintf(file, PPCCFG_MAX_PATH_LEN, "%s/etc/http/siteinfo.xml.tmp", cfg_work_root);

                                unlink(file);
                                si->fd = fopen(file, "w+");
                                if(si->fd == NULL) {
                                        NEED_DEUBG_MODULE(DEBUG_LV_1,DEBUG_FREE)print_log("siteinfo_download_established: fopen err %s\n", strerror(errno));
                                        close_stream(si, HCT_REPLY_PARSE_ERROR);
                                        return;
                                }
                        }

                        s->reply_body_size_left = http_reply_body_size(s->request->method, s->reply); //content-length
                        if(s->reply_body_size_left > 0) {
                                s->reply_body_size_left -= (buf_data_size(s->out_buf) - s->reply->hdr_sz);
                                IO(s->out_buf->buffer + s->reply->hdr_sz, 0, buf_data_size(s->out_buf) - s->reply->hdr_sz, 1, si->fd);
                                clear_space(s->out_buf);
                                if(s->reply_body_size_left == 0) {
                                        do_siteinfo_update(si, http_reply_body_size(s->request->method, s->reply));
                                        g_last_modified_time = s->reply->last_modified;
                                        //printf("s->reply->last_modified: %ld\n", (long)s->reply->last_modified);
                                        close_stream(si, HCT_OK_1);
                                        return;
                                }
                        }
                        /*
                        if(s->flags.server_closed == 1) {
                                NEED_DEUBG_MODULE(DEBUG_LV_1, DEBUG_FREE)print_log("siteinfo_download_established: server closed\n");
                                close_stream(si, 0);
                                return;
                        }
                        */
                }
        }
        download_set_event_in_out(si);
}

static void download_set_event_in_out(siteinfo *si) {
        http_stream_t* s = si->s;
        int rw_type = 0;
        if(s->server_sock < 0)
                return;
        if(buf_free_space(s->out_buf) > 0) {
                rw_type |= COMM_SELECT_READ;
        }
        if(buf_data_size(s->in_buf) > 0) {
                rw_type |= COMM_SELECT_WRITE;
        }

        comm_set_select(s->server_sock, rw_type, siteinfo_download_established, (void *)si);
        comm_set_timeout(s->server_sock, Config.timeout.read_server, siteinfo_download_timeout, (void *)si);
        return;
}

static void close_stream(siteinfo *si, int reason) {
        NEED_DEUBG_MODULE(DEBUG_LV_1, DEBUG_FREE)print_log("siteinfo_download close_stream\n");
        if(si != NULL) {
                if(si->fd != NULL) {
                        if(si->s != NULL) {
                                /*
                                long a = ftell(si->fd); //check file integrity
                                if(a == http_reply_body_size(si->s->request->method, si->s->reply)) {
                                        NEED_DEUBG_MODULE(DEBUG_LV_1, DEBUG_FREE)print_log("siteinfo_download: (real file size)%d == (content-length)%d\n", a, http_reply_body_size(si->s->request->method, si->s->reply) );
                                        fclose(si->fd);
                                        si->fd = NULL;
                                } else {
                                        NEED_DEUBG_MODULE(DEBUG_LV_1, DEBUG_FREE)print_log("siteinfo_download: (real file size)%d != (content-length)%d\n", a, http_reply_body_size(si->s->request->method, si->s->reply) );
                                        fclose(si->fd);
                                        si->fd = NULL;
                                }
                                */
                                fclose(si->fd);
                                si->fd = NULL;
                        }
                }

                http_stream_t* s = si->s;
                if(s->server_sock >= 0) {
                        comm_clear_select(s->server_sock);
                        comm_set_timeout(s->server_sock, -1, NULL, NULL);
                        fd_close(s->server_sock);
                        s->server_sock = -1;
                }

                if(s->request) {
                        http_request_destroy(s->request);
                        s->request = 0;
                }
                if(s->reply){
                        http_reply_destroy(s->reply);
                        s->reply = 0;
                }

                memset(&s->flags, 0, sizeof(int));
                string_clean(&s->oldhost);
                mem_free(s->in_buf->buffer);
                mem_free(s->in_buf);
                mem_free(s->out_buf->buffer);
                mem_free(s->out_buf);
                mem_free(s);

                mem_free(si);
                si = NULL;
        }
        return;
}
