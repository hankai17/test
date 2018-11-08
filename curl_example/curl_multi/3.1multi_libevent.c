/* Example application source code using the multi socket interface to
   download many files at once.

This is an adaptation of libcurl's "hipev.c" and libevent's "event-test.c"
sample programs.

When running, the program creates the named pipe "hiper.fifo"

Whenever there is input into the fifo, the program reads the input as a list
of URL's and creates some new easy handles to fetch each URL via the
curl_multi "hiper" API.


Thus, you can try a single URL:
  % echo http://www.yahoo.com > hiper.fifo

Or a whole bunch of them:
  % cat my-url-list > hiper.fifo

The fifo buffer is handled almost instantly, so you can even add more URL's
while the previous requests are still being downloaded.

Note:
  For the sake of simplicity, URL length is limited to 1023 char's !

This is purely a demo app, all retrieved data is simply discarded by the write
callback.

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <sys/poll.h>
#include <curl/curl.h>
#include <event2/event.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

typedef char bool;
#define TRUE 1
#define MSG_OUT stdout /* Send info to stdout, change to stderr if you want */

/* Global information, common to all connections */
typedef struct _GlobalInfo {
  struct event_base* evbase;
  struct event* fifo_event;
  struct event* timer_event;
  CURLM* multi;
  int still_running;
  FILE* input;
} GlobalInfo;


/* Information associated with a specific easy handle */
typedef struct _ConnInfo {
  CURL* easy;
  char* url;
  GlobalInfo* global;
  char error[CURL_ERROR_SIZE];
} ConnInfo;


/* Information associated with a specific socket */
typedef struct _SockInfo
{
  curl_socket_t sockfd;
  CURL *easy;
  int action;
  long timeout;
  struct event *ev;
  int evset;
  GlobalInfo *global;
} SockInfo;

/* Update the event timer after curl_multi library calls */
static int multi_timer_cb(CURLM *multi, long timeout_ms, GlobalInfo *g)
{
  struct timeval timeout;
  (void)multi; /* unused */

  timeout.tv_sec = timeout_ms/1000;
  timeout.tv_usec = (timeout_ms%1000)*1000;
  fprintf(MSG_OUT, "multi_timer_cb: Setting timeout to %ld ms\n", timeout_ms);

  /* TODO
   * if timeout_ms is 0, call curl_multi_socket_action() at once!
   * if timeout_ms is -1, just delete the timer
   * for all other values of timeout_ms, this should set or *update*
   * the timer to the new value
   */
  evtimer_add(g->timer_event, &timeout);
  return 0;
}

/* Die if we get a bad CURLMcode somewhere */
static void mcode_or_die(const char* where, CURLMcode code) {
  if(CURLM_OK != code) { //code一直为0 so不会打印下面的异常处理
    const char *s;
    switch(code) {
      case     CURLM_BAD_HANDLE:         s = "CURLM_BAD_HANDLE";         break;
      case     CURLM_BAD_EASY_HANDLE:    s = "CURLM_BAD_EASY_HANDLE";    break;
      case     CURLM_OUT_OF_MEMORY:      s = "CURLM_OUT_OF_MEMORY";      break;
      case     CURLM_INTERNAL_ERROR:     s = "CURLM_INTERNAL_ERROR";     break;
      case     CURLM_UNKNOWN_OPTION:     s = "CURLM_UNKNOWN_OPTION";     break;
      case     CURLM_LAST:               s = "CURLM_LAST";               break;
      default: s = "CURLM_unknown";
        break;
    case     CURLM_BAD_SOCKET:         s = "CURLM_BAD_SOCKET";
      fprintf(MSG_OUT, "ERROR: %s returns %s\n", where, s);
      /* ignore this error */
      return;
    }
    fprintf(MSG_OUT, "ERROR: %s returns %s\n", where, s);
    exit(code);
  }
}



