#include<string.h>
class foo_class {
public:
    char data[10000000];

    explicit foo_class(const char* param){
        memcpy(data,(char*)param,strlen(param)+1);
    }

};

foo_class* get_data() {
    static int i = 0;
    ++ i;
    if (i % 2) {
        return new foo_class("Just some string");
    } else {
        return 0;
    }
}


#include <boost/thread.hpp>
#include <boost/bind.hpp>

void process1(const foo_class* p);
void process2(const foo_class* p);
void process3(const foo_class* p);

void foo1() {
    while (foo_class* p = get_data()) // C way
    {
        std::cout<<p->data<<std::endl;
        // There will be too many threads soon, see
        // recipe 'Parallel execution of different tasks'
        // for a good way to avoid uncontrolled growth of threads 立即起了3个线程 但不知道什么时机释放

        boost::thread(boost::bind(&process1, p))
                .detach();
        boost::thread(boost::bind(&process2, p))
                .detach();
        boost::thread(boost::bind(&process3, p))
                .detach();
        // delete p; Oops!!!!
    }
}
/*如果用到配置文件更新 感觉用shared_ptr就麻烦了 如果用就这样用: 每来一个事务就拷贝构造一个shared_ptr<conf> 让全局conf引用++
当更新时 rest全局conf 从而实现替换   但如果事务很多会不会造成拷贝构造 变成瓶颈*/


#include <boost/shared_ptr.hpp>

void process_sp1(const boost::shared_ptr<foo_class>& p);
void process_sp2(const boost::shared_ptr<foo_class>& p);
void process_sp3(const boost::shared_ptr<foo_class>& p);

void foo2() { //shared_ptr has reference counter
    typedef boost::shared_ptr<foo_class> ptr_t;
    ptr_t p;
    while (p = ptr_t(get_data())) // C way 
    {
        std::cout<<p->data<<std::endl;
        boost::thread(boost::bind(&process_sp1, p)) //一切皆对象 函数对象中拷贝构造了shared_ptr对象 使shared_ptr对象引用计数++ 直到函数对象析构 shared_ptr对象才析构--
                .detach();
        boost::thread(boost::bind(&process_sp2, p))
                .detach();
        boost::thread(boost::bind(&process_sp3, p))
                .detach();
        // no need to anything
    }
}
/*
std::shared_ptr<Person> p1(new Person(1)); //p1的引用计数为1
std::shared_ptr<Person> p2 = std::make_shared<Person>(2); //p2的引用计数为1
注意两种构造区别  第一种调两次new 相当于脱裤子放屁 第二种简练
*/

#include <string>
#include <boost/smart_ptr/make_shared.hpp>

void process_str1(boost::shared_ptr<std::string> p);
void process_str2(const boost::shared_ptr<std::string>& p);

void foo3() {
    boost::shared_ptr<std::string> ps = boost::make_shared<std::string>(
        "Guess why make_shared<std::string> "
        "is faster than shared_ptr<std::string> "
        "ps(new std::string('this string'))"
    );

    boost::thread(boost::bind(&process_str1, ps))
            .detach();
    boost::thread(boost::bind(&process_str2, ps))
            .detach();
}

#include <boost/chrono/duration.hpp>
int main() {
    //foo1(); // Will cause a memory leak
    foo2();
    foo3();

    // Give all the threads a chance to finish
    // Note: It is an awfull design, but it is OK
    // for example
    boost::this_thread::sleep_for(boost::chrono::seconds(2));
    return 0;
}

#include <assert.h>


void process1(const foo_class* p) {
    assert(p);
}

void process2(const foo_class* p) {
    assert(p);
}

void process3(const foo_class* p) {
    assert(p);
}

void process_str1(boost::shared_ptr<std::string> p) {
    assert(p);
}

void process_str2(const boost::shared_ptr<std::string>& p) {
    assert(p);
}
void process_sp1(const boost::shared_ptr<foo_class>& p) {
    assert(!!p);
}

void process_sp2(const boost::shared_ptr<foo_class>& p) {
    assert(!!p);
}

void process_sp3(const boost::shared_ptr<foo_class>& p) {
    assert(!!p);
}

