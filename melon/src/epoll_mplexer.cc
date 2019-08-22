#include "epoll_mplexer.hh"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/time.h>

int EpollMultiplexer::s_max_events = 1024;

EpollMultiplexer::EpollMultiplexer() : d_events(new epoll_event[s_max_events]) {
  d_epoll_fd = epoll_create(s_max_events);
  if(d_epoll_fd < 0) throw FDMultiplexerError("Setting up epoll: ");
  int fd = socket(AF_INET, SOCK_DGRAM, 0); //--------------------------stream 只是测试
  if(fd < 0) return;
  try {
	add_read_fd(fd, 0);
	remove_read_fd(fd);
	close(fd);
	return;
  } catch(FDMultiplexerError& fe) {
	close(fd);
	close(d_epoll_fd);
	throw FDMultiplexerError("epoll multiplexer failed self-test: "+string(fe.what()));
  }
}

static FDMultiplexer* makeEpoll() {
  return new EpollMultiplexer();
}
/*
static struct EpollRegisterOurselves {
  EpollRegisterOurselves() {
	FDMultiplexer::getMultiplexerMap().insert(make_pair(0, &makeEpoll));
  }
} doItEpoll;
*/

void EpollMultiplexer::add_fd(callbackmap_t& cbmap, int fd, callbackfunc_t todo, const funcparam_t& para) {
  accounting_add_fd(cbmap, fd, todo, para);

  struct epoll_event e_event;
  e_event.events = (&cbmap == &m_read_cbs) ? EPOLLIN : EPOLLOUT;
  e_event.data.u64 = 0;
  e_event.data.fd = fd;
  if(epoll_ctl(d_epoll_fd, EPOLL_CTL_ADD, fd, &e_event) < 0 && errno != EEXIST) {
	cbmap.erase(fd);
	throw FDMultiplexerError("Adding fd to epoll set: ");
  }
}

void EpollMultiplexer::remove_fd(callbackmap_t& cbmap, int fd) {
  if(cbmap.erase(fd) != 1) throw FDMultiplexerError("Tried to remove unlisted fd "+std::to_string(fd)+ " from multiplexer");
  struct epoll_event e_event;
  e_event.events = 0;
  e_event.data.u64 = 0;
  if(epoll_ctl(d_epoll_fd, EPOLL_CTL_DEL, fd, &e_event) < 0)   
  throw FDMultiplexerError("Removing fd from epoll set: ");
}

int EpollMultiplexer::run(struct timeval* now) {
  int ret = epoll_wait(d_epoll_fd, d_events.get(), s_max_events, 500);
  gettimeofday(now, 0);
  if(ret < 0 && errno != EINTR)
  throw FDMultiplexerError("epoll returned error: ");
  if(ret < 1) return 0; //Will loop In outer
  for(int i = 0; i < ret; i++) {
	auto iter = m_read_cbs.find(d_events[i].data.fd); //用不到泛型指针
	if(iter != m_read_cbs.end()) {
	  iter->second.d_callback(iter->first, iter->second.d_parameter);
	  continue;
	}

	iter = m_write_cbs.find(d_events[i].data.fd);
	if(iter != m_write_cbs.end()) {
	  iter->second.d_callback(iter->first, iter->second.d_parameter);
	  //continue;
	}
  }
  return 0;
}

#if 0
void acceptData(int fd, funcparam_t& parameter)
{
  cout<<"Have data on fd "<<fd<<endl;
  Socket* sock=funcparam_t_cast<Socket*>(parameter);
  string packet;
  IPEndpoint rem;
  sock->recvFrom(packet, rem);
  cout<<"Received "<<packet.size()<<" bytes!\n";
}


int main()
{
  Socket s(AF_INET, SOCK_DGRAM);

  IPEndpoint loc("0.0.0.0", 2000);
  s.bind(loc);

  EpollMultiplexer sfm;

  sfm.addReadFD(s.getHandle(), &acceptData, &s);

  for(int n=0; n < 100 ; ++n) {
	sfm.run();
  }
  sfm.removeReadFD(s.getHandle());
  sfm.removeReadFD(s.getHandle());
};
#endif
