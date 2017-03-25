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
        A(A&& a) :m_ptr(a.m_ptr)
        {
            std::cout<<"move construct"<<std::endl;
            a.m_ptr = nullptr;
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
construct
construct
move construct
destruct
destruct
destruct
*/

/*
[root@90-20 day02]# g++ 2.3move_base.cc -fno-elide-constructors
[root@90-20 day02]# ./a.out 
construct
construct
move construct
destruct
destruct
move construct
destruct
destruct
[root@90-20 day02]# 
*/
