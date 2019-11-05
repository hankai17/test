#include "examples/socks4a/tunnel.h"

#include <malloc.h>
#include <stdio.h>
#include <sys/resource.h>
#include <unistd.h>
#include <iostream>

using namespace muduo;
using namespace muduo::net;

EventLoop* g_eventLoop;
InetAddress* g_serverAddr;
std::map<string, TunnelPtr> g_tunnels;

void onServerConnection(const TcpConnectionPtr& conn)
{
  LOG_DEBUG << (conn->connected() ? "UP" : "DOWN");
  if (conn->connected())
  {
    conn->setTcpNoDelay(true);
    conn->stopRead();
    TunnelPtr tunnel(new Tunnel(g_eventLoop, *g_serverAddr, conn)); //1本机拿到一条连接起一个tunnel
    tunnel->setup(); //设置tunnel的回调
    tunnel->connect(); //向一个固定的目的ip发起连接
    g_tunnels[conn->name()] = tunnel; //即每收到一个连接都起一个相应的连接连接源站
  }
  else //c端收到fin
  {
    assert(g_tunnels.find(conn->name()) != g_tunnels.end());
    g_tunnels[conn->name()]->disconnect(); //主动关闭osvc
    g_tunnels.erase(conn->name());
  }
}

void onServerMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp) //2
{
  LOG_DEBUG << buf->readableBytes();
  std::cout<<"------------------>buf->readablebytes: "<<buf->readableBytes()<<std::endl;
  if (!conn->getContext().empty())
  {
    const TcpConnectionPtr& clientConn
      = boost::any_cast<const TcpConnectionPtr&>(conn->getContext());
    clientConn->send(buf); //收到数据发给os
  }
}

//如果突然读到c端的fin 看写的chat文档吧

void memstat()
{
  malloc_stats();
}

int main(int argc, char* argv[])
{
  if (argc < 4)
  {
    fprintf(stderr, "Usage: %s <host_ip所有数据包都要发往这儿> <port> <portlisten_port本机port>\n", argv[0]); //把包发往指定的ip port
  }
  else
  {
    LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
    {
      // set max virtual memory to 256MB.
      size_t kOneMB = 1024*1024;
      rlimit rl = { 256*kOneMB, 256*kOneMB };
      setrlimit(RLIMIT_AS, &rl);
    }
    const char* ip = argv[1];
    uint16_t port = static_cast<uint16_t>(atoi(argv[2]));
    InetAddress serverAddr(ip, port);
    g_serverAddr = &serverAddr;

    uint16_t acceptPort = static_cast<uint16_t>(atoi(argv[3]));
    InetAddress listenAddr(acceptPort);

    EventLoop loop;
    g_eventLoop = &loop;
    loop.runEvery(3, memstat);

    TcpServer server(&loop, listenAddr, "TcpRelay");

    server.setConnectionCallback(onServerConnection);
    server.setMessageCallback(onServerMessage);

    server.start();

    loop.loop();
  }
}

/*
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
include_directories("/home/file/muduo-master/")

add_executable(tcprelay tcprelay.cc tunnel.h)
target_link_libraries(tcprelay /home/file/build/release-install-cpp11/lib/libmuduo_net.a /home/file/build/release-install-cpp11/lib/libmuduo_base.a pthread)

*/
