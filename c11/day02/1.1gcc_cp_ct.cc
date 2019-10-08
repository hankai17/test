#include<iostream>
using namespace std;

int g_construct_count = 0;
int g_copy_construct_count = 0;
int g_destruct_count = 0;

struct A {
  A() { cout<<"construct: "<<++g_construct_count<<endl; }
  A(const A& a) { cout<<"copy construct: "<<++g_copy_construct_count<<endl; }
  ~A() { cout<<"destruct: "<<++g_destruct_count<<endl; }
};

A get_A() { return A(); }

A get_A_foo() { //有区别? 跟2.2好像没有区别啊
  A a;
  return a;
}

int main() {
  A a = get_A();
  //A&& a = get_A();
  return 0;
}

/*
   -fno-elide-constructors 关闭优化
construct: 1
copy construct: 1  //拷贝构造成匿名
destruct: 1
copy construct: 2 //拷贝构造
destruct: 2
destruct: 3
*/

/*
如果直接g++ -std=c++11的话 构造与析构都是一次 没有拷贝
*/

/*
17line用右值引用来接
g++ 1.1gcc_cp_ct.cc -std=c++11 -fno-elide-constructors
./a.out 
construct: 1
copy construct: 1
destruct: 1
destruct: 2
直接匿名转有名 没有第二个拷贝
*/