/* Check for completed transfers, and remove their easy handles */
static void check_multi_info(GlobalInfo* g) {
  char* eff_url;
  CURLMsg* msg;
  int msgs_left;
  ConnInfo* conn;
  CURL* easy;
  CURLcode res;

  fprintf(MSG_OUT, "REMAINING...: %d\n", g->still_running);
  while((msg = curl_multi_info_read(g->multi, &msgs_left))) { //如果fd一直不变化是不会到while里面的
    if(msg->msg == CURLMSG_DONE) {
	  printf("done!!!\n");
      easy = msg->easy_handle;
      res = msg->data.result;
      curl_easy_getinfo(easy, CURLINFO_PRIVATE, &conn);
      curl_easy_getinfo(easy, CURLINFO_EFFECTIVE_URL, &eff_url);
      fprintf(MSG_OUT,"R:%d DONE: %s => (%d) %s\n",msg->data.result, eff_url, res, conn->error);
      curl_multi_remove_handle(g->multi, easy);
      free(conn->url);
      curl_easy_cleanup(easy);
      free(conn);
    }
  }
}



/* Called by libevent when we get action on a multi socket */
static void event_cb(int fd, short kind, void *userp) {
  printf("------->event_cb\n");
  GlobalInfo* g = (GlobalInfo*) userp;
  CURLMcode rc;

  int action =
    (kind & EV_READ ? CURL_CSELECT_IN : 0) |
    (kind & EV_WRITE ? CURL_CSELECT_OUT : 0);

  printf("+++++++++++++>\n");
  rc = curl_multi_socket_action(g->multi, fd, action, &g->still_running); //里面回调sock_cb 看doc这个函数主要用在 当libcurl检测到有事件到来
  printf("<+++++++++++++\n");
  mcode_or_die("event_cb: curl_multi_socket_action", rc);

  check_multi_info(g);
  if(g->still_running <= 0) {
    fprintf(MSG_OUT, "last transfer done, kill timeout\n");
    if(evtimer_pending(g->timer_event, NULL)) {
      evtimer_del(g->timer_event);
    }
  }
  printf("<-------event_cb\n");
}



/* Called by libevent when our timeout expires */
static void timer_cb(int fd, short kind, void *userp) {
  GlobalInfo *g = (GlobalInfo *)userp;
  CURLMcode rc;
  (void)fd;
  (void)kind;

  rc = curl_multi_socket_action(g->multi,  //----------------这里有坑
                                  CURL_SOCKET_TIMEOUT, 0, &g->still_running);
  mcode_or_die("timer_cb: curl_multi_socket_action", rc);
  check_multi_info(g);
}



/* Clean up the SockInfo structure */
static void remsock(SockInfo *f)
{
  if(f) {
    if(f->evset)
      event_free(f->ev);
    free(f);
  }
}



/* Assign information to a SockInfo structure */
static void setsock(SockInfo *f, curl_socket_t s, CURL *e, int act, GlobalInfo *g) {
  int kind = (act&CURL_POLL_IN?EV_READ:0)|(act&CURL_POLL_OUT?EV_WRITE:0)|EV_PERSIST;

  f->sockfd = s;
  f->action = act;
  f->easy = e;
  if(f->evset)
    event_free(f->ev);
  f->ev = event_new(g->evbase, f->sockfd, kind, event_cb, g);
  f->evset = 1;
  event_add(f->ev, NULL);
}

/* Initialize a new SockInfo structure */
static void addsock(curl_socket_t s, CURL *easy, int action, GlobalInfo *g) {
  SockInfo* fdp = calloc(sizeof(SockInfo), 1);
  fdp->global = g;
  setsock(fdp, s, easy, action, g);
  curl_multi_assign(g->multi, s, fdp);
}

