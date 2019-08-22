#pragma once
#include <map>
#include <string>
#include <list>
#include <boost/utility.hpp>
#include "namespaces.hh"
#include "socket.hh"

/*
   起端口 
   拿一条链接
   */
class Server {
  public:
	Server(const string& local_addr, int port) : d_local(local_addr.empty() ? "0.0.0.0" : local_addr, port),
	  d_server_socket(d_local.sin4.sin_family, SOCK_STREAM, 0) { //关联类方法堆积
	  d_server_socket.set_reuse_addr();
	  d_server_socket.bind(d_local);
	  d_server_socket.listen();
	}
	ComboAddress d_local;
	Socket* accept() {
	  return d_server_socket.accept(); //用关联就是为了用关联类中的方法
	}
  //protected:
	Socket d_server_socket; //关联socket
};

