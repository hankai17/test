#define  _CRT_SECURE_NO_WARNINGS 
#include <iostream>
using namespace std;
//抽象的策略（抽象的武器）
class AbstractStrategy
{
    public:
    //纯虚函数， 使用具体武器的策略,
    virtual void useWeapon() = 0;
};
class KnifeStrategy :public AbstractStrategy
{
    public:
    virtual void useWeapon() 
    {
        cout << "使用匕首，进行近战攻击" << endl;
    }
};
class AkStrategy :public AbstractStrategy
{
    public:
    virtual void  useWeapon() 
    {
        cout << "使用ak 进行远程攻击" << endl;
    }
};
class Hero
{
    public:
    Hero()
    {
        strategy = NULL;
    }
    void setStrategy(AbstractStrategy *strategy)
    {
        this->strategy = strategy;
    }
    //攻击方法
    void fight() 
    {
        cout << "英雄开始战斗了" << endl;
        this->strategy->useWeapon();
    }
    private:
    //拥有一个 使用攻击策略的抽象成员
    AbstractStrategy *strategy;
};
int main(void)
{
    AbstractStrategy *knife = new KnifeStrategy;
    AbstractStrategy *ak47 = new AkStrategy;
    Hero *hero = new Hero;

    cout << "远程兵来了， 更换远程攻击" << endl;
    hero->setStrategy(ak47);
    hero->fight();

    cout << "近战兵 来了， 更换近战的攻击" << endl;
    hero->setStrategy(knife);
    hero->fight();

    return 0;
}
