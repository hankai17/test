#include <iostream>
#include <functional>

void call_when_even(int x, const std::function<void(int)>& f) {
  if (!(x & 1)) { //x % 2 == 0
	f(x);
  }
}

void output(int x) {
  std::cout << x << " ";
}

void output_add_2(int x) {
  std::cout << x + 2 << " ";
}

int main(void)
{
  {
	auto fr = std::bind(output, std::placeholders::_1);
	for (int i = 0; i < 10; ++i) {
	  call_when_even(i, fr);
	}
	std::cout << std::endl;
  }

  {
	auto fr = std::bind(output_add_2, std::placeholders::_1);
	for (int i = 0; i < 10; ++i) {
	  call_when_even(i, fr);
	}
	std::cout << std::endl;
  }

  return 0;
}
//bind可将 可调用对象与参数绑定 (绑定后仍是可调用对象，只是有参数而已) 再用函数包装器保存 调用的时候直接调用函数包装器即可



class Foo
{
void doit();
};
shared_ptr<Foo> pFoo(new Foo);
boost::function<void()> func = boost::bind(&Foo::doit, pFoo); // long life foo

//这里func对象持有了shared_ptr<Foo>的一份拷贝 对象的生命期就不会短于boost::function对象  有可能会在不经意间延长倒数第二行创建的Foo对象的生命期 

