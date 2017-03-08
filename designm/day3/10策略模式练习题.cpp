#define  _CRT_SECURE_NO_WARNINGS 
#include <iostream>
using namespace std;
//销售策略
class AbstractStrategy
{
    public:
    //商品具体的销售策略计算方式
    virtual double getPrice(double price) = 0;
};
//策略A  商品打八折
class StrategyA :public AbstractStrategy
{
    public:
    virtual double getPrice(double price)  
    {
        return price*0.8;
    }
};
//策略B 如果商品超过200， 减100
class StrategyB :public AbstractStrategy
{
    public:
    virtual double getPrice(double price)
    {
        if (price > 200) 
        {
            price = price - 100;
        }
        return price;
    }
};
//商品
class Item
{
    public:
    Item(string name, double price)
    {
        this->name = name;
        this->price = price;
    }
    //提供一个可以更换策略的方法
    void setStrategy(AbstractStrategy *strategy)
    {
        this->strategy = strategy;
    }
    //最终获得商品的价格的方法
    double SellPrice()
    {
        return this->strategy->getPrice(this->price);
    }
    private:
    string name;
    double price;//商品的价格
    //销售的策略
    AbstractStrategy *strategy;
};
int main(void)
{
    Item it("nike鞋", 201);
    AbstractStrategy *sA = new StrategyA;
    AbstractStrategy *sB = new StrategyB;

    cout << "上午 商场执行 销售策略A 全程八折" << endl;
    it.setStrategy(sA); //让商品设置A销售策略
    cout << "nike鞋应该卖" << it.SellPrice() << endl;

    cout << "下午商场执行 销售策略B， 全场 超200减100" << endl;
    it.setStrategy(sB);
    cout << "nike鞋应该卖" << it.SellPrice() << endl;
    return 0;
}
