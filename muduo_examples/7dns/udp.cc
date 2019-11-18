#include "muduo/base/Logging.h"
#include "muduo/net/Channel.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/Socket.h"
#include "muduo/net/SocketsOps.h"
#include "muduo/net/TcpClient.h"
#include "muduo/net/TcpServer.h"

#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace muduo;
using namespace muduo::net;

const size_t frameLen = 3*sizeof(int64_t);
static int64_t id = 0;
static int pingpongs = 10;

/*
struct Entry : public muduo::copyable {
    explicit Entry(const WeakTcpConnectionPtr& weakConn)
        : weakConn_(weakConn) {} //shared_ptr ---> weakptr

    ~Entry() {
        muduo::net::TcpConnectionPtr conn = weakConn_.lock();
        if (conn) {
            conn->shutdown();
        }
    }
    WeakTcpConnectionPtr weakConn_;
};
typedef std::shared_ptr<Entry> EntryPtr;
typedef std::weak_ptr<Entry> WeakEntryPtr;
typedef std::unordered_set<EntryPtr> Bucket;
typedef boost::circular_buffer<Bucket> WeakConnectionList;

WeakConnectionList connectionBuckets_;
*/

int createNonblockingUDP()
{
    int sockfd = ::socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_UDP);
    if (sockfd < 0)
    {
        LOG_SYSFATAL << "::socket";
    }
    return sockfd;
}

/////////////////////////////// Server ///////////////////////////////

void serverReadCallback(int sockfd, muduo::Timestamp receiveTime)
{
    unsigned char messages[600] = {0};
    struct sockaddr peerAddr;
    memZero(&peerAddr, sizeof peerAddr);
    socklen_t addrLen = sizeof peerAddr;
    ssize_t nr = ::recvfrom(sockfd, messages, sizeof messages, 0, &peerAddr, &addrLen);

    char addrStr[64];
    sockets::toIpPort(addrStr, sizeof addrStr, &peerAddr);
    LOG_DEBUG << "received " << nr << " bytes from " << addrStr;

    if (nr < 0)
    {
        LOG_SYSERR << "::recvfrom";
    }
    else if (implicit_cast<size_t>(nr) > 0)
    {
        ssize_t nw = ::sendto(sockfd, messages, sizeof messages, 0, &peerAddr, addrLen);
        if (nw < 0)
        {
            LOG_SYSERR << "::sendto";
        }
        else if (implicit_cast<size_t>(nw) > 0)
        {
            LOG_INFO << "Wrote to client " << nw << " bytes.";
        }
    }
}

void runServer(std::vector<uint16_t> ports)
{
    EventLoop loop;

    for(int i = 0; i < ports.size(); i++) {
        Socket* s =  new Socket(createNonblockingUDP());
        s->bindAddress(InetAddress(ports[i]));
        Channel* ch = new Channel(&loop, s->fd());
        ch->setReadCallback(std::bind(&serverReadCallback, s->fd(), _1));
        ch->enableReading();
    }
    loop.loop();
}

/////////////////////////////// Client ///////////////////////////////

void clientReadCallback(int sockfd, muduo::Timestamp receiveTime)
{
    unsigned char message[600];
    ssize_t nr = sockets::read(sockfd, message, sizeof message);

    if (nr < 0)
    {
        LOG_SYSERR << "::read";
    }
    else if (implicit_cast<size_t>(nr) > 0)
    {
        LOG_INFO << "id: " << id++ << " client read: " << implicit_cast<size_t>(nr);
    }
}

void sendMyTime(int sockfd)
{
    char messages[600] = {0};

    //
    strcpy(messages, (char*)"hello world");

    for(int i = 0; i < pingpongs; i++) {
        ssize_t nw = sockets::write(sockfd, messages, sizeof messages);
        if (nw < 0)
        {
            LOG_SYSERR << "::write";
        }
        else if (implicit_cast<size_t>(nw) > 0)
        {
            LOG_INFO << "index: " << i << " wrote " << nw << " bytes.";
        }
    }
}

void statics_message(int sockfd, EventLoop* loop) {
    LOG_INFO << "statics id: " << id;
    /*
    if(close(sockfd) < 0) {
        LOG_INFO << "close err: " << strerror(errno);
    }
    */ //because it close in quit()
    loop->quit();
}

//send 100 packages. timeouts 1s
void runClient(const char* ip, uint16_t port)
{
    Socket sock(createNonblockingUDP());
    InetAddress serverAddr(ip, port);
    int ret = sockets::connect(sock.fd(), serverAddr.getSockAddr());
    if (ret < 0)
    {
        LOG_SYSFATAL << "::connect";
    }
    EventLoop loop;
    Channel channel(&loop, sock.fd());
    channel.setReadCallback(std::bind(&clientReadCallback, sock.fd(), _1)); //typedef std::function<void(Timestamp)> ReadEventCallback;
    channel.enableReading();
    loop.runAfter(0.0, std::bind(sendMyTime, sock.fd())); //Ϊʲô_1 //typedef std::function<void()> TimerCallback;
    loop.runAfter(1.0, std::bind(statics_message, sock.fd(), &loop));
    loop.loop();
}

int main(int argc, char* argv[])
{
    if (argc > 2 && strstr(argv[1], "s") != NULL)
    {
        std::vector<uint16_t> ports;
        for (int i = 2; i < argc; i++) {
            ports.push_back(static_cast<uint16_t>(atoi(argv[i])));
        }

        runServer(ports);

    }
    else if (argc == 3)
    {
        runClient(argv[1], static_cast<uint16_t>(atoi(argv[2]))); //ip, port
    }
    else
    {
        printf("Usage:\n%s -s port\n%s ip port\n", argv[0], argv[0]);
    }
}
