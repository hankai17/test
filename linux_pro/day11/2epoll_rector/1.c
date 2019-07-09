#include <pthread.h> 
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <assert.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/epoll.h>

extern int sys_start;
extern config_t Config;
extern int multi_port_index;
extern int g_pid;
extern pthread_mutex_t  update_stream_info_ctc_mutex;
#define cfg_http_buf_size (8*1024) 
#define mid_http_buf_size (64*1024) 
#define MAX_EVENTS  1024

typedef struct _stream_info_t {
        int fileid;
        FILE* fd;
        int pub_len;
        int pri_len;
        http_stream_t* s;
        //other message
} stream_info;

typedef struct myevent_s {
    int fd;                 
    int events;             //EPOLLIN  EPLLOUT
    void *arg;              
    void (*call_back)(int fd, int read_event, int write_event, void *arg);
    int status;             //1 means polling; 0 means not polling
    long last_active;
    stream_info* si;
} myevent_s;

myevent_s g_events[MAX_EVENTS + 1];
static int g_efd;

int epoll_svc(void* arg);
static int create_socket(unsigned long lbe_ip, unsigned short lbe_port);
static int stream_flush_out(http_stream_t *s, int size);
static int stream_feed_out(http_stream_t *s);
static void stream_info_download_established(int sock, int read_event, int write_event, void *data);
static void download_set_event_in_out(myevent_s *ev);
static void close_stream(myevent_s* ev, int reason);
static int do_stream(stream_info* si, int size);

static myevent_s* evget();
static void eventset(struct myevent_s *ev, int fd, stream_info* si, void (*call_back)(int, int, int, void *), void *arg);
static void eventadd(int efd, int events, struct myevent_s *ev);
static void eventdel(int efd, struct myevent_s *ev);

static time_t g_last_modified_time;
static long next_run_time;

void init_get_stream_info_task() {
        pthread_t tid;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        if(multi_port_index == 0) {
                pthread_create(&tid, &attr, (int)epoll_svc, NULL);
        }
        return;
}
signal(SIGPIPE, SIG_IGN);
int epoll_svc(void* arg) {
        g_last_modified_time = 0;
        g_efd = epoll_create(MAX_EVENTS + 1);
        if(g_efd <= 0) {
                printf("create efd err %s\n", strerror(errno));
                return 0;
        }
        struct epoll_event events[MAX_EVENTS + 1];
        int checkpos = 0, i;
        next_run_time = time(NULL);

        while(1) {
                long now = time(NULL);
                create_socket(lbe_ip, ntohs(lbe_port)); //period task

                for (i = 0; i < 100; i++, checkpos++) {
                    if (checkpos == MAX_EVENTS)
                        checkpos = 0;
                    if (g_events[checkpos].status != 1)
                        continue;
                    long duration = now - g_events[checkpos].last_active;
                    if (duration >= Config.timeout.read_server) {
                        printf("stream_info_download fd: %d\n", g_events[checkpos].fd);
                        eventdel(g_efd, &g_events[checkpos]);
                        close_stream(&g_events[checkpos], HCT_INSIDE_TIMEOUT_1);
                    }
                }

                int nfd = epoll_wait(g_efd, events, MAX_EVENTS + 1, 1000);
                if (nfd < 0 && errno != EINTR) {
                    printf("epoll_wait err %s\n", strerror(errno));
                    break;
                }
                for (i = 0; i < nfd; i++) {
                    struct myevent_s *ev = (struct myevent_s *)events[i].data.ptr;
                    ev->call_back(ev->fd, events[i].events & EPOLLIN ? 1 : 0, events[i].events & EPOLLOUT ? 1 : 0, ev->arg);
                }
        }
        close(g_efd);
        return 0;
}

static myevent_s* evget() {
    int i = 0;
    myevent_s* ev = NULL;

    for(i = 0; i < MAX_EVENTS; i++) {
        if (g_events[i].status == 0) {
                ev = &g_events[i];
                break;
        }
    }
    if(i == MAX_EVENTS) {
        printf("max fd limit\n");
    }
    return ev;
}

static void eventset(struct myevent_s *ev, int fd, stream_info* si, void (*call_back)(int, int, int, void *), void *arg) {
    ev->fd = fd;
    ev->call_back = call_back;
    ev->events = 0;
    ev->arg = arg;
    ev->status = 0;
    ev->last_active = time(NULL);
    ev->si = si;
    return;
}

