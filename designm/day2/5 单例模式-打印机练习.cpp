#define  _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <string>
using namespace std;

class Printer
{
    public:
    static Printer *getInstance() 
    {
        return instance;
    }
    void print(string text)
    {
        cout << "打印的内容是" << text << endl;
        sequence++;
        cout << "今天已经打印了 " << sequence << "条数据" << endl;
    }
    static int getCnt() 
    {
        return sequence;
    }

    private:
    Printer() {}
    //垃圾回收工
    class Garbo //释放单例//因为是静态区 当程序结束 就自动调析构
    {
        private:
        ~Garbo() 
        {
            if (instance != NULL) 
            {
                delete instance;
            }
        }
    };
    static Garbo garbo; //在静态区域开辟一个对象， garbo
    static int sequence; //记录打印机已经打印了多少条数据。
    static Printer* instance;
};
int Printer::sequence = 0;
Printer* Printer::instance = new Printer;
Printer::Garbo  Printer::garbo;

int main(void)
{
    //员工1  获取
    Printer *p1 = Printer::getInstance();//获取到唯一的打印机实例
    p1->print("一份简历");
    Printer *p2 = Printer::getInstance();
    p2->print("lol 皮肤");
    Printer *p3 = Printer::getInstance();
    p3->print("离职申请");
    return 0;
}
