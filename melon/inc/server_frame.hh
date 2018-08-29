#pragma once
#include <boost/utility.hpp> 
#include "namespaces.hh"
#include "multiplex.hh"
#include "server.hh"
#include "socket.hh"
#include "async_server.hh"
#include "miobuffer.hh"

/*
再加一层让s与复用蹭火花
*/
class AsyncServerFrame {
  public:
	AsyncServerFrame(FDMultiplexer* fdm, const string& address, int port) {
	  d_server = new AsyncServer(address, port);
      d_fdm = fdm;
      d_fdm->lfd = (d_server->d_server_socket).get_handle();
    }
	void go(); //即注册serve_connection
	void serve_connection(Socket* socket);
	void serve_connection1(Socket* socket);

  public:
	FDMultiplexer* d_fdm;
    Server* d_server;
};

