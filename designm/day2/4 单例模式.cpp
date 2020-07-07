#define  _CRT_SECURE_NO_WARNINGS 
#include <iostream>
using namespace std;
/*
class A
{
public:
private:
A() {
    }
    };
    */
/*
一是某个类只能有一个实例；
二是它必须自行创建这个实例；
三是它必须自行向整个系统提供这个实例。
*/
/*
单例模式的使用步骤：
a)	 构造函数私有化。// 为不让在类的外部再创建一个本类的实例
b)	 提供一个全局的静态方法（全局访问点）来获取单例对象。
c)	 在类中定义一个静态指针，指向本类的变量的静态变量指针 。
*/
class Singleton
{
    public:
    static Singleton* getInstance()
    {
        return instance;
    }
    private:
    //不让类的外部再创建实例
    Singleton() {
    }
    static Singleton* instance;//指向本类唯一实例的指针。
};
//饿汉式  - 在编译期间就已经确定这个唯一的实例了。
Singleton*  Singleton::instance = new Singleton; //类的内部



//懒汉式
//当懒汉遇到高并发
class Singleton2
{
    public:
    static Singleton2* getInstance()
    {
        //加锁
        if (instance == NULL) {
            instance = new Singleton2;
        }
        //关锁
        return instance;
    }
    private:
    Singleton2() {
    }
    static Singleton2 * instance;
};

//懒汉式的初始化方式
Singleton2 *Singleton2::instance = NULL;

int main(void)
{
    Singleton * s1 = Singleton::getInstance();
    Singleton *s2 = Singleton::getInstance();
    if (s1 == s2) {
        cout << "s1 == s2" << endl;
    }
    else {
        cout << "s1 != s2 " << endl;
    }


    Singleton2 *s3 = Singleton2::getInstance();
    Singleton2 *s4 = Singleton2::getInstance();
    if (s3 == s4) {
        cout << "s3 == s4" << endl;
    }
    else {
        cout << "s3 != s4" << endl;
    }
    return 0;
}
https://www.cnblogs.com/xuhuajie/p/11647164.html  懒汉用std::call_once 保证线程安全