static void eventadd(int efd, int events, struct myevent_s *ev) {
    struct epoll_event epv = {0, {0}};
    int op;
    ev->last_active = time(NULL);
    epv.data.ptr = ev;
    epv.events = ev->events = events;

    if (ev->status == 1) {
        op = EPOLL_CTL_MOD;
    } else {
        op = EPOLL_CTL_ADD;
        ev->status = 1;
    }

    if (epoll_ctl(efd, op, ev->fd, &epv) < 0) {
        printf("eventadd failed fd: %d, events: %d, op: %d, err: %s\n", ev->fd, events, op,  xstrerror());
    } else {
        ;
        //printf("eventadd OK [fd=%d], op=%d, events[%0X]\n", ev->fd, op, events);
    }
    return;
}

static void eventdel(int efd, struct myevent_s *ev) {
    struct epoll_event epv = {0, {0}};

    if (ev->status != 1)
        return;

    epv.data.ptr = ev;
    ev->status = 0;
    if(epoll_ctl(efd, EPOLL_CTL_DEL, ev->fd, &epv) < 0) {
        printf("eventdell fd: %d, err: %s\n", ev->fd, xstrerror());
    }

    return;
}

static http_stream_t* stream_create(int sock, struct sockaddr_in* paddr) {
        printf("alloc s, & alloc s->buf\n");
        return s;
}

static void format_request(char* buf, int len, unsigned long lbe_ip, unsigned short lbe_port) {
        printf("format_req buf\n");
}

