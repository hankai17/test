/*************************************************************************
  > File Name: 1.cpp
  > Author: guaz
  > Mail: hankai17@126.com 
  > Created Time: 2017-03-06 16:24:41
 ************************************************************************/

#include<iostream>
using namespace std;

class A {
    public:
        long a;
};

class B : public A {
    public:
        long b;
};

//void seta(B* data,int idx) {
void seta(A* data,int idx) {
    data[idx].a = 2;
}

int main()
{
    B data[4];
    for(int i=0; i<4; ++i)
    {
        data[i].a = 1;
        data[i].b = 1;
        seta(data,i);
    }
    for(int i=0; i<4; ++i)
    {
        cout<<data[i].a<<data[i].b<<endl;
    }
    return 0;
}
