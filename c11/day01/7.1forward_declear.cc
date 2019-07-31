
a.h中依赖b    
# include "B.h" 
class A 
{ 
    A(void); 
    ~A(void);     
    B b_;  //要包含B.h 
    
b.h中依赖a  不能写include "A.h"
class A;
class B 
{ 
    B(void); 
    ~B(void); 
    void fun(A& a)//只能是指针或引用 
    { 
    } 
    //前向声明的类不能实例化对象 
    A* a_;  // 
};

1 前向声明的类不能定义对象
2 可以用于定义指向这个类型的指针和引用
3 用于申明使用该类型作为形参或返回类型的函数
    
