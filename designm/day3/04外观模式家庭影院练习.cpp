#define  _CRT_SECURE_NO_WARNINGS 
#include <iostream>
using namespace std;
/*
根据类图，实现家庭影院外观模式应用。
实现KTV模式：电视打开，灯关掉，音响打开，麦克风打开，dvd打开；
实现游戏模式：电视打开，音响打开，游戏机打开。
*/
class TV
{
    public:
    void On() 
    {
        cout << "电视打开了" << endl;
    }
    void Off() 
    {
        cout << "电视关闭了" << endl;
    }
};

class DVD
{
    public:
    void On() 
    {
        cout << "DVD打开了" << endl;
    }
    void Off() 
    {
        cout << "DVD关闭了" << endl;
    }
};

class Xbox
{
    public:
    void On() 
    {
        cout << "Xbox打开了" << endl;
    }
    void Off() 
    {
        cout << "Xbox关闭了" << endl;
    }
};

class MikePhone
{
    public:
    void On() 
    {
        cout << "MikePhone打开了" << endl;
    }
    void Off() 
    {
        cout << "MikePhone关闭了" << endl;
    }
};

class Light
{
    public:
    void On() 
    {
        cout << "Light打开了" << endl;
    }
    void Off() 
    {
        cout << "Light关闭了" << endl;
    }
};
//******************************
class HomePlayer
{
    public:
    //ktv模式的接口
    void doKTV() 
    {
        light.Off();
        tv.On();
        dvd.On();
    }
    //游戏模式的接口
    void doGame()
    {
        tv.On();
        xbox.On();
    }
    Light light;
    TV tv;
    MikePhone mike;
    Xbox xbox;
    DVD dvd;
};

int main(void)
{
    HomePlayer hp;
    cout << "进入ktv模式" << endl;
    hp.doKTV();
    cout << "进入游戏模式" << endl;
    hp.doGame();
    return 0;
}
