#include "async_server.hh"
#include <string>
#include"mtasker.hh"
#include"multiplex.hh"
#include <iostream>

void async_server_new_connection_MT(void* p) {
  AsyncServer* server = (AsyncServer*)p;
  try {
	Socket* socket = server->accept(); //拿一条连接
	server->d_async_new_connection_cb(socket); //开始回调
	//if (socket != nullptr) delete socket;
  } catch (NetWorkError &e) {
	// we're running in a shared process/thread, so can't just terminate/abort
	return;
  }
}

void AsyncServer::new_connection() {
  cout<<"cb spin up cont"<<endl;
  MT->makeThread(&async_server_new_connection_MT, this);
  cout<<"end cb spin up cont"<<endl;
}
/*
void AsyncServer::process_accept() {
  //MT->makeThread(&async_server_new_connection_MT, this);
  //已经挂树上了 epoll_wait一旦跑起来 就会回调这个函数
  //感觉一样啊 
}
void AsyncServer::async_process_accept() {
  fdm->add_read_fd(d_server_socket.get_handle(), boost::bind(&AsyncServer::process_accept, this));
}
*/

void AsyncServer::async_wait_for_connections(FDMultiplexer* fdm, const new_connection_cb_t& cb) {
  d_async_new_connection_cb = cb;
  fdm->add_read_fd(d_server_socket.get_handle(), boost::bind(&AsyncServer::new_connection, this));
}

