#pragma once
#include <boost/utility.hpp> 
#include "namespaces.hh"
#include "multiplex.hh"
#include "server.hh"
#include "socket.hh"
#include "mtasker.hh"
#include "global.hh"
#include "event.hh"

extern MTasker<RWEvent>* MT;
extern FDMultiplexer* t_fdm;

class AsyncServer : public Server { //开闭原则 用开闭原则是为了用继承多态 这里只用了继承
  public:
	friend void async_server_new_connection_MT(void* p);
	using new_connection_cb_t = std::function< void(Socket*)>;

	AsyncServer(const string& address, int port) : Server(address, port) {};
	void new_connection(); //起协程
	void async_wait_for_connections(FDMultiplexer* fdm, const new_connection_cb_t& callback); //注册回调并挂树上 //依赖

  public:
	new_connection_cb_t d_async_new_connection_cb;
};

/*
server基本功能 起端口 拿链接
web服务器基本功能 即go函数 死循环拿一条链接起一个线程

async服务器基本功能 死循环拿一条链接起一个线程? 
	异步的话 肯定用到epoll 所以有注册函数即链接到来的处理函数

*/

