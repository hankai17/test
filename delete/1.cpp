/*************************************************************************
  > File Name: 1.cpp
  > Author: guaz
  > Mail: hankai17@126.com 
  > Created Time: 2017-02-24 09:43:25
 ************************************************************************/

#include<iostream>
using namespace std;

struct S
{
    public:
        ~S()
        {
            if(q)
            {
                delete q;
                cout<<"s del"<<endl;
            }
                cout<<"s del"<<endl;
        }
    public:
        char *q;
};

class T
{
    public:
        T()
        {
            age = 10;
            p = new char[10];
            s = new S;
        }
        ~T()
        {
            cout<<"delete begin"<<endl;
            if(p)
            {
                delete [] p;
            }
            if(s)
            {
                delete s;
            }
            cout<<"delete over"<<endl;
        }
    public:
        int age;
        char* p;
        S* s;
};

int main()
{
    T* t = new T();
    cout<<t->age<<endl;
    delete t;
    return 0;
}

