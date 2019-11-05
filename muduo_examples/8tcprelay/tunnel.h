#ifndef MUDUO_EXAMPLES_SOCKS4A_TUNNEL_H
#define MUDUO_EXAMPLES_SOCKS4A_TUNNEL_H

#include "muduo/base/Logging.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"
#include "muduo/net/TcpClient.h"
#include "muduo/net/TcpServer.h"

class Tunnel : public std::enable_shared_from_this<Tunnel>,
               muduo::noncopyable
{
 public:
  Tunnel(muduo::net::EventLoop* loop,
         const muduo::net::InetAddress& serverAddr,
         const muduo::net::TcpConnectionPtr& serverConn)
    : client_(loop, serverAddr, serverConn->name()),
      serverConn_(serverConn)
  {
    LOG_INFO << "Tunnel " << serverConn->peerAddress().toIpPort()
             << " <-> " << serverAddr.toIpPort();
  }

  ~Tunnel()
  {
    LOG_INFO << "~Tunnel";
  }

  void setup()
  {
    using std::placeholders::_1;
    using std::placeholders::_2;
    using std::placeholders::_3;

    client_.setConnectionCallback(
        std::bind(&Tunnel::onClientConnection, shared_from_this(), _1));
    client_.setMessageCallback(
        std::bind(&Tunnel::onClientMessage, shared_from_this(), _1, _2, _3));
    serverConn_->setHighWaterMarkCallback(
        std::bind(&Tunnel::onHighWaterMarkWeak,
                  std::weak_ptr<Tunnel>(shared_from_this()), kServer, _1, _2),
        1024*1024);
  }

  void connect()
  {
    client_.connect();
  }

  void disconnect()
  {
    client_.disconnect();
    // serverConn_.reset();
  }

 private:
  void teardown()
  {
    client_.setConnectionCallback(muduo::net::defaultConnectionCallback);
    client_.setMessageCallback(muduo::net::defaultMessageCallback);
    if (serverConn_)
    {
      serverConn_->setContext(boost::any());
      serverConn_->shutdown();
    }
    clientConn_.reset();
  }

  void onClientConnection(const muduo::net::TcpConnectionPtr& conn) //2连接源站成功
  {
    using std::placeholders::_1;
    using std::placeholders::_2;

    LOG_DEBUG << (conn->connected() ? "UP" : "DOWN");
    if (conn->connected())
    {
      conn->setTcpNoDelay(true);
      conn->setHighWaterMarkCallback(
          std::bind(&Tunnel::onHighWaterMarkWeak,
                    std::weak_ptr<Tunnel>(shared_from_this()), kClient, _1, _2),
          1024*1024);
      serverConn_->setContext(conn); //关联vc<--->osvc
      serverConn_->startRead(); //监控读   //即连接os成功后才监听读 所以即使你猛的推过来1G 在我没有连接到os之前是不会读这1G内容的  ------>感觉合理吗?  如果作为中继合理 但是作为一个缓存就不合理了
      clientConn_ = conn;
      if (serverConn_->inputBuffer()->readableBytes() > 0)
      {
        conn->send(serverConn_->inputBuffer()); //如果vc有数据则发数据 如果累积了很多 只发出一部分 则监听写 回调仍是这个回调
      }
    }
    else
    {
      teardown();
    }
  }

  void onClientMessage(const muduo::net::TcpConnectionPtr& conn,
                       muduo::net::Buffer* buf,
                       muduo::Timestamp)
  {
    LOG_DEBUG << conn->name() << " " << buf->readableBytes();
    if (serverConn_)
    {
      serverConn_->send(buf); //收到数据发给vc //收到洪水
    }
    else
    {
      buf->retrieveAll();
      abort();
    }
  }

  enum ServerClient
  {
    kServer, kClient
  };

  void onHighWaterMark(ServerClient which,
                       const muduo::net::TcpConnectionPtr& conn,
                       size_t bytesToSent)
  {
    using std::placeholders::_1;

    LOG_INFO << (which == kServer ? "server" : "client")
             << " onHighWaterMark " << conn->name()
             << " bytes " << bytesToSent;

    if (which == kServer) //vc
    {
      if (serverConn_->outputBuffer()->readableBytes() > 0)
      {
        clientConn_->stopRead(); //如果os发的太猛 就先停止监听读
        LOG_INFO << "clientConn stop Read os  "<<serverConn_->outputBuffer()->readableBytes();
        serverConn_->setWriteCompleteCallback(
            std::bind(&Tunnel::onWriteCompleteWeak,
                      std::weak_ptr<Tunnel>(shared_from_this()), kServer, _1));
      }
    }
    else //osvc
    {
      if (clientConn_->outputBuffer()->readableBytes() > 0)
      {
        serverConn_->stopRead();
        clientConn_->setWriteCompleteCallback(
            std::bind(&Tunnel::onWriteCompleteWeak,
                      std::weak_ptr<Tunnel>(shared_from_this()), kClient, _1));
      }
    }
  }

  static void onHighWaterMarkWeak(const std::weak_ptr<Tunnel>& wkTunnel, //Highwater调用时机是103line: vc write时调highwater
                                  ServerClient which,
                                  const muduo::net::TcpConnectionPtr& conn,
                                  size_t bytesToSent)
  {
    std::shared_ptr<Tunnel> tunnel = wkTunnel.lock();
    if (tunnel)
    {
      tunnel->onHighWaterMark(which, conn, bytesToSent);
    }
  }

  void onWriteComplete(ServerClient which, const muduo::net::TcpConnectionPtr& conn)
  {
    LOG_INFO << (which == kServer ? "server" : "client")
             << " onWriteComplete " << conn->name();
    if (which == kServer)
    {
      clientConn_->startRead();
      serverConn_->setWriteCompleteCallback(muduo::net::WriteCompleteCallback());
    }
    else
    {
      serverConn_->startRead();
      clientConn_->setWriteCompleteCallback(muduo::net::WriteCompleteCallback());
    }
  }

  static void onWriteCompleteWeak(const std::weak_ptr<Tunnel>& wkTunnel,
                                  ServerClient which,
                                  const muduo::net::TcpConnectionPtr& conn)
  {
    std::shared_ptr<Tunnel> tunnel = wkTunnel.lock();
    if (tunnel)
    {
      tunnel->onWriteComplete(which, conn);
    }
  }

 private:
  muduo::net::TcpClient client_;
  muduo::net::TcpConnectionPtr serverConn_;
  muduo::net::TcpConnectionPtr clientConn_;
};
typedef std::shared_ptr<Tunnel> TunnelPtr;

#endif  // MUDUO_EXAMPLES_SOCKS4A_TUNNEL_H
