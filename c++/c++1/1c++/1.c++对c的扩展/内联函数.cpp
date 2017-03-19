//const int a=3 等价于deifne a 3
//类似内联是替代一块内存区域
//内联函数在编译时候直接将函数体插入函数调用的地方
//inline只是一种请求，编译器不一定允许这种请求
//内联函数省去了普通函数调用时候的压栈 跳转和返回开销
#include <iostream>
using namespace std;

inline void printA()//内联函数必须和函数的实现写在一块
{
	int a = 10;
	cout << "a：" << a << endl;
}


void main1()
{
	printA();//只适合于短小精悍的代码 只做显示
	cout << "hello" << endl;
	system("pause");

}
//宏与内联区别
#define MYFUNC(a,b) ((a)<(b)?(a):(b))//带参数的宏
inline int myfunc(int a, int b)
{
	return a < b ? a : b;
}
int main()
{
	int a = 1;
	int b = 3;
	//int c = myfunc(++a, b);//这一句是函数 a=2 b=3 c=2
	int c = MYFUNC(++a, b); //这一句调用宏，调用宏就展开 ((++a) < (b) ? (++a) : (b))//a=3 b=3 c=3
	printf("a=%d\n", a);
	printf("b=%d\n", b);
	printf("c=%d\n", c);

	system("pause");
	return 0;
}//一般不要用++--作函数参数