static int create_socket(unsigned long lbe_ip, unsigned short lbe_port) {
        int r = 0;
        int sock = 0;
        struct sockaddr_in peeraddr;
        int status;
        char stream_info_req[1024] = {0};

        format_request(stream_info_req, sizeof(stream_info_req) - 1, lbe_ip, lbe_port);

        memset(&peeraddr, 0, sizeof(peeraddr));
        peeraddr.sin_family = AF_INET;
        peeraddr.sin_addr.s_addr = lbe_ip;
        peeraddr.sin_port = htons(lbe_port);

        sock = socket(AF_INET,SOCK_STREAM,0);
        struct linger l;
        l.l_onoff = 1;
        l.l_linger = 0;
        setsockopt(sock, SOL_SOCKET, SO_LINGER, (char *)&l, sizeof(l));

        stream_info* si = (stream_info *)mem_alloc("stream_info", sizeof(stream_info)); //every stream has a si //ev-->si-->s
        memset(si, 0x0, sizeof(si));

        http_stream_t* s = stream_create(sock, &peeraddr);
        si->s = s;

        s->server_sock = sock;
        s->flags.server_closed = 0;

        struct myevent_s* ev = evget();
        eventset(ev, sock, si, stream_info_download_established, ev); //耦合有点高
        eventadd(g_efd, EPOLLOUT, ev);

        buf_put(s->in_buf, stream_info_req, strlen(stream_info_req));
        http_request_t* request = http_request_parse(s->in_buf, &status);
        if(comm_set_nonblock(sock) < 0) {
               printf("create_socket: comm_set_nonblock failed: %s\n", xstrerror());
               close_stream(ev, HCT_SOCKET_ERROR_2);
               return 0;
        }
        r = connect_nonb(sock, (struct sockaddr *)&peeraddr);
        if(r == -1) {
               close_stream(ev,  HCT_SOCKET_ERROR_3);
               printf("create_socket: connect failed: %s\n", xstrerror());
               return 0;
        }
        printf("create_socket --->\n");
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

static int do_stream(stream_info* si, int size) {
        printf("do working...");
}

static void stream_info_download_established(int sock, int read_event, int write_event, void *data) {
        myevent_s * ev = (myevent_s*)data;
        stream_info* si = ev->si;
        http_stream_t* s = si->s;
        assert(sock == s->server_sock);
        if(read_event == 0 && write_event == 0) {
                printf("stream_info_download_established: read & write event are 0\n");
                close_stream(ev, HCT_SERVER_CLOSE_2);
                return;
        }
        int size = 0;

        if(write_event) { //send req
                if(stream_flush_out(s, -1) < 0) {
                        close_stream(ev, HCT_SERVER_CLOSE_4);
                        printf("stream_info_download_established: lbe close when send req\n");
                        return;
                }
        }

        if(read_event) { //recv resp
                assert(buf_data_size(s->out_buf) == 0);
                if((stream_feed_out(s)) < 0) { //peer closed or other err
                        if(buf_data_size(s->out_buf) == 0) {
                                close_stream(ev, HCT_SERVER_CLOSE_3);
                                printf("stream_info_download_established: lbe closed and buf data size is 0\n");
                                return;
                        }
                        /*
                        if(buf_data_size(s->out_buf) > 0) {
                                printf("stream_info_download_established: lbe closed and exists buf data\n");
                        }
                        */
                }
                if(s->out_buf->data_len < s->out_buf->buf_len) {
                        char* enddata = s->out_buf->buffer + s->out_buf->data_len;
                        *enddata = 0;
                }
                if(s->reply_body_size_left > 0) {
                        size = buf_data_size(s->out_buf);
                        //IO(s->out_buf->buffer, http_reply_body_size(s->request->method, s->reply) - s->reply_body_size_left, size, 1, si->fd);
                        s->reply_body_size_left -= size;
                        clear_space(s->out_buf);
                        if(s->reply_body_size_left == 0 || s->flags.server_closed) { //recv done. update stream_info.xml & update g_last_modified_time & active close
                                do_stream(si, http_reply_body_size(s->request->method, s->reply));
                                g_last_modified_time = s->reply->last_modified;
                                close_stream(ev, HCT_OK_1);
                        }

                } else if(s->reply_body_size_left == 0 && !s->reply) {
                        reply = http_reply_parse(s->out_buf, 0);
                        /* how to do in this case ? */
                        if(reply == NULL || reply->content_length == 0) {
                                NEED_DEUBG_MODULE(DEBUG_LV_1,DEBUG_FREE)print_log("stream_info_download_established: http_reply_parse return nul\n");
                                close_stream(ev, HCT_REPLY_PARSE_ERROR);
                                return;
                        }
                        if(reply->sline.status != 200) {
                                NEED_DEUBG_MODULE(DEBUG_LV_1, DEBUG_FREE)print_log("stream_info_download_established: http code: %d, end stream\n", reply->sline.status);
                                close_stream(ev, HCT_REPLY_PARSE_ERROR);
                                return;
                        }

                        if(http_reply_body_size(s->request->method, reply) != (pub_len == -1 ? 0 : pub_len) + (pri_len == -1 ? 0 : pri_len) ) {
                                NEED_DEUBG_MODULE(DEBUG_LV_1,DEBUG_FREE)print_log("stream_info_download_established: http_reply_parse cl: %d != comm: %d + pri: %d\n", http_reply_body_size(s->request->method, reply), pub_len, pri_len);
                                close_stream(ev, HCT_REPLY_PARSE_ERROR);
                                return;
                        }

                        s->reply = reply;

                        if(reply->last_modified == 0) {
                                NEED_DEUBG_MODULE(DEBUG_LV_1, DEBUG_FREE)print_log("stream_info_download_established: http code: %d, not found last modify, end stream\n", reply->sline.status);
                                close_stream(ev, HCT_REPLY_PARSE_ERROR);
                                return;
                        } 

                        if(si->fd == NULL) {
                        }

                        s->reply_body_size_left = http_reply_body_size(s->request->method, s->reply); //content-length
                        if(s->reply_body_size_left > 0) {
                                s->reply_body_size_left -= (buf_data_size(s->out_buf) - s->reply->hdr_sz);
                                IO(s->out_buf->buffer + s->reply->hdr_sz, 0, buf_data_size(s->out_buf) - s->reply->hdr_sz, 1, si->fd);
                                clear_space(s->out_buf);
                                if(s->reply_body_size_left == 0) {
                                        do_stream(si, http_reply_body_size(s->request->method, s->reply));
                                        g_last_modified_time = s->reply->last_modified;
                                        //printf("s->reply->last_modified: %ld\n", (long)s->reply->last_modified);
                                        close_stream(ev, HCT_OK_1);
                                        return;
                                }
                        }
                        /*
                        if(s->flags.server_closed == 1) {
                                NEED_DEUBG_MODULE(DEBUG_LV_1, DEBUG_FREE)print_log("stream_info_download_established: server closed\n");
                                close_stream(ev, 0);
                                return;
                        }
                        */
                }
        }
        download_set_event_in_out(ev);
}

static void download_set_event_in_out(myevent_s *ev) {
        stream_info* si = ev->si;
        http_stream_t* s = si->s;
        int rw_type = 0;
        if(s->server_sock < 0)
                return;
        if(buf_free_space(s->out_buf) > 0) {
                rw_type |= EPOLLIN;
        }
        if(buf_data_size(s->in_buf) > 0) {
                rw_type |= EPOLLOUT;
        }
        //eventdel(g_efd, ev);
        //eventset(ev, ev->fd, ev->si, stream_info_download_established, ev);
        eventadd(g_efd, rw_type, ev);
        return;
}

static void close_stream(myevent_s* ev, int reason) {
        printf("stream_info_download close_stream, %d\n", reason);
        if(ev != NULL) {
                stream_info* si = ev->si;
                if(si != NULL) {
                        if(si->fd != NULL) {
                                //release stream_info mates //业务层的东西
                        }

                        http_stream_t* s = si->s; //tcp层的东西 就是fd 就是buf http协议 等
                        if(s->server_sock >= 0) {
                                eventdel(g_efd, ev);
                                close(ev->fd);
                                s->server_sock = -1;
                        }
                        //free s's request reply bufs
                        si = NULL;
                }
        }
        return;
}
