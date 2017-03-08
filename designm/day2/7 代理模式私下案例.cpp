#define  _CRT_SECURE_NO_WARNINGS 
#include <iostream>
using namespace std;
//抽象的美女类
class BeautyGirl
{
    public:
    //1 跟男人抛媚眼
    virtual void MakeEyesWithMan() = 0;
    //2 与男人共度美好的约会
    virtual void HappyWithMan() = 0;
};
//潘金莲
class JinLian :public BeautyGirl
{
    public:
    //1 跟男人抛媚眼
    virtual void MakeEyesWithMan()  
    {
        cout << "潘金莲抛了一个媚眼" << endl;
    }
    //2 与男人共度美好的约会
    virtual void HappyWithMan()  
    {
        cout << "潘金莲跟你共度约会" << endl;
    }
};
class WangPo :public BeautyGirl
{
    public:
    WangPo(BeautyGirl *girl) 
    {
        this->girl = girl;
    }
    //1 跟男人抛媚眼
    virtual void MakeEyesWithMan() 
    {
        // ...
        girl->MakeEyesWithMan();
        //...
    }
    //2 与男人共度美好的约会
    virtual void HappyWithMan() 
    {
        girl->MakeEyesWithMan();
    }
    private:
    BeautyGirl *girl;
};
//西门大官人
int main(void)
{
    BeautyGirl *jinlian = new JinLian;
    WangPo *wangpo = new WangPo(jinlian);
    //向让潘金莲抛一个
    wangpo->MakeEyesWithMan();
    //让金莲，月个会
    wangpo->HappyWithMan();
    return 0;
}
