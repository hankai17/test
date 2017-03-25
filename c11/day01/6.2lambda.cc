/*************************************************************************
	> File Name: 05lamdba.cpp
	> Author: guaz
	> Mail: hankai17@126.com 
	> Created Time: 2017-02-04 22:39:39
 ************************************************************************/

#include<iostream>

int main()
{
    int id = 0;
    auto f = [id] () mutable {  //拷贝成一个对象，且这个对象可变（可引用） 
        std::cout<<"id: "<<id<<std::endl;
        ++id;//ok
    };
    id = 42;
    f(); //0
    f(); //1
    f(); //2
    std::cout<<"id: "<<id<<std::endl; //42
    return 0;
}
int main3()
{
    int x = 0;
    int y = 42;
    auto qqq = [x,&y] {    //copy,ref   //immediatly
    //auto qqq = [=,&y] {    //copy,ref
        std::cout<<"x: "<<x<<std::endl;    
        std::cout<<"y: "<<y<<std::endl;
        //++x; this is error
        ++y;
    };
    x = y = 77;
    qqq(); // 0 77
    qqq(); // 0 78
    std::cout<<"final y: "<<y<<std::endl; //79

    return 0;
}

int main2()
{
    auto l = [](const std::string & s){
        std::cout<<s<<std::endl;
    };
    l("hello lamdba");
    return 0;
}

int main1()
{
    auto l = []{
        std::cout<<"hello lamdba"<<std::endl;
    };
    l();
    return 0;
}

