#include "muduo/base/Logging.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/TcpServer.h"
#include <stdio.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

void onHighWaterMark(const TcpConnectionPtr& conn, size_t len) {
  LOG_INFO << "HighWaterMark " << len;
}

const int kBufSize = 64*1024;
const char* g_file = NULL;

void onConnection(const TcpConnectionPtr& conn) {
  LOG_INFO << "FileServer - " << conn->peerAddress().toIpPort() << " -> "
           << conn->localAddress().toIpPort() << " is "
           << (conn->connected() ? "UP" : "DOWN");
  if (conn->connected()) {
    LOG_INFO << "FileServer - Sending file " << g_file
             << " to " << conn->peerAddress().toIpPort();
    conn->setHighWaterMarkCallback(onHighWaterMark, kBufSize + 1);

    FILE* fp = ::fopen(g_file, "rb");
    if (fp) {
      conn->setContext(fp);
      char buf[kBufSize];
      size_t nread = ::fread(buf, 1, sizeof buf, fp); //流水线 读64K
      conn->send(buf, static_cast<int>(nread));
    } else {
      conn->shutdown();
      LOG_INFO << "FileServer - no such file";
    }
  } else {
    if (!conn->getContext().empty()) {
      FILE* fp = boost::any_cast<FILE*>(conn->getContext());
      if (fp) {
        ::fclose(fp);
      }
    }
  }
}

void onWriteComplete(const TcpConnectionPtr& conn) {        //第一次的回调只调onConnection 第n次的回调都是通过调onWriteComplete完成 ---------->这一步应该gdb一下
  FILE* fp = boost::any_cast<FILE*>(conn->getContext());    //文件读完主动关闭连接  这一点跟我想的不一样 应该读取文件cl 根据cl与发送了多少数据进行判断是否关闭链接
  char buf[kBufSize];                                       //什么时候主动关闭 自己心里应该有b数 不能被动的等flag
  size_t nread = ::fread(buf, 1, sizeof buf, fp); //-------------------------------------------------------------------------???
  if (nread > 0) {
    conn->send(buf, static_cast<int>(nread));
  } else {
    ::fclose(fp);
    fp = NULL;
    conn->setContext(fp);
    conn->shutdown();
    LOG_INFO << "FileServer - done";
  }
}

int main(int argc, char* argv[]) {
  LOG_INFO << "pid = " << getpid();
  if (argc > 1) {
    g_file = argv[1];

    EventLoop loop;
    InetAddress listenAddr(2021);
    TcpServer server(&loop, listenAddr, "FileServer");
    server.setConnectionCallback(onConnection);
    server.setWriteCompleteCallback(onWriteComplete);
    server.start();
    loop.loop();
  } else {
    fprintf(stderr, "Usage: %s file_for_downloading\n", argv[0]);
  }
}

