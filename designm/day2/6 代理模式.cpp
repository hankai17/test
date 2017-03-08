#define  _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <string>
using namespace std;
//商品
class Item
{
    public:
    Item(string kind, bool fact)
    {
        this->kind = kind;
        this->fact = fact;
    }
    string getKind()
    {
        return this->kind;
    }
    bool getFact()
    {
        return this->fact;
    }
    private:
    string kind;//商品的种类
    bool fact; //商品的真假
};

// 抽象的购物方式
class Shopping
{
    public:
    virtual void buy(Item *it) = 0;//抽象的买东西方法
};

//韩国购物
class KoreaShopping :public Shopping
{
    public:
    virtual void buy(Item *it)  
    {
        cout << "去韩国买了" << it->getKind()<< endl;
    }
};
//美国购物
class USAShopping :public Shopping
{
    public:
    virtual void buy(Item *it)  
    {
        cout << "去美国买了" << it->getKind() << endl;
    }
};
//海外代理
class OverseasProxy :public Shopping
{
    public:
    OverseasProxy(Shopping *shpping)
    {
        this->shopping = shpping;
    }
    virtual void buy(Item *it)  
    {
        //1 辨别商品的真假，
        //2 进行购买（）
        //3 通过海关安检，带回祖国
        if (it->getFact() == true)
        {
            cout << "1 发现正品， 要购物" << endl;
            //用传递进来的购物方式去购物
            shopping->buy(it);
            //3 安检
            cout << "2 通过海关安检， 带回祖国" << endl;
        }
        else 
        {
            cout << "1 发现假货，不会购买" << endl;
        }
    }
    private:
    Shopping *shopping; //有一个购物方式
};
int main(void)
{
    //1 辨别商品的真假，
    //2 进行购买（）
    //3 通过海关安检，带回祖国
    Item it1("nike鞋", true);
    Item it2("CET4证书", false);
    #if 0
    // 想去韩国买一个鞋
    Shopping *koreaShopping = new KoreaShopping;
    //1  辨别商品的真伪
    if (it1.getFact() == true) {
        cout << "1 发现正品， 要购物" << endl;
        //2 去韩国买了这个商品
        koreaShopping->buy(&it1);
        //3 安检
        cout << "2 通过海关安检， 带回祖国" << endl;
    }
    else {
        cout << "3 发现假货，不会购买" << endl;
    }
    #endif
    Shopping *usaShopping = new USAShopping;
    Shopping *overseaProxy = new OverseasProxy(usaShopping);
    overseaProxy->buy(&it1);
    return 0;
}
