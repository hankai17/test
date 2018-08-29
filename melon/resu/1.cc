/*
 * 编译：c++ -o epoll epoll.cc
 * 运行： ./epoll
 * 测试：curl -v localhost
 */
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <signal.h>
using namespace std;

bool output_log = true;

#define exit_if(r, ...) if(r) {printf(__VA_ARGS__); printf("%s:%d error no: %d error msg %s\n", __FILE__, __LINE__, errno, strerror(errno)); exit(1);}

void set_non_block(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  exit_if(flags<0, "fcntl failed");

  int r = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
  exit_if(r<0, "fcntl failed");
}

void update_events(int efd, int fd, int events, int op) {
  struct epoll_event ev;
  memset(&ev, 0, sizeof(ev));
  ev.events = events;
  ev.data.fd = fd;
  printf("%s fd %d events read %d write %d\n",
		op==EPOLL_CTL_MOD?"mod":"add", fd, ev.events & EPOLLIN, ev.events & EPOLLOUT);
  int r = epoll_ctl(efd, op, fd, &ev);
  exit_if(r, "epoll_ctl failed");
}

void handle_accept(int efd, int fd) { //cfd挂树上
  struct sockaddr_in raddr;
  socklen_t rsz = sizeof(raddr);
  int cfd = accept(fd,(struct sockaddr *)&raddr, &rsz);
  exit_if(cfd<0, "accept failed");

  sockaddr_in peer, local;
  socklen_t alen = sizeof(peer);
  int r = getpeername(cfd, (sockaddr*)&peer, &alen);
  exit_if(r<0, "getpeername failed");
  printf("accept a connection from %s\n", inet_ntoa(raddr.sin_addr));

  set_non_block(cfd);
  update_events(efd, cfd, EPOLLIN, EPOLL_CTL_ADD);
}

struct Connect {
  string readed;
  size_t written;
  bool writeEnabled;
  Connect(): written(0), writeEnabled(false) {}
};
map<int, Connect> cons;

string http_res;
void send_res(int efd, int fd) {
  Connect& con = cons[fd];
  size_t left = http_res.length() - con.written;
  int wd = 0;
  while((wd = ::write(fd, http_res.data()+con.written, left)) > 0) {
	con.written += wd;
	left -= wd;
	if(output_log) printf("write %d bytes left: %lu\n", wd, left);
  };
  if (left == 0) {
	//        close(fd); // 测试中使用了keepalive，因此不关闭连接。连接会在read事件中关闭
	if (con.writeEnabled) {
	  update_events(efd, fd, EPOLLIN, EPOLL_CTL_MOD); // 当所有数据发送结束后，不再关注其缓冲区可写事件
	  con.writeEnabled = false;
	}
	cons.erase(fd);
	return;
  }
  if (wd < 0 &&  (errno == EAGAIN || errno == EWOULDBLOCK)) {
	if (!con.writeEnabled) {
	  update_events(efd, fd, EPOLLIN|EPOLLOUT, EPOLL_CTL_MOD);
	  con.writeEnabled = true;
	}
	return;
  }
  if (wd<=0) {
	printf("write error for %d: %d %s\n", fd, errno, strerror(errno));
	close(fd);
	cons.erase(fd);
  }
}

void handle_read(int efd, int fd) {
  char buf[4096];
  int n = 0;
  while ((n = ::read(fd, buf, sizeof buf)) > 0) { //循环读
	if(output_log) printf("read %d bytes\n", n);
	string& readed = cons[fd].readed;
	readed.append(buf, n);
	if (readed.length() > 4) {
	  if (readed.substr(readed.length()-2, 2) == "\n\n" || readed.substr(readed.length()-4, 4) == "\r\n\r\n") { //当读取到一个完整的http请求，测试发送响应
		send_res(efd, fd);
	  }
	}
  }
  if ( n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) return;
  //实际应用中，n<0应当检查各类错误，如EINTR
  if (n < 0) {
	printf("read %d error: %d %s\n", fd, errno, strerror(errno));
  }
  close(fd);
  cons.erase(fd);
}

void handle_write(int efd, int fd) {
  send_res(efd, fd);
}

void loop_once(int efd, int lfd, int waitms) {
  const int max_events = 20;
  struct epoll_event active_evs[100];

  int n = epoll_wait(efd, active_evs, max_events, waitms);

  if(output_log) printf("epoll_wait return %d\n", n);
  for (int i = 0; i < n; i ++) {
	int fd = active_evs[i].data.fd;
	int events = active_evs[i].events;
	if (events & (EPOLLIN | EPOLLERR)) { //读事件到来
	  if (fd == lfd) {
		handle_accept(efd, fd);
	  } else {
		handle_read(efd, fd);
	  }
	} else if (events & EPOLLOUT) {
	  if(output_log) printf("handling epollout\n");
	  handle_write(efd, fd);
	} else {
	  exit_if(1, "unknown event");
	}
  }
}

int main(int argc, const char* argv[]) {
  if (argc > 1) { output_log = false; }
  ::signal(SIGPIPE, SIG_IGN);
  http_res = "HTTP/1.1 200 OK\r\nConnection: Keep-Alive\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: 1048576\r\n\r\n123456";
  for(int i = 0; i < 1048570; i++) {
	http_res += '\0';
  }
  short port = 80;
  int epollfd = epoll_create(1);
  exit_if(epollfd < 0, "epoll_create failed");

  int lfd = socket(AF_INET, SOCK_STREAM, 0);
  exit_if(lfd < 0, "socket failed");

  struct sockaddr_in addr;
  memset(&addr, 0, sizeof addr);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;

  int r = ::bind(lfd,(struct sockaddr *)&addr, sizeof(struct sockaddr));
  exit_if(r, "bind to 0.0.0.0:%d failed %d %s", port, errno, strerror(errno));

  r = listen(lfd, 20);
  exit_if(r, "listen failed %d %s", errno, strerror(errno));
  printf("fd %d listening at %d\n", lfd, port);

  set_non_block(lfd);
  update_events(epollfd, lfd, EPOLLIN, EPOLL_CTL_ADD); //lfd挂树上

  for (;;) { //实际应用应当注册信号处理函数 退出时清理资源
	loop_once(epollfd, lfd, 10000);
  }
  return 0;
}
