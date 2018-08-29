#pragma once
#include <map>
#include <string>
#include <list>
#include <boost/utility.hpp>
#include "namespaces.hh"
#include "socket.hh"
#include "server.hh"

/*
   webserver基本功能
   死循环 拿到一条链接起一个线程
   */
class WebServer : public boost::noncopyable {
  public:
	WebServer(const string& local_addr, int port);
	void bind();
	void go();
	void server_connect(Socket* client);
	virtual Server* create_server() {
	  return new Server(d_listenaddress, d_port);
	}
	string d_listenaddress;
	int d_port;
	string d_password;
	Server* d_server; //里面有被动套接字 以及获取连接的接口accept
};

