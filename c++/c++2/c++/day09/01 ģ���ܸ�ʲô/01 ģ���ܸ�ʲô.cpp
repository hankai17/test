#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

//交换两个Int类型的变量
void SwapInt(int& a,int& b){
	int temp = a;
	a = b;
	b = temp;
}

//交换两个double类型变量
void SwapDouble(double& a,double& b){
	double temp = a;
	a = b;
	b = temp;
}

//交换char类型
void SwapChar(char& a,char& b){
	char temp = a;
	a = b;
	b = temp;
}

//仅仅由于类型不同，我们要编写交换函数，就需要定义很多函数
//函数多了，对应的维护任务量大了，维护成本高了，费劲
//类型参数化

//c++提供了模板机制，泛型编程
 //template<typename T>
template<class T>  //只对当前行下一个函数有效
void MySwap(T& a,T& b){
	T temp = a;
	a = b;
	b= temp;
}


//一般情况下，对于函数模板都是使用自动推导


int main(){

	int a = 10;
	int b = 20;
	//SwapInt(a, b);

	//调用函数模板
	//1. 显式制定类型
	MySwap<int>(a,b);
	cout << "A:" << a << " B:" << b << endl;

	double da = 1.23;
	double db = 3.21;

	MySwap<double>(da, db);
	cout << "DA:" << da << " DB:" << db << endl;

	//2. 通过自动推导
	MySwap(a, b);
	MySwap(da, db);


	system("pause");
	return EXIT_SUCCESS;
}