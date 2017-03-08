/*************************************************************************
  > File Name: 1.cpp
  > Author: guaz
  > Mail: hankai17@126.com 
  > Created Time: 2017-02-22 19:57:55
 ************************************************************************/

#include<iostream>
//#include<google/profiler.h>
#include<gperftools/profiler.h>
using namespace std;

void testfunc()
{
    int sum = 0;
    for(int i =0; i < 700000000; ++i)
    {
        sum +=i;
    }
    std::cout<<"sum:"<<sum<<endl;

}
int main()
{
    ProfilerStart("first.prof");
    std::cout<<"hello profiler"<<endl;
    testfunc();

    int s = 0;

    for(int i = 0; i < 300000000; ++i)
    {
        s +=i;
    }

    std::cout<<"int main s:"<<s<<endl;

    ProfilerStop();
    return 0;
}