static int sock_cb(CURL *e, curl_socket_t s/*套接字*/, int what/*待监听套接字什么事件*/, void *cbp, void* sockp) {
  printf("=======>socket cb\n");
  GlobalInfo *g = (GlobalInfo*) cbp;
  SockInfo* fdp = (SockInfo*) sockp;
  const char *whatstr[]={ "none", "IN", "OUT", "INOUT", "REMOVE" }; 
  fprintf(MSG_OUT, "socket callback: s=%d e=%p what=%s ", s, e, whatstr[what]); //首先是in事件到来s=7 e=0x11f7000 what=IN Adding data: IN

  if(what == CURL_POLL_REMOVE) {
    fprintf(MSG_OUT, "\n");
    remsock(fdp);
  } else {
    if(!fdp) {
      fprintf(MSG_OUT, "Adding data: %s\n", whatstr[what]);
      addsock(s, e, what, g);
    } else {
      fprintf(MSG_OUT, "Changing action from %s to %s\n", whatstr[fdp->action], whatstr[what]);
      setsock(fdp, s, e, what, g); //里面吧读写事件都挂libevent树上
    }
  }
  printf("<=======socket cb\n");
  return 0;
}

static size_t write_cb(void *ptr, size_t size, size_t nmemb, void* data) {
  size_t realsize = size * nmemb;
  ConnInfo* conn = (ConnInfo*) data;
  (void)ptr;
  (void)conn;
  return realsize;
}


static int prog_cb(void *p, double dltotal, double dlnow, double ult, double uln) {
  ConnInfo *conn = (ConnInfo *)p;
  (void)ult;
  (void)uln;

  fprintf(MSG_OUT, "Progress: %s (%g/%g)\n", conn->url, dlnow, dltotal);
  return 0;
}

static void dump(const char *text, FILE *stream, unsigned char *ptr, size_t size, bool nohex) {
  size_t i;
  size_t c;

  unsigned int width = 0x10;
  if(nohex)
  /* without the hex output, we can fit more on screen */
  width = 0x40;

  fprintf(stream, "%s, %10.10lu bytes (0x%8.8lx)\n", text, (unsigned long)size, (unsigned long)size);

  for(i = 0; i<size; i += width) {
	fprintf(stream, "%4.4lx: ", (unsigned long)i);
	if(!nohex) {
	  /* hex not disabled, show it */
	  for(c = 0; c < width; c++)
	  if(i + c < size)
	  fprintf(stream, "%02x ", ptr[i + c]);
	  else
	  fputs("   ", stream);
	}

	for(c = 0; (c < width) && (i + c < size); c++) {
	  /* check for 0D0A; if found, skip past and start a new line of output */
	  if(nohex && (i + c + 1 < size) && ptr[i + c] == 0x0D && ptr[i + c + 1] == 0x0A) {
		i += (c + 2 - width);
		break;
	  }
	  fprintf(stream, "%c", (ptr[i + c] >= 0x20) && (ptr[i + c]<0x80)?ptr[i + c]:'.');
	  /* check again for 0D0A, to avoid an extra \n if it's at width */
	  if(nohex && (i + c + 2 < size) && ptr[i + c + 1] == 0x0D && ptr[i + c + 2] == 0x0A) {
		i += (c + 3 - width);
		break;
	  }
	}
	fputc('\n', stream); /* newline */
  }
  fflush(stream);
}

static int my_trace(CURL* handle, curl_infotype type, unsigned char* data, size_t size, void* userp) {
  const char *text;
  (void)userp;
  (void)handle; /* prevent compiler warning */

  switch(type) {
	case CURLINFO_TEXT:
	  fprintf(stderr, "== Info: %s", data);
	  /* FALLTHROUGH */
	default: /* in case a new one is introduced to shock us */
	  return 0;

	case CURLINFO_HEADER_OUT:
	  text = "=> Send header";
	  break;
	case CURLINFO_DATA_OUT:
	  text = "=> Send data";
	  break;
	case CURLINFO_HEADER_IN:
	  text = "<= Recv header";
	  break;
	case CURLINFO_DATA_IN:
	  text = "<= Recv data";
	  break;
  }
  //dump(text, stderr, data, size, TRUE);
  printf("            trace:%s\n", text);
  return 0;
}


