#include<iostream>
#include<log.hh>

int main()
{
    int i = 0;
    for(i = 0; i < 100; i++)
    {
        std::cout<<"i="<<i<<std::endl;
        //Log(__FILE__, __LINE__,Level[1],0,"i");
        Log(__FILE__, __LINE__,Level[2],0,"i= %d",i);
    }
    return 0;
}
//g++ main.cc log.cc -o test -I./

