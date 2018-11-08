/*************************************************************************
	> File Name: 1.cpp
	> Author: Bill
	> Mail: hankai17@126.com 
	> Created Time: 2017-01-16 13:55:49
 ************************************************************************/

// gdb non-stop mode demo
// build instruction: g++ -g -o nonstop nonstop.cpp -lboost_thread

#include <iostream>
#include <boost/thread/thread.hpp>

struct op
{
        op(int id): m_id(id) {}

        void operator()()
        {
                std::cout << m_id << " begin" << std::endl;
                std::cout << m_id << " end" << std::endl;
        }

        int m_id;
};

int main(int argc, char ** argv)
{
        boost::thread t1(op(1)), t2(op(2)), t3(op(3));
        t1.join(); t2.join(); t3.join();
        return 0;
}

