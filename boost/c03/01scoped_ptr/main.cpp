#include <stdexcept>
class foo_class {
public:
    char data[100000000];

    explicit foo_class(const char* /*param*/){}
};

bool some_function1(foo_class* param) {
    return !!param;
}

void some_function2(foo_class* /*param*/) {
    throw std::exception();
}

bool foo1() {  //引入话题，这样不好 如果some_function1() 或some_function2()抛出异常
               //就释放不了内存了
    foo_class* p = new foo_class("Some initialization data");

    bool something_else_happened = some_function1(p);
    if (something_else_happened) {
        delete p;
        return false;
    }

    some_function2(p);
    delete p;
    return true;
}

bool foo2() { //加上try 但看起来很冗余 , 可读性不高
    foo_class* p = new foo_class("Some initialization data");
    try {
        bool something_else_happened = some_function1(p);
        if (something_else_happened) {
            delete p;
            return false;
        }

       some_function2(p);
    } catch (...) {
        delete p;
        throw;
    }

    delete p;
    return true;
}


#include <boost/scoped_ptr.hpp>

bool foo3() { //一旦有异常，就自动清理内存 
    boost::scoped_ptr<foo_class> p(new foo_class("Some initialization data"));

    bool something_else_happened = some_function1(p.get()); //参数最好改成scoped_ptr的引用
    if (something_else_happened) {
       return false;
    }

    some_function2(p.get());
    return true;
}

int main() {
    try { foo2(); } catch(...){}
    try { foo3(); } catch(...){}

    return 0;
}

