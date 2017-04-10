/*************************************************************************
	> File Name: 03range_based_for.cpp
	> Author: guaz
	> Mail: hankai17@126.com 
	> Created Time: 2017-02-02 00:39:57
 ************************************************************************/

#include<iostream>
#include<vector>

int main()
{
    for ( int i : { 1, 2, 3, 4, 5, 6 } ) {
        std::cout<< i <<std::endl;
    }

    std::vector<int> v { 1, 2, 3, 4, 5, 6 };
    for (auto& elem : v ) {  
    //for ( auto elem : v ) { //这种取元素的方法是 一个个拷贝成临时变量 , 具体可参考书
        elem *= 3;
    }

    //for (auto elem : v) {  //这种打印效率最低
    for (const auto& elem : v ) {  // 为什么是const ref？ 防止调拷贝构造和析构 
        std::cout<<elem<<" ";
    }

    return 0;
}

