#define  _CRT_SECURE_NO_WARNINGS 
#include <iostream>
using namespace std;
//如果类派生的很多 最好建立一个工厂
//单一原则修改简单工厂  
//简单工厂模式 规模较小的模型
//工厂方法模式 中等
//抽象工厂模式 复杂的产品等级结构和产品族
//抽象的水果类
//
//
//最好把个数都对比一下
class Fruit
{
    public:
    virtual void getName() = 0;
};
class Apple :public Fruit 
{
    public:
    virtual void getName() 
    {
        cout << "我是苹果" << endl;
    }
};
class Banana :public Fruit 
{
    public:
    virtual void getName() 
    {
        cout << "我是香蕉 " << endl;
    }
};
//添加一个梨产品
class Pear : public Fruit
{
    public:
    virtual void getName() 
    {
        cout << "我是鸭梨 " << endl;
    }
};
//抽象的工厂类
class AbstractFactory
{
    public:
    virtual Fruit * createFruit() = 0;//抽象的水果生产器
};
//苹果的工厂
class AppleFactory :public AbstractFactory
{
    public:
    virtual Fruit * createFruit() 
    {
        return new Apple;
    }
};
//香蕉工厂 
class BananaFactory : public AbstractFactory 
{
    public:
    virtual Fruit *createFruit() 
    {
        return new Banana;
    }
};
//添加梨的工厂
class PearFactory :public AbstractFactory
{
    public:
    virtual Fruit *createFruit() 
    {
        //..--------------------------这里对梨进行各种判断  是雪花里还是岩石里
        //----------------------------工厂的好处 就在这里
        return  new Pear;
    }
};
int main(void)
{
    //只需new出来相应的工厂即可,再调用工厂的函数
    //main与工厂打交道 不与水果直接交互
    
    //1 给我来一个香蕉的工厂
    AbstractFactory * bananaFactory = new BananaFactory;
    //2 给我来一个水果
    Fruit * banana = bananaFactory->createFruit(); //只能够生成香蕉 //多态
    //Fruit *banana = new Banana;
    banana->getName(); //多态
    delete bananaFactory;
    delete banana;

    AbstractFactory *appleFactory = new AppleFactory;
    Fruit *apple = appleFactory->createFruit();
    apple->getName();

    AbstractFactory *pearFactory = new PearFactory;
    Fruit *pear = pearFactory->createFruit();
    //Fruit *pear = new Pear;
    pear->getName(); //多态

    return 0;
}
