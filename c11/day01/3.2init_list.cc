#include <iostream>

class Foo
{
	public:
		Foo(int){}

	private:
		Foo(const Foo&);
};

int main(void)
{
	Foo a1(123);
	Foo a2 = a1;  //error C2248: ��Foo::Foo��: �޷����� private ��Ա(�ڡ�Foo����������)
	Foo a3 = { 123 }; //just as init a1
	Foo a4{ 123 }; //just as init a1
			//������ϰ���ϼ���=
	int a5 = { 3 };
	int a6 { 3 };

	system("pause");
	return 0;
}

/* int* a = new int {123}  �� int* a = new(123) һ��
 * int* arr = new int[3] { 1, 2, 3}
 */

/* struct Foo{Foo(int,double){}};
 * Foo func(void) { retrun {1,1.000};}
 */
