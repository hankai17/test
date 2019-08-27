#include "muduo/base/Logging.h"
#include "muduo/net/Endian.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"
#include "muduo/net/TcpClient.h"
#include <utility>
#include <stdio.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

class TimeClient : noncopyable {
 public:
  TimeClient(EventLoop* loop, const InetAddress& serverAddr)
    : loop_(loop), client_(loop, serverAddr, "TimeClient") {
    client_.setConnectionCallback(std::bind(&TimeClient::onConnection, this, _1));
    client_.setMessageCallback(std::bind(&TimeClient::onMessage, this, _1, _2, _3));
    // client_.enableRetry();
  }
  void connect() { client_.connect(); }

 private:
  EventLoop* loop_;
  TcpClient client_;

  void onConnection(const TcpConnectionPtr& conn) { //链接建立回调 //accept回调?
    LOG_INFO << conn->localAddress().toIpPort() << " -> "
             << conn->peerAddress().toIpPort() << " is "
             << (conn->connected() ? "UP" : "DOWN");

    if (!conn->connected()) {
      loop_->quit();
    }
  }

  void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp receiveTime) { //正常读写回调
    if (buf->readableBytes() >= sizeof(int32_t)) {
      const void* data = buf->peek();
      int32_t be32 = *static_cast<const int32_t*>(data);
      buf->retrieve(sizeof(int32_t));
      time_t time = sockets::networkToHost32(be32);
      Timestamp ts(implicit_cast<uint64_t>(time) * Timestamp::kMicroSecondsPerSecond);
      LOG_INFO << "Server time = " << time << ", " << ts.toFormattedString();
    } else { //一个字节write一次  same as ppc parser httphdr
      LOG_INFO << conn->name() << " no enough data " << buf->readableBytes()
               << " at " << receiveTime.toFormattedString();
    }
  }
};

int main(int argc, char* argv[]) {
  LOG_INFO << "pid = " << getpid();
  if (argc > 1) {
    EventLoop loop;
    InetAddress serverAddr(argv[1], 2037);

    TimeClient timeClient(&loop, serverAddr);
    timeClient.connect();
    loop.loop();
  } else {
    printf("Usage: %s host_ip\n", argv[0]);
  }
}

