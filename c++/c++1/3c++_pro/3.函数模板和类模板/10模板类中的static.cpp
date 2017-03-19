#include <iostream>
using namespace std;

template <typename T>
class AA
{
public:
	static T m_a;
protected:
private:

};

template <typename T>
T AA<T>::m_a = 0;

void main()
{
	AA<int> a1, a2, a3;
	a1.m_a = 10;
	a2.m_a++;
	a3.m_a++;
	cout << AA<int>::m_a << endl;//12

	AA<char> b1, b2, b3;
	b1.m_a = 'a';
	b2.m_a++;
	b3.m_a++;
	cout << AA<char>::m_a << endl;//c

	system("pause");
	return;
}