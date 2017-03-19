#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class AbstractDrinking{
public:
	virtual void Zhushui() = 0;
	virtual void Chongpao() = 0;
	virtual void PourInBeizhong() = 0;
	virtual void addSomething() = 0;
	//模板方法
	void Make(){
		Zhushui();
		Chongpao();
		PourInBeizhong();
		addSomething();
	}
};

//制作咖啡
class MakeCoffee : public AbstractDrinking{
public:
	virtual void PourInBeizhong(){
		cout << "将咖啡倒入杯中!" << endl;
	}
	virtual void addSomething() {
		cout << "加点盐!" << endl;
	}
	virtual void Zhushui(){
		cout << "煮自开水!" << endl;
	}
	virtual void Chongpao(){
		cout << "冲泡！" << endl;
	}
};

//制作茶水
class MakeTea : public AbstractDrinking{
public:
	virtual void Zhushui(){
		cout << "煮农夫山泉!" << endl;
	}
	virtual void Chongpao(){
		cout << "冲泡大红袍!" << endl;
	}
	virtual void PourInBeizhong(){
		cout << "茶水倒入杯中！" << endl;
	}
	virtual void addSomething(){
		cout << "加点辣椒面儿!" << endl;
	}
};

int main(){

	AbstractDrinking* drinking = new MakeCoffee;
	drinking->Make();
	delete drinking;

	cout << "----------" << endl;
	drinking = new MakeTea;
	drinking->Make();
	 
	delete drinking;

	system("pause");
	return EXIT_SUCCESS;
}