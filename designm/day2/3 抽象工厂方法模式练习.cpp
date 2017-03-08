#define  _CRT_SECURE_NO_WARNINGS 
#include <iostream>
using namespace std;
//产品等级结构稳定
// 0---------  抽象层 ------
class CPU
{
    public:
    virtual void caculate() = 0; 
};
class Card
{
    public:
    virtual void display() = 0;
};
class Memory
{
    public:
    virtual void storage() = 0;
};
class AbstractFactory
{
    public:
    virtual CPU* createCPU() = 0;
    virtual Card* createCard() = 0;
    virtual Memory * createMem() = 0;
};
//架构类
class Computer
{
    public:
    Computer(CPU *cpu, Card *card, Memory *mem)
    {
        this->cpu = cpu;
        this->card = card;
        this->mem = mem;
    }
    void work() 
    {
        this->cpu->caculate();
        this->card->display();
        this->mem->storage();
    }
    private:
    CPU *cpu;
    Card*card;
    Memory *mem;
};
//抽象层结束------
// ----- 实现层-----
//intel厂商
class IntelCPU :public CPU
{
    public:
    virtual void caculate() 
    {
        cout << "Intel CPU 开始计算了..." << endl;
    }
};
class IntelCard :public Card
{
    public:
    virtual void display() 
    {
        cout << "Intel card 开始显示 了..." << endl;
    }
};
class IntelMem :public Memory
{
    public:
    virtual void storage() 
    {
        cout << "Intel mem 开始存储了..." << endl;
    }
};
class IntelFactory :public AbstractFactory
{
    public:
    virtual CPU* createCPU()  
    {
        return new IntelCPU;
    }
    virtual Card* createCard() 
    {
        return new IntelCard;
    }
    virtual Memory * createMem()  
    {
        return new IntelMem;
    }
};
// NVIDIA的厂商
class NvidiaCPU :public CPU
{
    public:
    virtual void caculate() 
    {
        cout << "Nvidia CPU 开始计算了..." << endl;
    }
};
class NvidiaCard :public Card
{
    public:
    virtual void display() 
    {
        cout << "Nvidia card 开始显示 了..." << endl;
    }
};
class NvidiaMem :public Memory
{
    public:
    virtual void storage() 
    {
        cout << "Nvidia mem 开始存储了..." << endl;
    }
};
class NvidiaFacory :public AbstractFactory
{
    public:
    virtual CPU* createCPU()  
    {
        return new NvidiaCPU;
    }
    virtual Card* createCard() 
    {
        return new NvidiaCard;
    }
    virtual Memory * createMem()  
    {
        return new NvidiaMem;
    }
};
// Kinston 厂商
class KinstonCPU :public CPU
{
    public:
    virtual void caculate() 
    {
        cout << "Kinston CPU 开始计算了..." << endl;
    }
};
class KinstonCard :public Card
{
    public:
    virtual void display() 
    {
        cout << "Kinston card 开始显示 了..." << endl;
    }
};
class KinstonMem :public Memory
{
    public:
    virtual void storage() 
    {
        cout << "Kinston mem 开始存储了..." << endl;
    }
};
class KinstonFacory :public AbstractFactory
{
    public:
    virtual CPU* createCPU()  
    {
        return new KinstonCPU;
    }
    virtual Card* createCard() 
    {
        return new KinstonCard;
    }
    virtual Memory * createMem()  
    {
        return new KinstonMem;
    }
};
int main(void)
{
    //必须用一个工厂的东西 
    //1 组装一个intel系列的电脑 对intel公司的所有产品，执行一个单元测试。
    //创建一个intel的工厂
    AbstractFactory * intelFactory = new IntelFactory;
    CPU *intelCPU = intelFactory->createCPU();
    Card *intelCard = intelFactory->createCard();
    Memory *intelMem = intelFactory->createMem();
    Computer *com1 = new Computer(intelCPU, intelCard, intelMem);
    com1->work();
    cout << " -----  " << endl;

    //2 组装一个 intel的cpu  kinston card  NVIDIA的显卡的电脑
    AbstractFactory *kinstonFactory = new KinstonFacory;
    Memory *kingstonMem = kinstonFactory->createMem();
    AbstractFactory *nvidiaFactory = new NvidiaFacory;
    Card *nvidiaCard = nvidiaFactory->createCard();
    Computer *com2 = new Computer(intelCPU, nvidiaCard, kingstonMem);
    com2->work();
    return 0;
}
