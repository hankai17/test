#include "codec.h"
#include "muduo/base/Logging.h"
#include "muduo/base/Mutex.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/TcpServer.h"
#include <set>
#include <stdio.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

class ChatServer : noncopyable {
 public:
  ChatServer(EventLoop* loop, const InetAddress& listenAddr)
  : server_(loop, listenAddr, "ChatServer"),
    codec_(std::bind(&ChatServer::onStringMessage, this, _1, _2, _3)) { //给关联类注册本对象方法
    server_.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1)); //老生常谈 自己的
    server_.setMessageCallback(std::bind(&LengthHeaderCodec::onMessage, &codec_, _1, _2, _3)); //与以往不同 关联的
    //有点分层/面向对象的概念 即具体的解析交给对象来做 做好后回调上层
  }
  void start() { server_.start(); }

 private:
  void onConnection(const TcpConnectionPtr& conn) {
    LOG_INFO << conn->localAddress().toIpPort() << " -> "
             << conn->peerAddress().toIpPort() << " is "
             << (conn->connected() ? "UP" : "DOWN");

    if (conn->connected()) {
      connections_.insert(conn);
    } else {
      connections_.erase(conn);
    }
  }

  void onStringMessage(const TcpConnectionPtr&, const string& message, Timestamp) {
    for (ConnectionList::iterator it = connections_.begin(); it != connections_.end(); ++it) {
      codec_.send(get_pointer(*it), message);
    }
  }

  typedef std::set<TcpConnectionPtr> ConnectionList;
  TcpServer server_;
  LengthHeaderCodec codec_;
  ConnectionList connections_;
};

int main(int argc, char* argv[]) {
  LOG_INFO << "pid = " << getpid();
  if (argc > 1) {
    EventLoop loop;
    uint16_t port = static_cast<uint16_t>(atoi(argv[1]));
    InetAddress serverAddr(port);
    ChatServer server(&loop, serverAddr);
    server.start();
    loop.loop();
  } else {
    printf("Usage: %s port\n", argv[0]);
  }
}

