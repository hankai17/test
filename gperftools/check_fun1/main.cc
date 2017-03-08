#include "conf.hh"
#include<iostream>
#include<gperftools/profiler.h>

dns_conf conf;

int main()
{
    ProfilerStart("dns_conf");
    conf.open_conf_file();
    conf.parse_black_conf();
    conf.parse_white_conf();
    conf.close_conf_file();
    char tmp[] = "www.jd.com";    
    for(int i=0; i<5000000; i++)
    {
        conf.check(tmp,strlen(tmp));
    }
    ProfilerStop();
    return 0;
}

