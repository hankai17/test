/*************************************************************************
	> File Name: 02init_list.cpp
	> Author: guaz
	> Mail: hankai17@126.com 
	> Created Time: 2017-02-02 00:12:50
 ************************************************************************/

#include<iostream>
#include<vector>

class my_num
{
    //c11允许构造函数和其他函数 的参数为初始化列表
    public:
        my_num(const std::initializer_list<int>& v) {
            for(auto it : v) {
                vec.push_back(it);
            }
        }

        void print() {
            for(const auto& it : vec) {
                std::cout<<it<<" ";
            }
        }
    private:
        std::vector<int> vec;
};

int main()
{
    int i; //undefined value
    int j {}; //0
    int *p;
    int *q {};  //nullptr

    int x(3.14);  //x become 3
    //int y {3.14};  //this is error! uniform init not support narrowing. 


    my_num m = { 1, 2, 3, 4, 5 };
    m.print();

    return 0;
}

