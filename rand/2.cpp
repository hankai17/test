/*************************************************************************
  > File Name: 2.c
  > Author: guaz
  > Mail: hankai17@126.com 
  > Created Time: 2017-03-02 13:00:38
 ************************************************************************/

#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<time.h>
using namespace std;
int main()
{
    int a[10];
    for(int i=0;i<10;i++)
    {
        a[i]=i+1;
    }

    printf("before: "); 
    for(int i=0;i<10;i++)
    {
        printf("%d ",a[i]);
    }

    srand((int)time(NULL));
    for(int i=0;i<10;i++)
    {
        swap(a[i],a[rand()%7]);
    }

    printf("\nafter "); 
    for(int i=0;i<10;i++)
    {
        printf("%d ",a[i]);
    }
    printf("\n");
    return 0;
}

