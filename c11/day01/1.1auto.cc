#include <iostream>

void func(auto a = 1);  //error:auto�������ں�������

struct Foo
{
	auto var1_ = 0;  //error:auto�������ڷǾ�̬��Ա����
	static const auto var2_ = 0;
};

template <typename T>
struct Bar {};

int main(void)
{
	int arr[10] = {0};
	auto aa = arr;  //OK: aa -> int *
	auto rr[10] = arr;  //error:auto�޷���������
	Bar<int> bar;
	Bar<auto> bb = bar;  //error:auto�޷��Ƶ���ģ�����

	system("pause");
	return 0;
}
/* auto x = 5; 5��const int����,������int����, ���Ƶ�������int����,const��ʧ
 ** auto������ָʾ��(type-specifier) ���Ǵ洢����ָʾ��(storage-class-specifiers)eg:static,register,mutable
 cv�޶���:(cv-qualifier)const_volatile
 auto���const,����,����ȥ��  eg: auto f = e;(����e�� int&���͵�) ���Ƶ�����fֻ��int���͵�
 ��auto& ����auto* constʱ,����ȥ����������
 */