static void new_conn(char* url, GlobalInfo* g) {
  CURLMcode rc;
  ConnInfo* conn = calloc(1, sizeof(ConnInfo));
  memset(conn, 0, sizeof(ConnInfo));
  conn->error[0]='\0';

  conn->easy = curl_easy_init(); //每来一个url 都会起一个handle
  if(!conn->easy) {
    fprintf(MSG_OUT, "curl_easy_init() failed, exiting!\n");
    exit(2);
  }
  conn->global = g;
  conn->url = strdup(url);
  curl_easy_setopt(conn->easy, CURLOPT_URL, conn->url);
  curl_easy_setopt(conn->easy, CURLOPT_DEBUGFUNCTION, my_trace);
  curl_easy_setopt(conn->easy, CURLOPT_WRITEFUNCTION, write_cb); //收到数据的回调
  curl_easy_setopt(conn->easy, CURLOPT_WRITEDATA, conn);
  curl_easy_setopt(conn->easy, CURLOPT_VERBOSE, 1L); //更多http信息
  curl_easy_setopt(conn->easy, CURLOPT_ERRORBUFFER, conn->error);
  curl_easy_setopt(conn->easy, CURLOPT_PRIVATE, conn);
  //curl_easy_setopt(conn->easy, CURLOPT_NOPROGRESS, 0L);
  //curl_easy_setopt(conn->easy, CURLOPT_PROGRESSFUNCTION, prog_cb); //收到数据的进度  感觉有点鸡肋
  //curl_easy_setopt(conn->easy, CURLOPT_PROGRESSDATA, conn);
  fprintf(MSG_OUT, "Adding easy %p to multi %p (%s)\n", conn->easy, g->multi, url);
  rc = curl_multi_add_handle(g->multi, conn->easy);
  mcode_or_die("new_conn: curl_multi_add_handle", rc);

  /* note that the add_handle() will set a time-out to trigger very soon so
     that the necessary socket_action() call will be called by this app */
}

static void fifo_cb(int fd, short event, void* arg) { //libevent回调 读事件到来
  char s[1024];
  long int rv = 0;
  int n = 0;
  GlobalInfo* g = (GlobalInfo *)arg;
  (void)fd; /* unused */
  (void)event; /* unused */

  do {
    s[0]='\0';
    rv = fscanf(g->input, "%1023s%n", s, &n);
    s[n]='\0';
    if(n && s[0]) {
      new_conn(s, arg);
    } else break;
  } while(rv != EOF);
}

/* Create a named pipe and tell libevent to monitor it */
static const char *fifo = "hiper.fifo";
static int init_fifo(GlobalInfo* g) {
  struct stat st;
  curl_socket_t sockfd;

  fprintf(MSG_OUT, "Creating named pipe \"%s\"\n", fifo);
  if(lstat (fifo, &st) == 0) {
    if((st.st_mode & S_IFMT) == S_IFREG) {
      errno = EEXIST;
      perror("lstat");
      exit(1);
    }
  }
  unlink(fifo);
  if(mkfifo (fifo, 0600) == -1) {
    perror("mkfifo");
    exit(1);
  }
  sockfd = open(fifo, O_RDWR | O_NONBLOCK, 0);
  if(sockfd == -1) {
    perror("open");
    exit(1);
  }
  g->input = fdopen(sockfd, "r");

  fprintf(MSG_OUT, "Now, pipe some URL's into > %s\n", fifo);
  g->fifo_event = event_new(g->evbase, sockfd, EV_READ|EV_PERSIST, fifo_cb, g);
  event_add(g->fifo_event, NULL);
  return (0);
}

static void clean_fifo(GlobalInfo *g)
{
    event_free(g->fifo_event);
    fclose(g->input);
    unlink(fifo);
}

int main(int argc, char** argv) {
  GlobalInfo g;
  (void)argc;
  (void)argv;

  memset(&g, 0, sizeof(GlobalInfo));
  g.evbase = event_base_new();
  init_fifo(&g);
  g.multi = curl_multi_init();
  g.timer_event = evtimer_new(g.evbase, timer_cb, &g);

  /* setup the generic multi interface options we want */
  curl_multi_setopt(g.multi, CURLMOPT_SOCKETFUNCTION, sock_cb); //fd发生变化即回调
  curl_multi_setopt(g.multi, CURLMOPT_SOCKETDATA, &g);
  curl_multi_setopt(g.multi, CURLMOPT_TIMERFUNCTION, multi_timer_cb);
  curl_multi_setopt(g.multi, CURLMOPT_TIMERDATA, &g);

  /* we don't call any curl_multi_socket*() function yet as we have no handles
     added! */

  event_base_dispatch(g.evbase);

  /* this, of course, won't get called since only way to stop this program is
     via ctrl-C, but it is here to show how cleanup /would/ be done. */
  clean_fifo(&g);
  event_free(g.timer_event);
  event_base_free(g.evbase);
  curl_multi_cleanup(g.multi);
  return 0;
}

