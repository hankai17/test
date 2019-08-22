#pragma once
#include "multiplex.hh"
#include <iostream>
#include <unistd.h>
#include <sys/epoll.h>
#include "namespaces.hh"

class EpollMultiplexer : public FDMultiplexer {
  public:
	EpollMultiplexer();
	virtual ~EpollMultiplexer() {
	  close(d_epoll_fd);
	}
	virtual int run(struct timeval* tv);
	virtual void add_fd(callbackmap_t& cbmap, int fd, callbackfunc_t todo, const funcparam_t& para);
	virtual void remove_fd(callbackmap_t& cbmap, int fd);
	string get_name() { return "epoll"; }
  private:
	int d_epoll_fd;
	boost::shared_array<epoll_event> d_events; //RAII ÎªÁË²»delte[]
	static int s_max_events;
};

