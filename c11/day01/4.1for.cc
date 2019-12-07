#include<iostream>
#include<vector>

int main() {
  for ( int i : { 1, 2, 3, 4, 5, 6 } ) {
	std::cout<< i <<std::endl;
  }

  std::vector<int> v { 1, 2, 3, 4, 5, 6 };
  for (auto& elem : v ) {  //非只读方式 可以修改原来值
	//for ( auto elem : v ) { //拷贝成临时变量 即只读的方式即做任何操作都不会改变原来的值
	elem *= 3;
  }

  //for (auto elem : v) {  //效率低
  for (const auto& elem : v ) {  // 为什么是const ref 防止调拷贝构造和析构 
	std::cout<<elem<<" ";
  }
  return 0;
}

