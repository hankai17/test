#include "my_sylar/log.hh"
#include "my_sylar/uri.hh"
#include "my_sylar/iomanager.hh"
#include "my_sylar/address.hh"
#include "my_sylar/socket.hh"
#include "my_sylar/util.hh"
#include "my_sylar/ns/ares.hh"
#include "my_sylar/http/http_connection.hh"

#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/time.h>
#include<arpa/inet.h>
#include<netinet/tcp.h>

#include <vector>
#include <fstream>
#include <stdlib.h>

#define closesocket close

sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();
sylar::AresChannel::ptr channel = nullptr;
std::vector<std::string> g_reqs;
int g_ideal_reqs = 10;
int g_current_reqs = 0;
int g_cur_req_pos = 0;

int g_sum_flow = 0;
int g_sum_flow_old = 0;
int g_sum_reqs = 0;

void read_log() {
    std::ifstream in("1.log");
    std::string str;
    int icount = 0;
    if (in.is_open()) {
        while(!in.eof()) {
    	    icount++;
            getline(in, str);
            g_reqs.push_back(std::string(str));
            //SYLAR_LOG_DEBUG(g_logger) << str;
        }
    }
    g_sum_reqs = icount;
    SYLAR_LOG_DEBUG(g_logger) << "sum line: " << icount;
}

void test_req(const std::string& req) {
    //std::string req = "http://61.157.86.132/2Q2W13064ADC539B89979D27DC6B6998AFCCBA300219_0_124389480BC9F0F0B232A5E6D4A12BBAA5222F20_0/119.84.42.2/dl.steam.ksyna.com/depot/1030831/chunk/6f8ebcf14d3374ca32b8c27eab94f2090352d6fe";
    
    sylar::Uri::ptr uri = sylar::Uri::Create(req.c_str());
    if (uri == nullptr) {
        //SYLAR_LOG_DEBUG(g_logger) << "Can not parse req: " << req;
        g_current_reqs--;
        return;
    }
    //SYLAR_LOG_DEBUG(g_logger) << "req start: " << req;

    sylar::http::HttpResult::ptr resu =  sylar::http::HttpConnection::DoGet(uri, 1000 * 5);
    //SYLAR_LOG_DEBUG(g_logger) << resu->toString();
    //SYLAR_LOG_DEBUG(g_logger) << resu->response->getHeader("Content-Length", "null");
    if (resu == nullptr || resu->response == nullptr) {
        //SYLAR_LOG_DEBUG(g_logger) << "Cannot get result may be timeout " << req;
        g_current_reqs--;
        return;
    }
    std::string str_flow = resu->response->getHeader("Content-Length", "null");
    if (str_flow != "null") {
        int flow = atoi(str_flow.c_str());;
        g_sum_flow += flow;
    }
    g_current_reqs--;
    //SYLAR_LOG_DEBUG(g_logger) << "req end: " << req;

    return;
}

void static_flow() {
    while (1) {
        sleep(5);
        SYLAR_LOG_DEBUG(g_logger) << "rate: " <<
        (g_sum_flow - g_sum_flow_old) / 1024 / 1024 / 5 << "MB/s" 
        << " concurr_reqs: " << g_current_reqs
        << " cur_req_pos: " << g_cur_req_pos
        << " sum_reqs: " << g_sum_reqs;
        g_sum_flow_old = g_sum_flow;
    }
}

void do_task() {
    sleep(5);
    while (1) {
        if (g_current_reqs < g_ideal_reqs) {
           if (g_cur_req_pos == (int)g_reqs.size()) {
               break;
           }
           std::string req = g_reqs[g_cur_req_pos];
           g_current_reqs++;
           g_cur_req_pos++;
           sylar::IOManager::GetThis()->schedule(std::bind(test_req, req));
           continue;
        }
        usleep(1000 * 10);
    }
}

