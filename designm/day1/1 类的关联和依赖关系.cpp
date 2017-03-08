#define  _CRT_SECURE_NO_WARNINGS 
#include <iostream>
using namespace std;
class Car
{
    public:
    void run() 
    {
        cout << "Car 启动了" << endl;
    }
};
//如果说zhang3类有一个成员方法，将Car类当做一个对象指针形参来传递，并且并且并且zhang3没有car成员。
//就说zhang3类 依赖 于Car类张3 -----> Car
//这个车不是他的 可能是他老丈人的
class Zhang3
{
    public:
    void goWork(Car * car) 
    {
        car->run();
    }
    Car *fix() 
    {
    }
};

//li4类在gowork方法中也用到了另外一个Car的方法，但是LI4类还拥有Car的成员
//就说li4类 关联 Car类
//li4有自己的车  关联关系比依赖更紧密一些 用实线表示
class Li4
{
    public:
    void goWork() 
    {
        car.run();
    }
    private:
    Car car;
};
int main(void)
{
    return 0;
}
