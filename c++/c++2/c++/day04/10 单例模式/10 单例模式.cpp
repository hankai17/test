#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Chairman{
private:
	Chairman(){}
	Chairman(const Chairman&){}
	~Chairman(){}
public:
	static Chairman* getInstance(){
		return pChairman;
	}
	void Speak(){
		cout << "同志们，辛苦了!" << endl;
	}
private:
	static Chairman* pChairman; //类内声明
};

//类外初始化
Chairman* Chairman::pChairman = new Chairman; //饿汉式创建单例模式

//1. 构造函数和拷贝构造函数私有
//2. 提供静态成员变量指向一个对象
//3. 私有化静态成员变量
//4. 提供外部获得对象的函数，静态成员函数

int main(){

	Chairman* p1 = Chairman::getInstance();
	Chairman* p2 = Chairman::getInstance();

	p1->Speak();

	if (p1 == p2){
		cout << "同个对象!" << endl;
	}
	else{
		cout << "两个对象!" << endl;
	}

#if 0
	Chairman  p3(*p1); //调用拷贝构造，创建新的对象
	if (&p3 == p1){
		cout << "相同!" << endl;
	}
	else{
		cout << "不相同!" << endl;
	}
#endif


	system("pause");
	return EXIT_SUCCESS;
}