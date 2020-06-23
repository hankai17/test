PIMPL(Pointer to Implementation)这个手法可以解決/改善C++编码时常碰到的2大问题。
1.class增加private/protected成员时，使用此class的相关 .cpp(s) 需要重新编译。
2.定义冲突与跨平台编译

假设我们有一个A.h(class A)，並且有A/B/C/D 4個.cpp引用他 如果A class增加了private/protected成员，A/B/C/D .cpp全部都要重新编译。
make发现A.h比A/B/C/D .cpp4个文件新时，就会通知compiler重新编译他们，
就算你的C++ compiler非常聪明，知道B/C/D文件只能存取A class public成员，make还是要通知compiler起来检查。三个文件也许还好，那五十个，一百个呢？

//a.h  优化后的代码
#ifndef A_H
#define A_H
#include <memory>
class A
{
public:
    A();
    ~A();
     
    void doSomething();
     
private:    
      struct Impl; // 前置声明
      std::auto_ptr<impl> m_impl; // 在这里我们把原本的private成员封裝到struct A::Impl里，用一个不透明的指针(m_impl)指向他
};
#endif

//a.cpp
#include <stdio.h>
#include "a.h"

/////////////  A private数据成员和成员函数全部被封裝到struct A::Impl里  如此一来无论private成员如何改变都只会重新编译A.cpp，而不会影响B/C/D.cpp
struct A::Impl {
    int m_count;
    Impl();
    ~Impl();
    void doPrivateThing();
};  
A::Impl::Impl(): m_count(0){}
A::Impl::~Impl(){}           
void A::Impl::doPrivateThing() {
    printf("count = %d\n", ++m_count);
}    
 
 ////////////
A::A():m_impl(new Impl){}      
A::~A(){} 
 
void A::doSomething(){
    m_impl->doPrivateThing();    
}

Refer: https://blog.csdn.net/lihao21/article/details/47610309
Refer: java13种设计模式 https://www.cnblogs.com/boshen-hzb/p/10246144.html

