//主教与怪的故事
#include <iostream>
using namespace std;

class HeroFighter
{
public:
	virtual int power()
	{
		return 10;
	}
protected:
private:

};

class EnemyFighter
{
public:
	int attack()
	{
		return 15;
	}
protected:
private:

};

class AdvHeroFighter :public  HeroFighter
{
public:
	virtual int power()//子类函数与父类函数相同 是多态 加virtual关键字
	{
		return 20;
	}
protected:
private:

};


class AdvAdvHeroFighter :public  HeroFighter
{
public:
	virtual int power()//子类函数与父类函数相同 是多态 加virtual关键字
	{
		return 30;
	}
protected:
private:

};

void Playobj(HeroFighter *hf, EnemyFighter *ef)//基类指针可以指向父类对象也可指向子类对象
{
	if (hf->power()>ef->attack())//hf->power()会有多态发生 ，若是父类的hf则执行父类的 若是子类的hf则执行子类的
		//同样一句话产生不同效果
	{
		printf("主角赢了！\n");
	}
	else
	{
		printf("主角挂了！\n");
	}

}

//HeroFighter EnemyFighter AdvHeroFighter

void main()
{
	HeroFighter hf;
	AdvHeroFighter Advhf;
	EnemyFighter ef;
	AdvAdvHeroFighter advadvhf;//此框架可以把后来人写的代码利用起来 


	Playobj(&hf, &ef);
	Playobj(&Advhf, &ef);
	Playobj(&advadvhf, &ef);

	system("pause");
	return;
}
void main1()//不是多态
{
	HeroFighter hf;
	AdvHeroFighter Advhf;
	EnemyFighter ef;

	if (hf.power() > ef.attack())
	{
		printf("主角赢了！\n");
	}
	else
	{
		printf("主角挂了！\n");
	}
	if (Advhf.power() > ef.attack())
	{
		printf("Adv主角赢了！\n");
	}
	else
	{
		printf("Adv主角挂了！\n");
	}

	system("pause");
	return;
}

/*
多态的思想
面向对象三大概念
封装：突破C函数概念 用类做函数参数的时候 可以使用对象的属性和方法
继承：A B 代码复用
多态：可以使用未来


C中 间接赋值是指针存在的最大意义 条件定义两个变量 建立关联 *p在被调用函数中取间接修改实参
C++中 实现多态的三个条件
要有继承 要有虚函数重写（同名函数） 用父类指针

*/
