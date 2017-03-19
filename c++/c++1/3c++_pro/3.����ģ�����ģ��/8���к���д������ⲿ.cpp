#include <iostream>
using namespace std;
//结论 友元函数只有在重载<< >>时候用 其他时候不要滥用
template <typename T>
class Complex
{
	friend Complex MySub(Complex &c1, Complex &c2);
	

	friend ostream & operator << <T>(ostream &out, Complex &c3);//我这个不是bug！！！！
	
public:
	Complex(T a, T b);
	void printCom();
	Complex operator +(Complex &c2);
	Complex operator -(Complex &c2);
protected:
private:
	T a;
	T b;

};
template <typename T>
Complex<T>::Complex(T a, T b)//构造函数写在类的外部
							//1因为是模板函数 加template
							//2因为是类函数 加域名（别忘了参数列表<T>）
{
	this->a = a;
	this->b = b;
}

template <typename T>       //普通成员函数写在类的外部
							//1因为是模板函数 加template
							//2因为是类函数 加域名（别忘了参数列表<T>）
void Complex<T>::printCom()
{
	cout << "a:" << a << "b:" << endl;
}

template <typename T>
Complex<T> Complex<T>:: operator+ (Complex<T> &c2)//返回值也要具体化 参数也要具体化
{
	Complex <T>tmp(a + c2.a , b + c2.b); //里面可加可不加
	return tmp;
}

template <typename T>
Complex<T> Complex<T>:: operator- (Complex<T> &c2)//返回值也要具体化 参数也要具体化
{
	Complex <T> tmp(a - c2.a, b - c2.b); //里面可加可不加
	return tmp;
}

template <typename T>//友元函数也是模板函数  友元函数属于全局函数他不属于这个类 不需要加域作用符
 ostream & operator << (ostream &out, Complex<T> &c3)//友元函数写在外面比较复杂
{
	out << c3.a << " + " << c3.b << "i" << endl;
	return out;
}

 template <typename T>//???????????????????????????????????????
 Complex<T> MySub(Complex <T> &c1, Complex <T> &c2)
 {
	 Complex tmp(c1.a - c2.a, c1.b - c2.b);
	 return tmp;
 }


void main()
{
	Complex<int> c1(1, 2);
	Complex<int> c2(3, 4);

	Complex<int>c3 = c1 + c2;



	system("pause");
	return;
}



