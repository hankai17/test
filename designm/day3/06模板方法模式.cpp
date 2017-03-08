#define  _CRT_SECURE_NO_WARNINGS 
#include <iostream>
using namespace std;
//抽象的制作饮料方法
class MakeDrink
{
    public:
    //1 把水煮开
    void boil() 
    {
        cout << "把水煮开" << endl;
    }
    //2 冲某物
    virtual void brew() = 0;
    //3 从大杯倒入小杯
    void putInCup()
    {
        cout << "把冲泡好的饮料 从大杯倒入小杯" << endl;
    }
    //4 加一些酌料
    virtual void addThings() = 0;
    //钩子函数， hook   选择某一步骤是否执行
    virtual bool CustomWantAddThings() 
    {
        return true;
    }
    //业务的逻辑的统一模板 
    void make() 
    {
        boil();
        brew(); //子类
        putInCup(); 
        if (CustomWantAddThings() == true) 
        {
            addThings(); //子类的多态
        }
    }
};
//制作咖啡
class MakeCoffee :public MakeDrink
{
    public:
    MakeCoffee(bool isAdd)
    {
        this->isAdd = isAdd;
    }
    //2 冲某物
    virtual void brew()
    {
        cout << "冲泡咖啡豆" << endl;
    }
    //4 加一些酌料
    virtual void addThings()  
    {
        cout << "添加糖和牛奶" << endl;
    }
    virtual bool CustomWantAddThings() 
    {
        return isAdd;
    }
    private:
    bool isAdd;
};
//冲泡茶叶
class MakeTea :public MakeDrink
{
    public:
    MakeTea(bool isAdd)
    {
        this->isAdd = isAdd;
    }
    //2 冲某物
    virtual void brew() 
    {
        cout << "冲泡 茶叶" << endl;
    }
    //4 加一些酌料
    virtual void addThings()  
    {
        cout << "添加 柠檬 或者 菊花" << endl;
    }
    virtual bool CustomWantAddThings() 
    {
        return isAdd;
    }
    private:
    bool isAdd;
};
int main(void)
{
    MakeDrink *makeCoffee = new MakeCoffee(true);
    makeCoffee->make();

    MakeDrink *makeTea = new MakeTea(false);
    makeTea->make();

    return 0;
}
