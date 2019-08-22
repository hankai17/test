#include "select_mplexer.hh"
#include <sys/time.h>

void SelectMultiplexer::add_fd(callbackmap_t& cbmap, int fd, callbackfunc_t todo, const funcparam_t& para) {
  CallBack cb;
  cb.d_callback = todo;
  cb.d_parameter = para;
  memset(&cb.d_ttd, 0, sizeof(cb.d_ttd));
  if(cbmap.count(fd))
  throw FDMultiplexerError("Tried to add fd "+std::to_string(fd)+ " to multiplexer twice");
  cbmap[fd] = cb;
}

void SelectMultiplexer::remove_fd(callbackmap_t& cbmap, int fd) {
  if(d_inrun && d_iter->first == fd) //???
  d_iter++;
  if(!cbmap.erase(fd))
  throw FDMultiplexerError("Tried to remove unlisted fd "+std::to_string(fd)+ " from multiplexer");
}

int SelectMultiplexer::run(struct timeval* now) {
  fd_set read_fds, write_fds;
  FD_ZERO(&read_fds); FD_ZERO(&write_fds);
  int fd_max = 0;

  for(auto i = m_read_cbs.begin(); i != m_read_cbs.end(); i++) {
	FD_SET(i->first, &read_fds);	
	fd_max = max(i->first, fd_max);
  }
  for(auto i = m_write_cbs.begin(); i != m_write_cbs.end(); i++) {
	FD_SET(i->first, &write_fds);	
	fd_max = max(i->first, fd_max);
  }
  struct timeval tv = {0, 500000};
  int ret = select(fd_max + 1, &read_fds, &write_fds, 0, &tv);
  cout<<"select ret = "<<ret<<endl;
  gettimeofday(now, 0);
  if(ret < 0 && errno != EINTR)
  throw FDMultiplexerError("select returned error: " + string(strerror(errno)));
  if(ret < 1) return 0;
  //感觉这种设计模式有问题   读写回调 关键是回调只有那一个啊 而且调用过了 这里这样写不对 这里应该是sendevent
  //但是为什么还要这样封装呢?
  //因为pdns即是服务器又是客户端
/*
  for(auto i = m_read_cbs.begin(); i != m_read_cbs.end() && i->first <= fd_max; i++) {
    if(FD_ISSET(i->first, &read_fds) && i->first == lfd) {
      cout<<"begin read cb"<<endl;
	  i->second.d_callback(i->first, i->second.d_parameter);  
      cout<<"end read cb"<<endl;
	} else {
      MT
    }
  }
  for(auto i = m_write_cbs.begin(); i != m_write_cbs.end() && i->first <= fd_max; i++) {
    if(FD_ISSET(i->first, &write_fds)) {
	  i->second.d_callback(i->first, i->second.d_parameter);  
	  continue;
	}
  }
*/
  for(auto i = m_read_cbs.begin(); i != m_read_cbs.end() && i->first <= fd_max; i++) {
    if(FD_ISSET(i->first, &read_fds)) {
      cout<<"begin read cb"<<endl;
	  i->second.d_callback(i->first, i->second.d_parameter);  
      cout<<"end read cb"<<endl;
	  //continue;
	}
  }
  for(auto i = m_write_cbs.begin(); i != m_write_cbs.end() && i->first <= fd_max; i++) {
    if(FD_ISSET(i->first, &write_fds)) {
	  i->second.d_callback(i->first, i->second.d_parameter);  
	  //continue;
	}
  }
  cout<<"run return"<<endl;
  return 0;
}

static FDMultiplexer* make() { //为什么不设计成单例
  return new SelectMultiplexer();
}

//TODO
/* 
static struct RegisterOurselves {
  RegisterOurselves() {
	FDMultiplexer::get_multiplexer_map().insert(make_pair(1, &make));
  }
} doIt;
*/


#if 0
void acceptData(int fd, boost::any& parameter)
{
  cout<<"Have data on fd "<<fd<<endl;
  Socket* sock=boost::any_cast<Socket*>(parameter);
  string packet;
  IPEndpoint rem;
  sock->recvFrom(packet, rem);
  cout<<"Received "<<packet.size()<<" bytes!\n";
}


int main() {
  Socket s(AF_INET, SOCK_DGRAM);

  IPEndpoint loc("0.0.0.0", 2000);
  s.bind(loc);

  SelectMultiplexer sfm;

  sfm.addReadFD(s.getHandle(), &acceptData, &s);

  for(int n=0; n < 100 ; ++n) {
	sfm.run();
  }
  sfm.removeReadFD(s.getHandle());
  sfm.removeReadFD(s.getHandle());
}
#endif