void test1() {
    sylar::IPAddress::ptr addr = sylar::Address::LookupAnyIPAddress("www.baidu.com:80");
    if (addr) {
        SYLAR_LOG_DEBUG(g_logger) << "addr: " << addr->toString();
    } else {
        SYLAR_LOG_DEBUG(g_logger) << "get addr fail";
        return;;
    }
    sylar::Socket::ptr sock = sylar::Socket::CreateTCP(addr);
    if (!sock->connect(addr)) {
        SYLAR_LOG_DEBUG(g_logger) << "connect faild addr: " << addr->toString();
        return;
    } else {
        SYLAR_LOG_DEBUG(g_logger) << "connect succeed: " << addr->toString();
    }

    uint64_t ts = sylar::GetCurrentUs();
    for (size_t i = 0; i < 10000000000ul; i++) {
        if (int err = sock->getError()) {
            SYLAR_LOG_DEBUG(g_logger) << "err: " << err
            << " strerror: " << strerror(errno);
            break;
        }

        struct tcp_info tcp_info;
        if(!sock->getOption(IPPROTO_TCP, TCP_INFO, tcp_info)) {
            SYLAR_LOG_INFO(g_logger) << "err";
            break;
        }
        if(tcp_info.tcpi_state != TCP_ESTABLISHED) {
            SYLAR_LOG_INFO(g_logger)
                    << " state=" << (int)tcp_info.tcpi_state;
            break;
        }
        static int batch = 10000000;
        if(i && (i % batch) == 0) {
            uint64_t ts2 = sylar::GetCurrentUs();
            SYLAR_LOG_INFO(g_logger) << "i=" << i << " used: " << ((ts2 - ts) * 1.0 / batch) << " us";
            ts = ts2;
        }
    }
}

void test3() {
    //sylar::IPAddress::ptr addr = sylar::Address::LookupAnyIPAddress("www.baidu.com:80");
    sylar::IPAddress::ptr addr = sylar::IPAddress::Create("61.135.169.121", 80);
    if (addr) {
        SYLAR_LOG_DEBUG(g_logger) << "addr: " << addr->toString();
    } else {
        SYLAR_LOG_DEBUG(g_logger) << "get addr fail";
        return;;
    }
    sylar::Socket::ptr sock = sylar::Socket::CreateTCP(addr);
    if (!sock->connect(addr)) {
        SYLAR_LOG_DEBUG(g_logger) << "connect faild addr: " << addr->toString();
        return;
    } else {
        SYLAR_LOG_DEBUG(g_logger) << "connect succeed: " << addr->toString();
    }

    //uint64_t ts = sylar::GetCurrentUs();
}

void test1_1(sylar::Socket::ptr sock) {
    std::string buf;
    buf.resize(1024);

    sock->recv(&buf[0], buf.size());
    SYLAR_LOG_DEBUG(g_logger) << "end test1_1";
}

void test2() {
    sylar::IPAddress::ptr addr = nullptr;
    if (0) {
        addr = sylar::Address::LookupAnyIPAddress("www.baidu.com:80"); // 不知道为什么加上以后就变成阻塞的了
    } else {
        std::string domain("www.ifeng.com");
        if (channel == nullptr) {
            SYLAR_LOG_DEBUG(g_logger) << "channel is nullptr sleep 5s...";
            sleep(5);
        }
        auto ips = channel->aresGethostbyname(domain.c_str());
        for (auto& i : ips) {
            SYLAR_LOG_DEBUG(g_logger) << i.toString();
        }
        SYLAR_LOG_DEBUG(g_logger) << "domain: " << domain << ", test done";

        addr = std::dynamic_pointer_cast<sylar::IPAddress>(
              sylar::Address::Create(ips[0].getAddr(), ips[0].getAddrLen())
              );
    }

    if (addr) {
        SYLAR_LOG_DEBUG(g_logger) << "addr: " << addr->toString();
    } else {
        SYLAR_LOG_DEBUG(g_logger) << "get addr fail";
        return;;
    }
    sylar::Socket::ptr sock = sylar::Socket::CreateTCP(addr);
    if (!sock->connect(addr)) {
        SYLAR_LOG_DEBUG(g_logger) << "fd: " << sock->getSocket() << " connect faild addr: " << addr->toString();
        return;
    } else {
        SYLAR_LOG_DEBUG(g_logger) << "fd: " << sock->getSocket() << " connect succeed: " << addr->toString();
    }

    sylar::IOManager::GetThis()->schedule(std::bind(test1_1, sock));
    SYLAR_LOG_DEBUG(g_logger) << "after schedule...";

    sleep(10);
    sock->close(); //这里手动close 底层会触发test_1中的读事件 (so dangerous!)
    SYLAR_LOG_DEBUG(g_logger) << "after sock->close: " << addr->toString();
}

void ares_test() {
    SYLAR_LOG_DEBUG(g_logger) << "in ares test";
    channel.reset(new sylar::AresChannel);
    channel->init();
    channel->start();
}

int main() {
    sylar::IOManager iom(1, false, "io");
    iom.schedule(read_log);
    iom.schedule(do_task);
    iom.schedule(static_flow);
    //iom.schedule(ares_test);
    //iom.schedule(test3);
    iom.stop();
    return 0;
}

//cat 1.log | xargs -n1 -I % sh -c '{ nohup curl -v -o /dev/null %; sleep 0.01; }'
//ss -o state established '( dport = :80 )' dst 61.157.86.132
