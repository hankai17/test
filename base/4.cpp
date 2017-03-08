/*************************************************************************
	> File Name: 4.cpp
	> Author: Bill
	> Mail: XXXXXXX@qq.com 
	> Created Time: 2016-12-26 18:54:34
 ************************************************************************/

#include<iostream>
#include<boost/timer.hpp>

using namespace boost;

int main()
{
    timer t;
    std::cout<< "max"<<t.elapsed_max()/3600<<"h"<<std::endl;
    std::cout<< "min"<<t.elapsed_min()<<"s"<<std::endl;
    std::cout<< "now time elapsed:"<<t.elapsed()<<"s"<<std::endl;
    std::cout<<"hello world"<<std::endl;
    return 0;
}





















