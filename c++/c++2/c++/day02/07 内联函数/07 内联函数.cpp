#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

#define ADD(X,Y) ((X)+(Y))

int Add(int x,int y){
	return x + y ;
}

void test01(){
	int ret1 = ADD(10, 20) * 10;
	int ret2 = Add(10, 20) * 10;

	cout << "ret1:" << ret1 << endl;
	cout << "ret2:" << ret2 << endl;
}

#define COMPARE(X,Y) ((X)<(Y)?(X):(Y))
int compare(int x,int y){
	return x < y ? x : y;
}

void test02(){
	
	int a = 1;
	int b = 3;

	//cout << COMPARE(++a, b) << endl; // ((++a)<(b)?(++a):(b))
	cout << compare(++a, b) << endl;
}


//内联函数语法
inline void func(){

}

//不合法，无法成为内联函数
inline void func1();
void func1(){}


int main(){

	test02();

	system("pause");
	return EXIT_SUCCESS;
}