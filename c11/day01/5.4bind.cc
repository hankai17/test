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
