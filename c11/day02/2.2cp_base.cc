#include<iostream>

class A
{
    public:
        A() :m_ptr(new int(0))
        {
            std::cout<<"construct"<<std::endl;
        }
        A(const A& a) :m_ptr(new int(*a.m_ptr))
        {
            std::cout<<"cp construct"<<std::endl;
        }
        ~A()
        {
            std::cout<<"destruct"<<std::endl;
            delete m_ptr;
        }
    private:
        int* m_ptr;
};

A Get(bool flag)
{
    A a;
    A b;
    if(flag)
        return a;
    else
        return b;
}

int main()
{
    A a = Get(false);
    return 0;
}

/*
[root@90-20 day02]# g++ 2.2cp_base.cc -fno-elide-constructors
[root@90-20 day02]# ./a.out 
construct
construct
cp construct
destruct
destruct
cp construct
destruct
destruct
*/

/*
construct
construct
cp construct
destruct
destruct
destruct
打开优化 有一个拷贝 毫无疑问是拷贝b元素为匿名对象  但是这个拷贝可能有损性能 所以怎样避免拷贝成匿名对象，而直接返回临时变量b本身呢？
*/

/*
 * 只要返回一个元素 不管这个元素是匿名还是有名都会发生一次拷贝动作
 * 重点是: 只要用右值引用来接 则第二次拷贝动作会省去
 * */
