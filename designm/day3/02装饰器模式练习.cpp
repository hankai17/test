#define  _CRT_SECURE_NO_WARNINGS 
#include <iostream>
using namespace std;
class Hero
{
public:
	virtual void status() = 0;
};
class Akali :public Hero
{
public:
	virtual void status()  
    {
		cout << "HP: 1000" << endl;
		cout << "AP: 500" << endl;
		cout << "AD: 50" << endl;
	}
};
//英雄的装饰器
class Decorator :public Hero
{
public:
	Decorator(Hero *hero)
    {
		this->hero = hero;
	}
	virtual void status() = 0;
protected:
	Hero *hero;
};
//日炎斗篷的装饰器
class RYDecorator :public Decorator
{
public:
	RYDecorator(Hero *hero) : Decorator(hero) {}
	virtual void status()  
    {
		this->hero->status(); //先调用被装饰的 英雄的基本状态
		cout << "HP + 100000" << endl;
	}
};
//深渊权杖的装备装饰器
class SYQZDecorator :public Decorator
{
public:
	SYQZDecorator(Hero *hero) : Decorator(hero) {}
	virtual void status() 
    {
		this->hero->status();
		cout << "AP +50000 " << endl;
	}
};
int main(void)
{
	Hero *akali = new Akali;
	cout << "akali的初始状态" << endl;
	akali->status();
    
	cout << "通过日然斗篷创建新的akali" << endl;
	Hero *ryAkali = new RYDecorator(akali);
	ryAkali->status();

	cout << "再通过 深渊权杖的装备装饰器 修饰日炎akali" << endl;
	Hero *syAkali = new SYQZDecorator(ryAkali);
	syAkali->status();

	return 0;
}
