#define  _CRT_SECURE_NO_WARNINGS 
#include <iostream>
//开闭原则就是 增加功能不是通过修改源代码而是拓展的方式增加功能
//所以必须有抽象出的纯虚类 纯虚函数 通过继承
//虚函数重写 来实现新功能
//开闭原则就是多态

using namespace std;
//抽象类
class AbstractBanker
{
    public:
    virtual void work() = 0; //抽象的接口
};
//存款
class SaveBanker:public AbstractBanker
{
    public:
    virtual void work() {
        cout << "存款" << endl;
    }
};
//支付
class PayBanker:public AbstractBanker
{
    public:
    virtual void work() {
        cout << "支付" << endl;
    }
};
//转账
class TransBanker:public AbstractBanker
{
    public:
    virtual void work() {
        cout << "Trans" << endl;
    }
};
//添加一个办理基金的功能
class FuncBanker :public AbstractBanker
{
    public:
    virtual void work() {
        cout <<"基金"<<endl;
    }
};
#if 0
class Banker
{
    public:
    void save() {
        cout << "存款" << endl;
    }
    void pay() {
        cout << "支付" << endl;
    }
    void transfer() {
        cout << "转账" << endl;
    }
    //添加一个办理基金业务
    void fund() {
        cout << "办理基金" << endl;
    }
};
#endif
int main(void)
{
    #if 0
    Banker b;
    //存款
    b.save();
    b.transfer();
    #endif
    AbstractBanker *sb = new SaveBanker;
    sb->work();
    delete sb;
    AbstractBanker *tb = new TransBanker;
    tb->work();
    delete tb;
    return 0;
}
