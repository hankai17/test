#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <fcntl.h>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netdb.h>


#include <boost/utility.hpp>
#include <csignal>
#include "namespaces.hh"

int close_socket(int sock);
int make_IPv4_sockaddr(const std::string& str, struct sockaddr_in* ret/*out*/);
union ComboAddress {
  struct sockaddr_in sin4;
  struct sockaddr_in6 sin6;

  bool operator==(const ComboAddress& rhs) const {
	if(std::tie(sin4.sin_family, sin4.sin_port) != std::tie(rhs.sin4.sin_family, rhs.sin4.sin_port)) return false;
	if(sin4.sin_family == AF_INET) return sin4.sin_addr.s_addr == rhs.sin4.sin_addr.s_addr;
	else return memcmp(&sin6.sin6_addr.s6_addr, &rhs.sin6.sin6_addr.s6_addr, sizeof(sin6.sin6_addr.s6_addr)) == 0;
  }

  bool operator!=(const ComboAddress& rhs) const {
	return(!operator == (rhs));
  }

  bool operator<(const ComboAddress& rhs) const {
	if(sin4.sin_family == 0) {
	  return false;
	} 
	if(std::tie(sin4.sin_family, sin4.sin_port) < std::tie(rhs.sin4.sin_family, rhs.sin4.sin_port)) return true;
	if(std::tie(sin4.sin_family, sin4.sin_port) > std::tie(rhs.sin4.sin_family, rhs.sin4.sin_port)) return false;

	if(sin4.sin_family == AF_INET) return sin4.sin_addr.s_addr < rhs.sin4.sin_addr.s_addr;
	else return memcmp(&sin6.sin6_addr.s6_addr, &rhs.sin6.sin6_addr.s6_addr, sizeof(sin6.sin6_addr.s6_addr)) < 0;
  }

  bool operator>(const ComboAddress& rhs) const {
	return rhs.operator<(*this);
  }

  /*
  //TODO
  struct addressOnlyHash {
  uint32_t operator()(const ComboAddress& ca) const { 
  const unsigned char* start;
  int len;
  if(ca.sin4.sin_family == AF_INET) {
  start =(const unsigned char*)&ca.sin4.sin_addr.s_addr;
  len=4;
  } else {
  start =(const unsigned char*)&ca.sin6.sin6_addr.s6_addr;
  len=16;
  }
  return burtle(start, len, 0);
  }
  };
  */

  struct addressOnlyLessThan: public std::binary_function<ComboAddress, ComboAddress, bool> {
	bool operator()(const ComboAddress& a, const ComboAddress& b) const {
	  if(a.sin4.sin_family < b.sin4.sin_family) return true;
	  if(a.sin4.sin_family > b.sin4.sin_family) return false;
	  if(a.sin4.sin_family == AF_INET) return a.sin4.sin_addr.s_addr < b.sin4.sin_addr.s_addr;
	  else return memcmp(&a.sin6.sin6_addr.s6_addr, &b.sin6.sin6_addr.s6_addr, sizeof(a.sin6.sin6_addr.s6_addr)) < 0;
	}
  };

  struct addressOnlyEqual: public std::binary_function<ComboAddress, ComboAddress, bool> {
	bool operator()(const ComboAddress& a, const ComboAddress& b) const {
	  if(a.sin4.sin_family != b.sin4.sin_family) return false;
	  if(a.sin4.sin_family == AF_INET) return a.sin4.sin_addr.s_addr == b.sin4.sin_addr.s_addr;
	  else return !memcmp(&a.sin6.sin6_addr.s6_addr, &b.sin6.sin6_addr.s6_addr, sizeof(a.sin6.sin6_addr.s6_addr));
	}
  };

  using socklen_t = size_t;
  socklen_t get_sock_len() const {
	if(sin4.sin_family == AF_INET) return sizeof(sin4);
	else return sizeof(sin6);
  }

  ComboAddress() {
	sin4.sin_family = AF_INET;
	sin4.sin_addr.s_addr = 0;
	sin4.sin_port = 0;
  }

  void set_sock_addr(const struct sockaddr* sa, socklen_t slen) {
	if (slen > sizeof(struct sockaddr_in6)) cout<<"set_sock_addr err"<<endl;
	memcpy(this, sa, slen);
  }

  ComboAddress(const struct sockaddr* sa, socklen_t slen) {
	set_sock_addr(sa, slen);
  };

  ComboAddress(const struct sockaddr_in6* sa) {
	set_sock_addr((const struct sockaddr*)sa, sizeof(struct sockaddr_in6));
  };

