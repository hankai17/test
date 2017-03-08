#define  _CRT_SECURE_NO_WARNINGS 
#include <iostream>
using namespace std;
//------抽象层------
class Car
{
    public:
    virtual void run() = 0;
};

class Driver 
{
    public:
    virtual void drive(Car *car) = 0;
};

void travel(Driver *d, Car *c)
{
    d->drive(c);
}

//实现层-----
class Zhang3 :public Driver
{
    public:
    virtual void drive(Car *car)
    {
        cout << "涨3 开车了" << endl;
        car->run();
    }
};

class Li4 :public Driver
{
    public:
    virtual void drive(Car *car)
    {
        cout << "Li4 开车了" << endl;
        car->run();
    }
};

class Benz :public Car
{
    public:
    virtual void run() 
    {
        cout << "benz 启动了" << endl;
    }
};

class BMW :public Car
{
    public:
    virtual void run() 
    {
        cout << "BMW 启动了" << endl;
    }
};

#if 0
class Benz
{
    public:
    void run() {
        cout << "奔驰启动了" << endl;
    }
};
class BMW
{
    public:
    void run() {
        cout << "宝马启动了" << endl;
    }
};
class Zhang3
{
    public:
    void driveBenz(Benz *b) {
        b->run();
    }
    void driveBMW(BMW *b)
    {
        b->run();
    }
};
#endif
//业务
int main(void)
{
    #if 0
    //张三去开奔驰
    Benz  *benz = new Benz;
    Zhang3 *z3 = new Zhang3;
    z3->driveBenz(benz);
    //张三去开宝马
    BMW * bmw = new BMW;
    z3->driveBMW(bmw);
    #endif
    //张三去开奔驰
    Driver* zang3 = new Zhang3;
    Car * benz = new Benz;
    zang3->drive(benz);

    //li4 区开宝马
    Driver *li4 = new Li4;
    Car * bmw = new BMW;
    li4->drive(bmw);
    return 0;
}
