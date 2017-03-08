#define  _CRT_SECURE_NO_WARNINGS 
#include <iostream>

//高内聚  即单一性原则
//低耦合 模块间交互量越少越好
using namespace std;


#if 0
class Clothes
{
public:
	void shopping() {
		cout << "休闲的服装" << endl;
	}
	void working() {
		cout << "休闲的服装" << endl;
	}
};
#endif

class ClothesShpping
{
public:
	void style() {
		cout << "休闲的服装" << endl;
	}
};
class ClothesWorking
{
public:
	void style() {
		cout << "休闲的服装" << endl;
	}
};

int main(void)
{
#if 0
	Clothes c1;
	c1.shopping();

	c1.shopping();
#endif
	ClothesShpping cs; 
	cs.style();

	ClothesWorking cw;
	cw.style();
	
	return 0;
}