  ComboAddress(const struct sockaddr_in* sa) {
	set_sock_addr((const struct sockaddr*)sa, sizeof(struct sockaddr_in));
  };

  explicit ComboAddress(const string& str, uint16_t port = 0) {
	memset(&sin6, 0, sizeof(sin6));
	sin4.sin_family = AF_INET;
	sin4.sin_port = 0;
	if(make_IPv4_sockaddr(str, &sin4)) {
	  sin6.sin6_family = AF_INET6;
	  //if(make_IPv6_sockaddr(str, &sin6) < 0)
	  //TODO
	}
	if(!sin4.sin_port)
	sin4.sin_port = htons(port);
  }

  bool is_IPv6() const {
	return sin4.sin_family == AF_INET6;
  }

  bool is_IPv4() const {
	return sin4.sin_family == AF_INET;
  }

  bool isMappedIPv4()  const { return true;} //TODO

  //ComboAddress mapToIPv4() const {} //TODO

  string to_string() const {
	char host[1024];
	if(sin4.sin_family && !getnameinfo((struct sockaddr*)this, get_sock_len(), host, sizeof(host), 0, 0, NI_NUMERICHOST)) return host;
	else return "invalid";
  }

  string to_string_with_port() const {
	if(sin4.sin_family == AF_INET) return to_string() + ":" + std::to_string(ntohs(sin4.sin_port)); else
	return "["+to_string() + "]:" + std::to_string(ntohs(sin4.sin_port));
  }
  void truncate(unsigned int bits) noexcept; //TODO
};

class NetWorkError : public runtime_error {
  public:
	NetWorkError(const string& why = "Network Error") : runtime_error(why.c_str()) {}
	NetWorkError(const char* why = "Network Error") : runtime_error(why) {}
};

using protocol_type = int;

class Socket : public boost::noncopyable {
  public:
	Socket(int fd);
	Socket(int pt_family, int type, protocol_type pt = 0);
	~Socket();
	Socket* accept();
	bool get_remote(ComboAddress& remote);

	//bool acl(NetmaskGroup& ng) {} //TODO

	void set_non_blocking();
	bool set_blocking();
	void set_reuse_addr();
	void bind(const ComboAddress& local);
	/*
	   void connect(const ComboAddress& ep, int timeout = 0) {
	   if(::connect(d_socket, (struct sockaddr*)&ep, ep.get_sock_len()) < 0) {
	   if(errno == EINPROGRESS) {
	   if (timeout > 0) {
	   int res = wait_for_rwdata(d_socket, false, timeout, 0);
	   if (res == 0) {
	   throw NetWorkError("timeout while connecting to "+ep.toStringWithPort());
	   } else if (res < 0) {
	   throw NetWorkError("while waiting to connect to "+ep.toStringWithPort()+": "+string(strerror(errno)));
	   }
	   }
	   }
	   else {
	   throw NetWorkError("While connecting to "+ep.toStringWithPort()+": "+string(strerror(errno)));
	   }
	   }
	   }
	   */
	///root/tmp/source_code/socketlib/src
	void recv_from(string& data_gram, ComboAddress& ep);
	bool recv_from_async(string& data_gram, ComboAddress& ep);
	void send(const std::string& msg);
	void send_to(const char* msg, size_t len, const ComboAddress& ep);
	void send_to(const string& data_gram, const ComboAddress& ep);
	void writen(const string& data);
	size_t try_write(const char* ptr, size_t to_write);
	size_t write(const char* ptr, size_t to_write);
	int wait_for_rwdata(int fd, bool wait_for_read, int seconds, int useconds, bool* error = nullptr, bool* disconnected = nullptr);
	void writen_with_timeout(const void* buffer, size_t n, int timeout);
	int get_char();
	void get_line(string& data);
	void read(string& data);
	size_t read(char* buffer, size_t bytes);
	ssize_t read_with_timeout(char* buffer, size_t n, int timeout);
	void listen(unsigned int len = 10);
	int get_handle() const;

  private:
	char *d_buffer;
	int d_socket;
	size_t d_buflen;
};

template <typename Container>
void stringtok (Container& container, string const& in, const char * const delimiters = " \t\n") {
  const string::size_type len = in.length();
  string::size_type i = 0;
  while (i < len) {
	// eat leading whitespace
	i = in.find_first_not_of (delimiters, i);
	if (i == string::npos) //全部匹配
	return;   // nothing left but white space

	// find the end of the token
	string::size_type j = in.find_first_of (delimiters, i);

	// push token
	if (j == string::npos) {
	  container.push_back (in.substr(i));
	  return;
	} else
	container.push_back (in.substr(i, j-i));

	// set up for next loop
	i = j + 1;
  }
}
