/*************************************************************************
	> File Name: 2.cpp
	> Author: guaz
	> Mail: hankai17@126.com 
	> Created Time: 2017-03-01 16:47:25
 ************************************************************************/
#include<stdio.h>
#include<iostream>
using namespace std;

int main()
{
    double sec = 1234;
    double usec = 1234;
    double d_val = sec*1000 + usec/1000.0;
    printf("d_val=%f\n",d_val);//cout<<"d_val="<<d_val<<endl;

    double res;
    if(d_val > 1000.0)
    {
        printf("res=%f\n",res);
    }

    return 0;
}
//SB
