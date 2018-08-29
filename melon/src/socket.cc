#include"socket.hh"

//提取ip port初始化sockaddr_in并传出
int make_IPv4_sockaddr(const std::string& str, struct sockaddr_in* ret/*out*/) {
  if(str.empty()) return -1;
  struct in_addr inp;
  string::size_type pos = str.find(':');
  if(pos == string::npos) {
    if(inet_aton(str.c_str(), &inp)) {
      ret->sin_addr.s_addr = inp.s_addr;
      return 0;
    }
    return -1;
  }
  if(!*(str.c_str() + pos + 1)) return -1;
  char* e_ptr = (char*)str.c_str() + str.size();
  int port = strtol(str.c_str() + pos + 1, &e_ptr, 10);
  if (port < 0 || port > 65535) return -1;
  if(*e_ptr) return -1;

  ret->sin_port = htons(port);
  if(inet_aton(str.substr(0, pos).c_str(), &inp)) {
    ret->sin_addr.s_addr=inp.s_addr;
    return 0;
  }
  return -1;
}

int close_socket(int sock) {
  int ret = ::close(sock);
  if(ret < 0 && errno == ECONNRESET) return 0;
  if(ret < 0) //throw PDNSException("Error closing sock: "+stringerror());
  cout<<"close socket"<<endl;
  //TODO
  return ret;
}

bool set_close_on_exec(int sock) {
  int flags = fcntl(sock, F_GETFD, 0);
  if(flags<0 || fcntl(sock, F_SETFD, flags|FD_CLOEXEC) <0) return false;
  return true;
}

Socket::Socket(int fd) {
  d_socket = fd;
  d_buflen = 4096;
  d_buffer = new char[d_buflen];
}

Socket::Socket(int pt_family, int type, protocol_type pt) { //协议族 流/报 0
  if( (d_socket = socket(pt_family, type, pt)) < 0 ) throw NetWorkError(strerror(errno));
  set_close_on_exec(d_socket);
  d_buflen = 4096;
  d_buffer = new char[d_buflen];
}

Socket::~Socket() {
  try {
	close_socket(d_socket);
  } catch(...) {}
  delete[] d_buffer;
}

bool Socket::set_blocking() {
  int ret = fcntl(d_socket, F_GETFL, 0);
  if(ret < 0 || fcntl(d_socket, F_SETFL, ret&(~O_NONBLOCK)) < 0) {
	throw NetWorkError("Setting socket to blocking: "+string(strerror(errno)));
	return false;
  }
  return true;
}

void Socket::set_non_blocking() {
  int ret = fcntl(d_socket, F_GETFL, 0);
  if(ret < 0 || fcntl(d_socket, F_SETFL, ret|O_NONBLOCK) < 0)
  throw NetWorkError("Setting socket to nonblocking: "+string(strerror(errno)));
}

void Socket::set_reuse_addr() {
  int opt;
  if (setsockopt(d_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, static_cast<unsigned>(sizeof opt)) < 0)
  throw NetWorkError(string("Setsockopt failed: ")+strerror(errno));
}

void Socket::bind(const ComboAddress& local) {
  int opt = 1;
  if(setsockopt(d_socket, SOL_SOCKET, SO_REUSEADDR,(char*)&opt, sizeof opt) < 0 )
  throw NetWorkError(string("Setsockopt failed: ")+strerror(errno));

  if(::bind(d_socket, (struct sockaddr *)&local, local.get_sock_len()) < 0)
  throw NetWorkError("While binding: "+string(strerror(errno)));
}

Socket* Socket::accept() {
  struct sockaddr_in remote;
  socklen_t re_len = sizeof(remote);
  bzero(&remote, re_len);
  int s = ::accept(d_socket, (sockaddr *)&remote, &re_len);
  if(s < 0) {
	if(errno == EAGAIN) { //非阻塞 没有连接到来
	  throw NetWorkError("Accepting a connection: "+string(strerror(errno)));
	  return 0;
	}
  }
  return new Socket(s);
}

bool Socket::get_remote(ComboAddress& remote) {
  socklen_t re_len = sizeof(remote);
  return (getpeername(d_socket, (struct sockaddr *)&remote, &re_len) >= 0);
}

//bool acl(NetmaskGroup& ng) {} //TODO

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

void Socket::recv_from(string& data_gram, ComboAddress& ep) {
  socklen_t re_len = sizeof(ep);
  ssize_t bytes;
  if( (bytes = recvfrom(d_socket, d_buffer, d_buflen, 0, (sockaddr *)&ep , &re_len)) < 0)
  throw NetWorkError("After recvfrom: "+string(strerror(errno)));
  data_gram.assign(d_buffer, bytes);
}

bool Socket::recv_from_async(string& data_gram, ComboAddress& ep) {
  struct sockaddr_in remote;
  socklen_t re_len = sizeof(remote);
  ssize_t bytes;
  if( (bytes = recvfrom(d_socket, d_buffer, d_buflen, 0, (sockaddr *)&remote, &re_len)) < 0 ) {
	if(errno != EAGAIN) {
	  throw NetWorkError("After async recvfrom: "+string(strerror(errno)));
	} else {
	  return false;
	}
  }
  data_gram.assign(d_buffer, bytes);
  return true;
}