//https://curl.haxx.se/libcurl/c/CURLMOPT_SOCKETFUNCTION.html
//监听工作仍然是交给了我 我去调用libevent   

/*
When the curl_multi_socket_action function runs, it informs the application about updates in the socket (file descriptor) status by doing none, one, or multiple calls to the socket_callback. The callback gets status updates with changes since the previous time the callback was called. If the given callback pointer is NULL, no callback will be called. Set the callback's userp argument with CURLMOPT_SOCKETDATA. See curl_multi_socket_action for more details on how the callback is used and should work.
*/

/*
== Info: Rebuilt URL to: www.baidu.com/

=======>socket cb     //1可能是add_handle 后触发的
socket callback: s=7 e=0x20af000 what=IN Adding data: IN
<=======socket cb

multi_timer_cb: Setting timeout to 0 ms
REMAINING...: 1
multi_timer_cb: Setting timeout to 5000 ms
REMAINING...: 1

------->event_cb      //2libevent监听到fifo有事件到来 curl移除in方向fd监听  curl发起链接 然后监听out方向的fd   好像是个server模型 但是为什么两个fd显示的是同一个值?
					  //所有的libevent 监听到的事件 都要手动调用再触发libcurl
+++++++++++++>
=======>socket cb
socket callback: s=7 e=0x20af000 what=REMOVE 
<=======socket cb
== Info:   Trying 220.181.111.188...
== Info: TCP_NODELAY set
=======>socket cb
socket callback: s=7 e=0x20af000 what=OUT Adding data: OUT
<=======socket cb
multi_timer_cb: Setting timeout to 199 ms
<+++++++++++++
REMAINING...: 1
<-------event_cb


------->event_cb     //3libevent监听到可写 即建连成功 curl写 然后移除out方向fd监听
+++++++++++++>
== Info: Connected to www.baidu.com (220.181.111.188) port 80 (#0)
=======>socket cb
socket callback: s=7 e=0x20af000 what=REMOVE 
<=======socket cb
<+++++++++++++
REMAINING...: 1
<-------event_cb
            trace:=> Send header


=======>socket cb    //4libcurl开始监听in方向
socket callback: s=7 e=0x20af000 what=IN Adding data: IN
<=======socket cb


multi_timer_cb: Setting timeout to 149800 ms
REMAINING...: 1



------->event_cb    //5libevent监听到可读
+++++++++++++>
            trace:<= Recv header
            trace:<= Recv header
            trace:<= Recv header
            trace:<= Recv header
            trace:<= Recv header
            trace:<= Recv header
            trace:<= Recv header
            trace:<= Recv header
            trace:<= Recv header
            trace:<= Recv header
            trace:<= Recv header
            trace:<= Recv header
            trace:<= Recv header
            trace:<= Recv header
            trace:<= Recv header
            trace:<= Recv header
            trace:<= Recv header
            trace:<= Recv header
            trace:<= Recv data
<+++++++++++++
REMAINING...: 1
<-------event_cb



------->event_cb
+++++++++++++>
            trace:<= Recv data
== Info: Connection #0 to host www.baidu.com left intact
=======>socket cb
socket callback: s=7 e=0x20af000 what=REMOVE 
<=======socket cb
<+++++++++++++
REMAINING...: 0
done!!!
R:0 DONE: http://www.baidu.com/ => (0) 
multi_timer_cb: Setting timeout to -1 ms
last transfer done, kill timeout
<-------event_cb
*/
