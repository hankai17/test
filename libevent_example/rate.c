#include "../util-internal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <sys/socket.h>
#include <netinet/in.h>
# ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#include <signal.h>
#include "event2/bufferevent.h"
#include "event2/buffer.h"
#include "event2/event.h"
#include "event2/util.h"
#include "event2/listener.h"
#include "event2/thread.h"

static struct evutil_weakrand_state weakrand_state;
static int cfg_n_connections = 1;
static int cfg_duration = 5;
static int cfg_connlimit = 102400; //100K
static int cfg_tick_msec = 1000;
static int cfg_min_share = -1;
static int cfg_group_drain = 0;
static struct timeval cfg_tick = { 0, 500*1000 };

static struct ev_token_bucket_cfg* conn_bucket_cfg = NULL;
static double seconds_per_tick = 0.0;

struct client_state {
  size_t queued;
  ev_uint64_t received;

};
static const struct timeval* ms100_common = NULL;

/* info from check_bucket_levels_cb */
static int total_n_bev_checks = 0;
static ev_int64_t total_rbucket_level=0;
static ev_int64_t total_wbucket_level=0;
static ev_int64_t total_max_to_read=0;
static ev_int64_t total_max_to_write=0;
static ev_int64_t max_bucket_level=EV_INT64_MIN;
static ev_int64_t min_bucket_level=EV_INT64_MAX;

static int n_echo_conns_open = 0;

/* Info on the open connections */
struct bufferevent** bevs;
struct client_state* states;
static void check_bucket_levels_cb(evutil_socket_t fd, short events, void *arg);

static void loud_writecb(struct bufferevent *bev, void *ctx) {
  struct client_state* cs = ctx;
  struct evbuffer* output = bufferevent_get_output(bev);
  char buf[1024];
  int r = evutil_weakrand_(&weakrand_state);
  memset(buf, r, sizeof(buf));
  while (evbuffer_get_length(output) < 8192) {
    evbuffer_add(output, buf, sizeof(buf));
    cs->queued += sizeof(buf);
  }
}

static void discard_readcb(struct bufferevent* bev, void* ctx) {
  struct client_state* cs = ctx;
  struct evbuffer* input = bufferevent_get_input(bev);
  size_t len = evbuffer_get_length(input);
  evbuffer_drain(input, len);
  cs->received += len;
}

static void write_on_connectedcb(struct bufferevent *bev, short what, void* ctx) {
  if (what & BEV_EVENT_CONNECTED) {
    loud_writecb(bev, ctx);
    /* XXXX this shouldn't be needed. */
    bufferevent_enable(bev, EV_READ|EV_WRITE);
  }
}

static void echo_readcb(struct bufferevent *bev, void *ctx) {
  struct evbuffer* input = bufferevent_get_input(bev);
  struct evbuffer* output = bufferevent_get_output(bev);
  evbuffer_add_buffer(output, input); //从in--->out
  if (evbuffer_get_length(output) > 1024000)
  bufferevent_disable(bev, EV_READ);
}

static void echo_writecb(struct bufferevent *bev, void *ctx) {
  struct evbuffer* output = bufferevent_get_output(bev);
  if (evbuffer_get_length(output) < 512000)
  bufferevent_enable(bev, EV_READ);
}

static void echo_eventcb(struct bufferevent *bev, short what, void *ctx) {
  if (what & (BEV_EVENT_EOF|BEV_EVENT_ERROR)) {
    --n_echo_conns_open;
    bufferevent_free(bev);
  }
  if (total_n_bev_checks) {
    printf("Average read bucket level: %f\n",
          (double)total_rbucket_level/total_n_bev_checks);
    printf("Average write bucket level: %f\n",
          (double)total_wbucket_level/total_n_bev_checks);
    printf("Highest read bucket level: %f\n",
          (double)max_bucket_level);
    printf("Highest write bucket level: %f\n",
          (double)min_bucket_level);
    printf("Average max-to-read: %f\n",
          ((double)total_max_to_read)/total_n_bev_checks);
    printf("Average max-to-write: %f\n",
          ((double)total_max_to_write)/total_n_bev_checks);
  }
  printf("client close\n");
}

