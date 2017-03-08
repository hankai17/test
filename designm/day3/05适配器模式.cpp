#define  _CRT_SECURE_NO_WARNINGS 
#include <iostream>

using namespace std;

//5v电压的类
/*
class V5
{
public:
void useV5() {
cout << "使用了5v的电压" << endl;
}
};
*/

class V5
{
    public:
    virtual void useV5() = 0; 
};

//目前只有v220的类 没有v5
class V220
{
    public:
    void useV220() 
    {
        cout << "using 220v" << endl;
    }
};

//定义一个中间的适配器类
//首先适配器继承原本的
//重写修改原本的方法
class Adapter :public V5
{
    public:
    Adapter(V220 * v220)
    {
        this->v220 = v220;
    }
    ~Adapter() 
    {
        if (this->v220 != NULL) 
        {
            delete this->v220;
        }
    }

    virtual void useV5() 
    {
        v220->useV220(); //调用需要另外的方法
    }

    private:
    V220 *v220;
};

class iPhone//所有代码基于iphone不能修改的前提下
{
    public:
    iPhone(V5 *v5)
    {
        this->v5 = v5;
    }
    ~iPhone()
    {
        if (this->v5 != NULL) 
        {
            delete this->v5;
        }
    }

    //充电的方法
    void charge()
    {
        cout << "iphone is charging" << endl;
        v5->useV5();
    }
    private:
    V5*v5; 
};

int main(void)
{
    iPhone *phone = new iPhone(new Adapter(new V220));
    phone->charge();

    return 0;
}
