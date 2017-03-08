#define  _CRT_SECURE_NO_WARNINGS 
#include <iostream>

using namespace std;

class SysA
{
    public:
    void operationA() 
    {
        cout << "sysA。。。" << endl;
    }
};

class SysB
{
    public:
    void operationB() 
    {
        cout << "sysB。。。" << endl;
    }
};

class SysC
{
    public:
    void operationC() 
    {
        cout << "sysC。。。" << endl;
    }
};

class SysD
{
    public:
    void operationD() 
    {
        cout << "sysD。。。" << endl;
    }
};

class Facade
{
    public:
    void methodOne()
    {
        sysa.operationA();
        sysb.operationB();
    }

    void methodTwo()
    {
        sysc.operationC();
        sysd.operationD();
    }

    private:
    SysA sysa;
    SysB sysb;
    SysC sysc;
    SysD sysd;
};

int main(void)
{
    /*
    SysA sa;
    SysB sb;
    sa.operationA();
    sb.operationB();
    */
    //上面的话，main与sa sb的关系性很强
    //让main函数与fa耦合

    Facade fa;
    //AB 方法组合
    fa.methodOne();

    //CD 组合
    fa.methodTwo();

    return 0;
}
