#pragma once
#include <boost/function.hpp>
#include <boost/any.hpp>
#include <boost/shared_array.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <vector>
#include <map>
#include <stdexcept>
#include <string>
#include "socket.hh"

class FDMultiplexerError : public std::runtime_error {
  public:
	FDMultiplexerError(const std::string& str) : std::runtime_error(str) {}
	FDMultiplexerError(const char* str) : std::runtime_error(str) {}
};

class FDMultiplexer {
  public:
    int lfd;
	//typedef boost::variant<PacketID, TCPConnection> funcparam_t;
	using funcparam_t = boost::any;
  protected:
	using callbackfunc_t = boost::function< void(int, funcparam_t&) >; //回调函数 (fd和参数)

	struct CallBack {
	  callbackfunc_t d_callback;
	  funcparam_t d_parameter;
	  struct timeval d_ttd;
	};

  public:
	FDMultiplexer() : d_inrun(false) {}
	virtual ~FDMultiplexer() {}
	virtual int run(struct timeval* tv) = 0; //核心函数
	virtual void add_read_fd(int fd, callbackfunc_t todo, const funcparam_t& para = funcparam_t()) {
	  this->add_fd(m_read_cbs, fd, todo, para);
	}
	virtual void add_write_fd(int fd, callbackfunc_t todo, const funcparam_t& para = funcparam_t()) {
	  this->add_fd(m_write_cbs, fd, todo, para);
	}
	virtual void remove_read_fd(int fd) {
	  this->remove_fd(m_read_cbs, fd);
	}
	virtual void remove_write_fd(int fd) {
	  this->remove_fd(m_write_cbs, fd);
	}

	virtual void set_read_ttd(int fd, struct timeval tv, int timeout) {
	  if(!m_read_cbs.count(fd))
	  throw FDMultiplexerError("attempt to timestamp fd not in the multiplexer");
	  tv.tv_sec += timeout;
	  m_read_cbs[fd].d_ttd = tv;
	}

	virtual funcparam_t& get_read_para(int fd) {
	  if(!m_read_cbs.count(fd))
	  throw FDMultiplexerError("attempt to look up data in multiplexer for unlisted fd "+std::to_string(fd));
	  return m_read_cbs[fd].d_parameter;
	}

	virtual std::vector< std::pair<int, funcparam_t> > get_timeouts(const struct timeval& tv) {
	  std::vector<std::pair<int, funcparam_t> > ret;
	  for(callbackmap_t::iterator i = m_read_cbs.begin(); i != m_read_cbs.end(); ++i)
	  if(i->second.d_ttd.tv_sec && boost::tie(tv.tv_sec, tv.tv_usec) > boost::tie(i->second.d_ttd.tv_sec, i->second.d_ttd.tv_usec)) 
	  ret.push_back(std::make_pair(i->first, i->second.d_parameter));
	  return ret;
	}

	virtual std::string get_name() = 0;
	//typedef FDMultiplexer* getMultiplexer_t();
	//typedef std::multimap<int, get_multiplexer_t*> FDMultiplexermap_t;

	using get_multiplexer_t = FDMultiplexer* ();
	using fd_multiplexer_map_t = std::multimap<int, get_multiplexer_t*>;
/*
	static m_fd_multiplexer& get_multiplexer_map() {
	  static m_fd_multiplexer the_map;
	  return the_map;
	}
*/
  protected:
	using callbackmap_t = std::map<int, CallBack>;
	callbackmap_t m_read_cbs, m_write_cbs; //fd->上层 //类似全局myevent数组  //都是抽象fd跟上层之间的联系
	bool d_inrun;
	callbackmap_t::iterator d_iter;

	virtual void add_fd(callbackmap_t& cbmap, int fd, callbackfunc_t todo, const funcparam_t& para) = 0;
	virtual void remove_fd(callbackmap_t& cbmap, int fd) = 0;

	void accounting_add_fd(callbackmap_t& cbmap, int fd, callbackfunc_t todo, const funcparam_t& para) {
	  CallBack cb;
	  cb.d_callback = todo;
	  cb.d_parameter = para;
	  memset(&cb.d_ttd, 0, sizeof(cb.d_ttd));

	  //if(cbmap.count(fd)) throw FDMultiplexerError("Tried to add fd "+std::to_string(fd)+ " to multiplexer twice");
	  cbmap[fd] = cb;
	}

	void accounting_remove_fd(callbackmap_t& cbmap, int fd) {
	  if(!cbmap.erase(fd)) 
	  throw FDMultiplexerError("Tried to remove unlisted fd "+std::to_string(fd)+ " from multiplexer");
	}
};

