#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

static const char MESSAGE[] = "Hello, World!\n";
static const int PORT = 9995;

static void listener_cb(struct evconnlistener *, evutil_socket_t,
	  struct sockaddr *, int socklen, void *);
static void conn_writecb(struct bufferevent *, void *);
static void conn_eventcb(struct bufferevent *, short, void *);
static void signal_cb(evutil_socket_t, short, void *);

int main(int argc, char** argv) {
  struct event_base* base;
  struct evconnlistener* listener;
  struct event* signal_event;
  struct sockaddr_in sin;

  base = event_base_new();
  if (!base) {
	fprintf(stderr, "Could not initialize libevent!\n");
	return 1;
  }

  //初始化listen的server的ip
  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_port = htons(PORT);

  //创建一个listen的fd的事件 同时将这个事件让base监听 实际上监听的就是lfd的读事件
  listener = evconnlistener_new_bind(base, listener_cb, (void *)base,
		LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
		(struct sockaddr*)&sin,
		sizeof(sin));

  if (!listener) {
	fprintf(stderr, "Could not create a listener!\n");
	return 1;
  }

  //创建信号事件，绑定SIGINT信号， 当收到信号会触发 signal_cb 函数
  signal_event = evsignal_new(base, SIGINT, signal_cb, (void *)base);
  //初始化--->未决
  if (!signal_event || event_add(signal_event, NULL) < 0) {
	fprintf(stderr, "Could not create/add a signal event!\n");
	return 1;
  }
  event_base_dispatch(base);
  //回收一些资源
  evconnlistener_free(listener);
  event_free(signal_event);
  event_base_free(base);

  printf("done\n");
  return 0;
}

static void listener_cb(struct evconnlistener *listener, evutil_socket_t fd/*accept's cfd*/,
	  struct sockaddr* sa/*client ip*/, int socklen, void* user_data) {
  struct event_base* base = user_data;
  //绑定主动套接字一个事件 让base监控

  //创建bev
  struct bufferevent* bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
  if (!bev) {
	fprintf(stderr, "Error constructing bufferevent!");
	event_base_loopbreak(base);
	return;
  }
  //bev仅绑定写回调函数
  bufferevent_setcb(bev, NULL, conn_writecb, conn_eventcb, NULL); //注册写回调 的调用时机是已经发送出去了
  //启动bev写事件监听
  bufferevent_enable(bev, EV_WRITE);
  //关闭bev读事件监听
  bufferevent_disable(bev, EV_READ);
  //实际上 message 只是写到bufferevent 写缓冲区中
  bufferevent_write(bev, MESSAGE, strlen(MESSAGE));
}

static void conn_writecb(struct bufferevent* bev, void* user_data) {
  //从bev中的输出缓冲区中拿出数据地址
  struct evbuffer* output = bufferevent_get_output(bev);
  if (evbuffer_get_length(output) == 0) {
	printf("flushed answer\n");
	//由于设置了BEV_OPT_CLOSE_ON_FREE, 当free bufferevent事件的时候，也会将所关联的fd， close
	bufferevent_free(bev);
  }
}

static void conn_eventcb(struct bufferevent* bev, short events, void* user_data) {
  if (events & BEV_EVENT_EOF) {
	printf("Connection closed.\n");
  } else if (events & BEV_EVENT_ERROR) {
	printf("Got an error on the connection: %s\n",
		  strerror(errno));/*XXX win32*/
  }
  /* None of the other events can happen here, since we haven't enabled
   * timeouts */
  bufferevent_free(bev);
}

static void signal_cb(evutil_socket_t sig, short events, void* user_data) {
  struct event_base* base = user_data;
  struct timeval delay = { 2, 0 };
  printf("Caught an interrupt signal; exiting cleanly in two seconds.\n");
  event_base_loopexit(base, &delay);
}
