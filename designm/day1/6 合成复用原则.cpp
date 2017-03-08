#define  _CRT_SECURE_NO_WARNINGS 
#include <iostream>
using namespace std;
//继承和组合都能完成功能添加  就用组合
//组合有关联 和 依赖 依赖更好一些耦合度更小
class Cat
{
    public:
    void sleep() 
    {
        cout << " 小猫睡觉了" << endl;
    }
};
//向给猫添加一个功能， 创建一个新的猫 既能够睡觉，又能吃东西
//通过继承的方式完成
//如果使用继承 子类和其所有父类的耦合度非常高了 并且继承了大量的东西
//一旦父类的函数出现问题 则子类的函数有可能出问题
class AdvCat :public Cat
{
    public:
    void eatAndSleep() 
    {
        cout << "吃东西" << endl;
        sleep();
    }
};
//使用组合的方式来添加小猫的吃东西方法
//使用组合的方式，降低了AdvCat2 和Cat的耦合度， 跟Cat的父类没有任何关系，
//只跟Cat的sleep方法有关系  只要父类的sleep不出问题就可以
class AdvCat2
{
    public:
    AdvCat2(Cat *cat)// 依赖
    {
        this->cat = cat;
    }
    void eatAndSleep() 
    {
        cout << "吃东西" << endl;
        cat->sleep();
    }
    private:
    Cat *cat;//关联
};
int main(void)
{
    Cat c;
    c.sleep();

    AdvCat ac;
    ac.eatAndSleep();

    cout << "----- " << endl;

    AdvCat2 ac2(&c);
    ac2.eatAndSleep();
    return 0;
}
