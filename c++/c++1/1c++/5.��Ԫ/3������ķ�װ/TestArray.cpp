#include <iostream>
using namespace std;
#include "MyArray.h"

void main()
{
	Array a1(10);
	for (int i = 0; i < a1.length(); i++)
	{
		a1.setData(i, i);
	}
	cout << "´òÓ¡a1" << endl;
	for (int i = 0; i < a1.length(); i++)
	{
		cout << a1.getData(i) << endl;
	}

	Array a2 = a1;
	cout << "´òÓ¡a2" << endl;
	for (int i = 0; i < a2.length(); i++)
	{
		cout << a2.getData(i) << endl;
	}


	system("pause");
	return;
}