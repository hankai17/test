#ifndef MUDUO_EXAMPLES_SIMPLE_DISCARD_DISCARD_H
#define MUDUO_EXAMPLES_SIMPLE_DISCARD_DISCARD_H

#include "muduo/net/TcpServer.h"

class DiscardServer { //RFC863
 public:
  DiscardServer(muduo::net::EventLoop* loop, const muduo::net::InetAddress& listenAddr);
  void start(); //Impl

 private:
  void onConnection(const muduo::net::TcpConnectionPtr& conn);
  void onMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp time);

  muduo::net::TcpServer server_; //Impl
};

#endif

