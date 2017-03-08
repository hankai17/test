#define  _CRT_SECURE_NO_WARNINGS 
#include <iostream>
using namespace std;
//工厂是一个工厂产一个东西 而抽象工厂是生产一个族的东西 从而减少了工厂数目
//水果有9种;工厂有3（地域）个针对产品族

//1 添加一个日本的工厂 和日本的苹果 和 香蕉  (添加一个产品族)
//针对产品族进行添加 符合开闭

//2 添加一个产品种类， 鸭梨  (添加一个产品等级结构)
//针对产品等级机构添加 不符合开闭
//每添加一个水果 就得在工厂里添加相应的工厂

//试用与产品等级结构非常清晰的情况下 电脑组装
class Fruit
{
    public:
    virtual void getName() = 0;
};
class USAApple :public Fruit
{
    public:
    virtual void getName()  
    {
        cout << "我是美国的苹果" << endl;
    }
};
class USABanana :public Fruit
{
    public:
    virtual void getName()  
    {
        cout << "我是美国的香蕉" << endl;
    }
};
class USAPear :public Fruit
{
    public:
    virtual void getName()  
    {
        cout << "我是美国的梨" << endl;
    }
};
class ChinaApple :public Fruit
{
    public:
    virtual void getName()  
    {
        cout << "我是中国的苹果" << endl;
    }
};
class ChinaBanana :public Fruit
{
    public:
    virtual void getName()  
    {
        cout << "我是中国的香蕉" << endl;
    }
};
class ChinaPear :public Fruit
{
    public:
    virtual void getName()  
    {
        cout << "我是中国的梨" << endl;
    }
};
//顶一个一个抽象的工厂，是面向产品族进行生产
class AbstractFactory 
{
    public:
    virtual Fruit *createApple() = 0;
    virtual Fruit *createBanana() = 0;
    //添加一个鸭梨的生成器
    virtual Fruit* createPear() = 0;
};
//美国的具体工厂
class USAFactory :public AbstractFactory
{
    public:
    virtual Fruit *createApple() 
    {
        return new USAApple;
    }
    virtual Fruit *createBanana()  
    {
        return new USABanana;
    }
    //添加一个美国的生产器
    virtual Fruit * createPear() 
    {
        return new USAPear;
    }
};
//中国的工厂
class ChinaFactory :public AbstractFactory
{
    public:
    virtual Fruit *createApple()
    {
        return new ChinaApple;
    }
    virtual Fruit *createBanana()  
    {
        return new ChinaBanana;
    }
    //添加一个的生产器
    virtual Fruit * createPear() 
    {
        return new ChinaPear;
    }
};
//添加一个日本的产品族，工厂，和具体产品  符合 开闭原则
class JapanApple :public Fruit
{
    public:
    virtual void getName() 
    {
        cout << "我是日本的苹果" << endl;
    }
};
class JapanBanana :public Fruit
{
    public:
    virtual void getName() 
    {
        cout << "我是日本的香蕉" << endl;
    }
};
class JapanPear :public Fruit
{
    public:
    virtual void getName()  
    {
        cout << "我是日本的梨" << endl;
    }
};
class JapanFactory :public AbstractFactory
{
    public:
    virtual Fruit *createApple() 
    {
        return new JapanApple;
    }
    virtual Fruit *createBanana()  
    {
        return new JapanBanana;
    }
    //添加
    virtual Fruit * createPear() 
    {
        return new JapanPear;
    }
};
int main(void)
{
    //想要一个美国的香蕉
    //1 来一个美国的工厂
    AbstractFactory * usaF = new USAFactory;
    //2 来一个香蕉
    Fruit *usaBanana = usaF->createBanana();
    usaBanana->getName();

    //想要一个中国的苹果
    AbstractFactory *chinaF = new ChinaFactory;
    Fruit *chinaApple = chinaF->createApple();
    chinaApple->getName();

    return 0;
}
