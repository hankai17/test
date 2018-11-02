#include<iostream>
#include<vector>

int main() {
  for ( int i : { 1, 2, 3, 4, 5, 6 } ) {
	std::cout<< i <<std::endl;
  }

  std::vector<int> v { 1, 2, 3, 4, 5, 6 };
  for (auto& elem : v ) {  
	//for ( auto elem : v ) { //拷贝成临时变量
	elem *= 3;
  }

  //for (auto elem : v) {  //效率低
  for (const auto& elem : v ) {  // 为什么是const ref 防止调拷贝构造和析构 
	std::cout<<elem<<" ";
  }
  return 0;
}

