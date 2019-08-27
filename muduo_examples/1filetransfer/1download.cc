#include "muduo/base/Logging.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/TcpServer.h"
#include <stdio.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

const char* g_file = NULL;

string readFile(const char* filename) { //FIXME: use FileUtil::readFile()
  string content;
  FILE* fp = ::fopen(filename, "rb");
  if (fp) { // inefficient!!!
    const int kBufSize = 1024*1024;
    char iobuf[kBufSize];
    ::setbuffer(fp, iobuf, sizeof iobuf);

    char buf[kBufSize];
    size_t nread = 0;
    while ( (nread = ::fread(buf, 1, sizeof buf, fp)) > 0) {
      content.append(buf, nread);
    }
    ::fclose(fp);
  }
  return content;
}

void onHighWaterMark(const TcpConnectionPtr& conn, size_t len) {
  LOG_INFO << "HighWaterMark " << len;
}

void onConnection(const TcpConnectionPtr& conn) {
  LOG_INFO << "FileServer - " << conn->peerAddress().toIpPort() << " -> "
           << conn->localAddress().toIpPort() << " is "
           << (conn->connected() ? "UP" : "DOWN");
  if (conn->connected()) {
    LOG_INFO << "FileServer - Sending file " << g_file
             << " to " << conn->peerAddress().toIpPort();
    conn->setHighWaterMarkCallback(onHighWaterMark, 64*1024);
    string fileContent = readFile(g_file); //读取整个文件
    conn->send(fileContent);
    conn->shutdown(); //调shutdown(SHUT_WR)  发fin  主动关闭 只是不写了仍能读
    LOG_INFO << "FileServer - done";
  }
}

int main(int argc, char* argv[]) {
  LOG_INFO << "pid = " << getpid();
  if (argc > 1) {
    g_file = argv[1];

    EventLoop loop;
    InetAddress listenAddr(2021);
    TcpServer server(&loop, listenAddr, "FileServer"); //不再用依赖那种了
    server.setConnectionCallback(onConnection);
    server.start();
    loop.loop();
  } else {
    fprintf(stderr, "Usage: %s file_for_downloading\n", argv[0]);
  }
}

//https://blog.csdn.net/qq_14821541/article/details/52425875
//对端已经close     (我也收到了fin 收不收的到无所谓) 但是我硬是再向对端发数据/或是在途字节刚刚到达对端 我会收到rst 如果我收到了rst再发数据才会sigpipe我死掉
//               sigpipe产生前提条件是我底层必须收到了rst
//对端已经shutdown  (我也收到了fin 收不收的到无所谓) 但是我硬是再向对端发数据/或是在途字节刚刚到达对端 我bu会收到rst
