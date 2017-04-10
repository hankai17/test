#include "observer.hh"

struct st
{
	int a, b;
	void print(int a, int b)
	{
		cout<< a <<" "<< b << endl;
	}
};

void print(int a, int b)
{
	cout<< a <<" "<< b << endl;
}

int main()
{
	Events<std::function<void(int,int)> > myevent;

	auto key = myevent.Connect(print);
	st t;
	auto lambdakey = myevent.Connect([&t](int a, int b)
	{
		t.a = a;
		t.b = b;
	});

	std::function<void(int,int)> f = std::bind(&st::print, &t, std::placeholders::_1,std::placeholders::_2);
	myevent.Connect(f);
	int a = 1, b = 2;
	myevent.Notify(a, b);
	myevent.Disconnect(key);
	return 0;
}
	
