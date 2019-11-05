#include "examples/hub/codec.h"

#include "muduo/base/Logging.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/TcpServer.h"

#include <map>
#include <set>
#include <stdio.h>

using namespace muduo;
using namespace muduo::net;

namespace pubsub
{

typedef std::set<string> ConnectionSubscription;

class Topic : public muduo::copyable
{
 public:
  Topic(const string& topic)
    : topic_(topic)
  {
  }

  void add(const TcpConnectionPtr& conn)
  {
    audiences_.insert(conn);
    if (lastPubTime_.valid())
    {
      conn->send(makeMessage());
    }
  }

  void remove(const TcpConnectionPtr& conn)
  {
    audiences_.erase(conn);
  }

  void publish(const string& content, Timestamp time)
  {
    content_ = content;
    lastPubTime_ = time;
    string message = makeMessage();
    for (std::set<TcpConnectionPtr>::iterator it = audiences_.begin();
         it != audiences_.end();
         ++it)
    {
      (*it)->send(message);
    }
  }

 private:

  string makeMessage()
  {
    return "pub " + topic_ + "\r\n" + content_ + "\r\n";
  }

  string topic_;
  string content_;
  Timestamp lastPubTime_;
  std::set<TcpConnectionPtr> audiences_;
};

class PubSubServer : noncopyable
{
 public:
  PubSubServer(muduo::net::EventLoop* loop,
               const muduo::net::InetAddress& listenAddr)
    : loop_(loop),
      server_(loop, listenAddr, "PubSubServer")
  {
    server_.setConnectionCallback(
        std::bind(&PubSubServer::onConnection, this, _1));
    server_.setMessageCallback(
        std::bind(&PubSubServer::onMessage, this, _1, _2, _3));
    loop_->runEvery(1.0, std::bind(&PubSubServer::timePublish, this));
  }

  void start()
  {
    server_.start();
  }

 private:
  void onConnection(const TcpConnectionPtr& conn)
  {
    if (conn->connected())
    {
      conn->setContext(ConnectionSubscription());
    }
    else //交互中 收到某个c的fin  则把该链接的所有topic都去掉
    {
      const ConnectionSubscription& connSub
        = boost::any_cast<const ConnectionSubscription&>(conn->getContext());
      // subtle: doUnsubscribe will erase *it, so increase before calling.
      for (ConnectionSubscription::const_iterator it = connSub.begin();
           it != connSub.end();)
      {
        doUnsubscribe(conn, *it++);
      }
    }
  }

  void onMessage(const TcpConnectionPtr& conn,
                 Buffer* buf,
                 Timestamp receiveTime)
  {
    ParseResult result = kSuccess;
    while (result == kSuccess)
    {
      string cmd;
      string topic; //0
      string content;
      result = parseMessage(buf, &cmd, &topic, &content); //1server收到数据后解析
      if (result == kSuccess)
      {
        if (cmd == "pub") //2某个c发布topic 如果server有此topic则用没有则新建 每次收到c发布的消息后都会重置该topic内容  然后同步的向该topic的每隔订阅者发送
        {
          doPublish(conn->name(), topic, content, receiveTime);
        }
        else if (cmd == "sub")
        {
          LOG_INFO << conn->name() << " subscribes " << topic;
          doSubscribe(conn, topic); //3把该练级放到 相应topic的订阅者中
        }
        else if (cmd == "unsub")
        {
          doUnsubscribe(conn, topic); //4...
        }
        else
        {
          conn->shutdown();
          result = kError;
        }
      }
      else if (result == kError)
      {
        conn->shutdown();
      }
    }
  }

  void timePublish()
  {
    Timestamp now = Timestamp::now();
    doPublish("internal", "utc_time", now.toFormattedString(), now);
  }

  void doSubscribe(const TcpConnectionPtr& conn,
                   const string& topic)
  {
    ConnectionSubscription* connSub
      = boost::any_cast<ConnectionSubscription>(conn->getMutableContext());

    connSub->insert(topic);
    getTopic(topic).add(conn);
  }

  void doUnsubscribe(const TcpConnectionPtr& conn,
                     const string& topic)
  {
    LOG_INFO << conn->name() << " unsubscribes " << topic;
    getTopic(topic).remove(conn);
    // topic could be the one to be destroyed, so don't use it after erasing.
    ConnectionSubscription* connSub
      = boost::any_cast<ConnectionSubscription>(conn->getMutableContext());
    connSub->erase(topic);
  }

  void doPublish(const string& source,
                 const string& topic,
                 const string& content,
                 Timestamp time)
  {
    getTopic(topic).publish(content, time);
  }

  Topic& getTopic(const string& topic)
  {
    std::map<string, Topic>::iterator it = topics_.find(topic);
    if (it == topics_.end())
    {
      it = topics_.insert(make_pair(topic, Topic(topic))).first;
    }
    return it->second;
  }

  EventLoop* loop_;
  TcpServer server_;
  std::map<string, Topic> topics_;
};

}  // namespace pubsub

int main(int argc, char* argv[])
{
  if (argc > 1)
  {
    uint16_t port = static_cast<uint16_t>(atoi(argv[1]));
    EventLoop loop;
    if (argc > 2)
    {
      //int inspectPort = atoi(argv[2]);
    }
    pubsub::PubSubServer server(&loop, InetAddress(port));
    server.start();
    loop.loop();
  }
  else
  {
    printf("Usage: %s pubsub_port [inspect_port]\n", argv[0]);
  }
}