static void echo_listenercb(struct evconnlistener* listener, evutil_socket_t newsock,
      struct sockaddr* sourceaddr, int socklen, void* ctx) { //每来一个连接 都回调一次
  struct event_base* base = ctx;
  int flags = BEV_OPT_CLOSE_ON_FREE|BEV_OPT_THREADSAFE;
  struct bufferevent* bev;

  bev = bufferevent_socket_new(base, newsock, flags); //被动套接字 设置套接字flag
  bufferevent_setcb(bev, echo_readcb, echo_writecb, echo_eventcb, NULL); //读写回调 
  if (conn_bucket_cfg) { //标记容器 限速的核心句柄
    struct event* check_event = event_new(base, -1, EV_PERSIST, check_bucket_levels_cb, bev); //check_bucket事件
    bufferevent_set_rate_limit(bev, conn_bucket_cfg);
    assert(bufferevent_get_token_bucket_cfg(bev) != NULL);
    event_add(check_event, ms100_common);
  }
  ++n_echo_conns_open;
  bufferevent_enable(bev, EV_READ|EV_WRITE);
}

/* Called periodically to check up on how full the buckets are */
static void check_bucket_levels_cb(evutil_socket_t fd, short events, void *arg) {
  struct bufferevent* bev = arg;
  ev_ssize_t r = bufferevent_get_read_limit(bev); //当前速率
  ev_ssize_t w = bufferevent_get_write_limit(bev);
  ev_ssize_t rm = bufferevent_get_max_to_read(bev);
  ev_ssize_t wm = bufferevent_get_max_to_write(bev);
  /* XXXX check that no value is above the cofigured burst limit */
  total_rbucket_level += r;
  total_wbucket_level += w;
  total_max_to_read += rm;
  total_max_to_write += wm;

#define B(x) \
  if ((x) > max_bucket_level)		\
  max_bucket_level = (x);		\
  if ((x) < min_bucket_level)		\
  min_bucket_level = (x)
  B(r);
  B(w);
#undef B

  total_n_bev_checks++;
  if (total_n_bev_checks >= .8 * ((double)cfg_duration / cfg_tick_msec) * cfg_n_connections) {
    event_free(event_base_get_running_event(bufferevent_get_base(bev)));
  }
}

static int test_ratelimiting(void) {
  struct event_base* base;
  struct sockaddr_in sin;
  struct evconnlistener* listener;
  struct sockaddr_storage ss;
  ev_socklen_t slen;
  struct timeval tv;
  double expected_avg_persec = -1.0;

  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  //sin.sin_addr.s_addr = htonl(0x7f000001); /* 127.0.0.1 */
  //sin.sin_addr.s_addr = htonl(INADDR_ANY);
  sin.sin_addr.s_addr = htonl(0);
  //sin.sin_port = 9999; /* unspecified port */
  sin.sin_port = htons(9999); /* unspecified port */

  //if (1) event_enable_debug_mode();

  base = event_base_new();
  if (! base) {
    fprintf(stderr, "Couldn't create event_base");
    return 1;
  }

  listener = evconnlistener_new_bind(base, echo_listenercb, base, //accept回调
        LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE, -1,
        (struct sockaddr *)&sin, sizeof(sin));
  if (! listener) {
    fprintf(stderr, "Couldn't create listener");
    return 1;
  }

  slen = sizeof(ss);
  if (getsockname(evconnlistener_get_fd(listener), (struct sockaddr *)&ss, &slen) < 0) { //传出server ss
    perror("getsockname");
    return 1;
  }

  conn_bucket_cfg = ev_token_bucket_cfg_new( //标记容器 即限速的核心模块 
        cfg_connlimit, cfg_connlimit * 4, //读速率 
        cfg_connlimit, cfg_connlimit * 4, //写速率
        &cfg_tick); //tick
  assert(conn_bucket_cfg);

  expected_avg_persec = cfg_connlimit;

  /*
  bevs = calloc(cfg_n_connections, sizeof(struct bufferevent *)); //分配30个bufferevent
  states = calloc(cfg_n_connections, sizeof(struct client_state)); //分配30个cli_state
  for (i = 0; i < cfg_n_connections; ++i) {
    bevs[i] = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE|BEV_OPT_THREADSAFE); //30个connect 
    assert(bevs[i]);
    bufferevent_setcb(bevs[i], discard_readcb, loud_writecb, write_on_connectedcb, &states[i]); //befferevent的回调函数 
    bufferevent_enable(bevs[i], EV_READ|EV_WRITE);
    bufferevent_socket_connect(bevs[i], (struct sockaddr *)&ss, slen);
  }

  tv.tv_sec = cfg_duration - 1;
  tv.tv_usec = 995000;
  event_base_loopexit(base, &tv);
  */

  tv.tv_sec = 0;
  tv.tv_usec = 100*1000;
  ms100_common = event_base_init_common_timeout(base, &tv);

  event_base_dispatch(base);
  /*
  for (i = 0; i < cfg_n_connections; ++i) {
    bufferevent_free(bevs[i]);
  }
  */
  evconnlistener_free(listener);

  while (n_echo_conns_open) {
    printf("waiting for %d conns\n", n_echo_conns_open);
    tv.tv_sec = 0;
    tv.tv_usec = 300000;
    event_base_loopexit(base, &tv);
    event_base_dispatch(base);
  }

  /*
  total_received = 0;
  total_persec = 0.0;
  total_sq_persec = 0.0;
  for (i=0; i < cfg_n_connections; ++i) {
    double persec = states[i].received;
    persec /= cfg_duration;
    total_received += states[i].received;
    total_persec += persec;
    total_sq_persec += persec*persec;
    printf("%d: %f per second\n", i+1, persec);
  }
  printf("   total: %f per second\n",
        ((double)total_received)/cfg_duration);
  if (expected_total_persec > 0) {
    double diff = expected_total_persec -
      ((double)total_received/cfg_duration);
    printf("  [Off by %lf]\n", diff);
  }

  printf(" average: %f per second\n",
        (((double)total_received)/cfg_duration)/cfg_n_connections);
  if (expected_avg_persec > 0) {
    double diff = expected_avg_persec - (((double)total_received)/cfg_duration)/cfg_n_connections;
    printf("  [Off by %lf]\n", diff);
  }

  variance = total_sq_persec/cfg_n_connections - total_persec*total_persec/(cfg_n_connections*cfg_n_connections);

  printf("  stddev: %f per second\n", sqrt(variance));
  */
  event_base_free(base);
  free(bevs);
  free(states);
  return 0;
}

