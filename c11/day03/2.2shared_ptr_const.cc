#include <memory>
#include <iostream>

class Base
{
public:
    virtual ~Base() {}
};

class Derived : public Base
{
public:
    ~Derived() {}
};

void processSpBase(const std::shared_ptr<Base> &p)
{
    std::cout << p.use_count() << std::endl;
}

int main()
{
    std::shared_ptr<Derived> p(new Derived);
    std::cout << p.use_count() << std::endl;
    processSpBase(p);

    return 0;
}
//https://blog.csdn.net/imred/article/details/88729688
