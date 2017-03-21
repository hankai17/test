#include"spider_ip.hh"
#include"tb.hh"
//#include"ipcn.hh"
#include"ip138.hh"
#include<thread>
#include<vector>

//std::vector<std::thread> threads;

void DoBussiness(spider_ip* spider)
{
    spider->make_spider();
    delete spider;
}

void test()
{
    //threads.push_back(std::thread(DoBussiness(new tb_spider_ip("1.0.8.0","1.0.15.255"))));
    //threads.push_back(std::thread(DoBussiness(new ipcn_spider_ip("1.0.8.0","1.0.15.255"))));
    //threads.push_back(std::thread(DoBussiness(new ip138_spider_ip("1.0.8.0","1.0.15.255"))));
    //DoBussiness(new tb_spider_ip("100.0.0.0","101.0.0.0"));  //淘宝的ip库不准确
    //DoBussiness(new ipcn_spider_ip("1.0.8.0","1.0.15.255"));
    DoBussiness(new ip138_spider_ip("100.0.0.0","160.0.0.0"));

/*
    spider_ip* s1 = new tb_spider_ip("1.0.8.0","1.0.15.255");
    std::thread t1(DoBussiness,s1);

    spider_ip* s2 = new ipcn_spider_ip("1.0.8.0","1.0.15.255");
    std::thread t2(DoBussiness,s2);

    spider_ip* s3 = new ip138_spider_ip("1.0.8.0","1.0.15.255");
    std::thread t3(DoBussiness,s3);

    t1.join();
    t2.join();
    t3.join();
*/
    //sleep(1);
}

int main()
{
    test();
    return 0;
}

