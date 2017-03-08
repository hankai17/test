/*************************************************************************
	> File Name: 0context.cpp
	> Author: guaz
	> Mail: hankai17@126.com 
	> Created Time: 2017-02-09 17:06:55
 ************************************************************************/

#include <iostream>
#include <array>
#include <functional>
#include <boost/context/all.hpp>

class Coroutine {
    public:
    Coroutine() :
        my_context(boost::context::detail::make_fcontext(
            stack.data() + stack.size(),
            stack.size(),
            Coroutine::dispatch
        ))
    {}
    virtual ~Coroutine() {}

    void operator()() {
        boost::context::detail::jump_fcontext(&yield_context, my_context, reinterpret_cast<intptr_t>(this));
    }

    protected:
    void yield() {
        boost::context::detail::jump_fcontext(my_context, &yield_context, 0);
    }

    virtual void call() = 0;

    private:
    static void dispatch(intptr_t coroutine_ptr) {
        Coroutine *coroutine = reinterpret_cast<Coroutine *>(coroutine_ptr);
        coroutine->call();
        while (true) coroutine->yield();
    }

    private:
    boost::context::detail::fcontext_t *my_context;
    boost::context::detail::fcontext_t yield_context;
    std::array<intptr_t, 64*1024> stack;
};

struct A : public Coroutine {
    void call() {
        std::cout << "A went to the store one day.\n";
        yield();
        std::cout << "A was looking for groceries.\n";
        yield();
        std::cout << "A finally found what she was looking for.\n";
    }
};

struct B : public Coroutine {
    void call() {
        std::cout << "B went to the store one day.\n";
        yield();
        std::cout << "B was looking for replacement tires.\n";
        yield();
        std::cout << "B didn't find anything at all.\n";
        yield();
        std::cout << "B went to another store.\n";
        yield();
        std::cout << "B got the tires installed there.\n";
    }
};

struct C : public Coroutine {
    void call() {
        std::cout << "C went to the store one day.\n";
        yield();
        std::cout << "C was looking for USB drives.\n";
        yield();
        std::cout << "C found several with competitive pricing.\n";
        yield();
        std::cout << "C couldn't decide which to buy, so gave up.\n";
    }
};


int main() {
    std::cout << "So, this is what happened.\n";
    A a;
    B b;
    C c;
    for (size_t i=0; i<10; ++i) {
        a();
        b();
        c();
    }
    std::cout << "Then it all was done.\n";
}