void Socket::send(const std::string& msg) {
  if(::send(d_socket, msg.c_str(), msg.size(), 0) < 0 )
  throw NetWorkError("After send: "+string(strerror(errno)));
}

void Socket::send_to(const char* msg, size_t len, const ComboAddress& ep) {
  if( sendto(d_socket, msg, len, 0, (sockaddr *)&ep, ep.get_sock_len()) < 0)
  throw NetWorkError("After sendto: "+string(strerror(errno)));
}

void Socket::send_to(const string& data_gram, const ComboAddress& ep) {
  send_to(data_gram.c_str(), data_gram.length(), ep);
}

void Socket::writen(const string& data) {
  if(data.empty()) return;

  size_t to_write = data.length();
  ssize_t res;
  const char* ptr = data.c_str();
  do {
	res = ::send(d_socket, ptr, to_write, 0);
	if(res < 0) throw NetWorkError("Writing to a socket: "+string(strerror(errno)));
	if(!res) throw NetWorkError("EOF on socket");
	to_write -= (size_t)res;
	ptr += (size_t)res;
  } while(to_write);
}

size_t Socket::try_write(const char* ptr, size_t to_write) {
  ssize_t res;
  res = ::send(d_socket, ptr, to_write, 0);
  if(res == 0) throw NetWorkError("EOF on writing to a socket");
  if(res > 0) return res;
  if(errno == EAGAIN) return 0;
  throw NetWorkError("Writing to a socket: "+string(strerror(errno)));
}

size_t Socket::write(const char* ptr, size_t to_write) {
  ssize_t res;
  res = ::send(d_socket, ptr, to_write, 0);
  if(res < 0) {
	throw NetWorkError("Writing to a socket: "+string(strerror(errno)));
  }
  return res;
}

int Socket::wait_for_rwdata(int fd, bool wait_for_read, int seconds, int useconds, bool* error, bool* disconnected) {
  int ret;
  struct pollfd pfd;
  memset(&pfd, 0, sizeof(pfd));
  pfd.fd = fd;

  if(wait_for_read) pfd.events = POLLIN;
  else pfd.events = POLLOUT;

  ret = poll(&pfd, 1, seconds * 1000 + useconds / 1000); //0 timeout, -1 err
  if (ret == -1) {
	errno = ETIMEDOUT;
  } else if (ret > 0) {
	if (error && (pfd.revents & POLLERR)) {
	  *error = true;
	}
	if (disconnected && (pfd.revents & POLLHUP)) {
	  *disconnected = true;
	}
  }
  return ret;
}

void Socket::writen_with_timeout(const void* buffer, size_t n, int timeout) {
  size_t bytes = n;
  const char* ptr = (char*)buffer;
  ssize_t ret;
  while(bytes) {
	ret = ::write(d_socket, ptr, bytes); //循环写
	if(ret < 0) {
	  if(errno == EAGAIN) { //缓冲区满
		ret = wait_for_rwdata(d_socket, false, timeout, 0);
		if(ret < 0) throw NetWorkError("Waiting for data write"); //timeout内仍无法发送
		if(!ret) throw NetWorkError("Timeout writing data");
		continue;
	  } else throw NetWorkError("Writing data: "+string(strerror(errno)));
	}
	if(!ret) {
	  throw NetWorkError("Did not fulfill TCP write due to EOF"); //?
	}
	ptr += (size_t) ret;
	bytes -= (size_t) ret;
  }
}

int Socket::get_char() {
  char c;
  ssize_t res = ::recv(d_socket, &c, 1, 0);
  if(res) return c;
  return -1;
}

void Socket::get_line(string& data) {
  data = "";
  int c;
  while((c = get_char()) != -1) {
	data += (char)c;
	if(c == '\n') break;
  }
}

void Socket::read(string& data) {
  ssize_t res = ::recv(d_socket, d_buffer, d_buflen, 0);
  if(res < 0) throw NetWorkError("Reading from a socket: "+string(strerror(errno)));
  data.assign(d_buffer, res);
}

//一般用这个最单纯的没有超时的read函数 因为调用这个函数之前可能调用了waitEvent 
//也就是说这个函数+waitEvent等于下一个函数
size_t Socket::read(char* buffer, size_t bytes) {
  ssize_t res = ::recv(d_socket, buffer, bytes, 0);
  if(res < 0) throw NetWorkError("Reading from a socket: "+string(strerror(errno)));
  return (size_t)res;
}

ssize_t Socket::read_with_timeout(char* buffer, size_t n, int timeout) {
  int err = wait_for_rwdata(d_socket, true, timeout, 0);
  if(err == 0) throw NetWorkError("timeout reading");
  if(err < 0) throw NetWorkError("nonblocking read failed: "+string(strerror(errno)));
  return read(buffer, n);
}

void Socket::listen(unsigned int len) {
  if(::listen(d_socket, len) < 0)
  throw NetWorkError("Setting socket to listen: "+string(strerror(errno)));
}

int Socket::get_handle() const {
  return d_socket;
}