int main() 
{
  double ratio;
  evutil_weakrand_seed_(&weakrand_state, 0);

  cfg_tick.tv_sec = cfg_tick_msec / 1000;
  cfg_tick.tv_usec = (cfg_tick_msec % 1000)*1000;
  seconds_per_tick = ratio = cfg_tick_msec / 1000.0;

  cfg_connlimit*= ratio;
  {
    struct timeval tv;
    evutil_gettimeofday(&tv, NULL);
    srandom(tv.tv_usec);
  }
#ifndef EVENT__DISABLE_THREAD_SUPPORT
  evthread_enable_lock_debugging();
#endif
  return test_ratelimiting();
}

/*
主函数调read之前会调readmax = bufferevent_get_read_max_(bufev_p); 即根据bfe的私有数据 获取当前要读多少数据
会调bufferevent_ratelim.c中 return bufferevent_get_rlim_max_(bev, 0);
        if (bev->rate_limiting->cfg) {
                bufferevent_update_buckets(bev); //这里根据限速桶计算出要读写的字节数(见下)
                max_so_far = LIM(bev->rate_limiting->limit);                                                        
        }
        
        bufferevent_update_buckets(struct bufferevent_private *bev)
        {
          struct timeval now;
          unsigned tick;
          event_base_gettimeofday_cached(bev->bev.ev_base, &now);
          tick = ev_token_bucket_get_tick_(&now, bev->rate_limiting->cfg); //当前时间的tick个数  可以想象tick(y)随时间(x)是个一次函数 y=kx
          if (tick != bev->rate_limiting->limit.last_updated)    //(见下)
                ev_token_bucket_update_(&bev->rate_limiting->limit,
                    bev->rate_limiting->cfg, tick);
}

ev_token_bucket_update_(struct ev_token_bucket *bucket, cfg, current_tick)                                                                                                                                                                                          
{                                                                                                                                                                                                                      
        unsigned n_ticks = current_tick - bucket->last_updated; //当前时间的tick与上次read时tick差值
        if ((cfg->read_maximum - bucket->read_limit) / n_ticks < cfg->read_rate)
                bucket->read_limit = cfg->read_maximum;
        else
                bucket->read_limit += n_ticks * cfg->read_rate; //根据两个tick差值计算出本次要读、写的字节数 很明显感性上差值越大要读写的字节数就越多

        bucket->last_updated = current_tick;
        return 1;
}


*/
