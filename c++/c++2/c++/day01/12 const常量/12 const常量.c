#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void test01(){
	
	//c语言中const就是只读变量
	const int a = 10;
	//a = 100;
	int* pA = &a;
	*pA = 100;

	printf("a:%d\n",a);
	printf("*pA:%d\n", *pA);

}


//c语言的const是外部连接，其他文件可访问
//extern const int a;

const int a = 10;
int main(){

	int* p = (int*)&a;
	*p = 200;

	printf("%d\n",a);

	test01();

	system("pause");
	return EXIT_SUCCESS;
}


//c++中的全局const和c中的全局const，编译器会把这个const常量放到只读数据段，我们可以取地址，但是不能修改