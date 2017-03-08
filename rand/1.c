/*************************************************************************
  > File Name: 1.c
  > Author: guaz
  > Mail: hankai17@126.com 
  > Created Time: 2017-03-02 12:43:22
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

int main()
{
    int a[] = {3,5,7,1,4};
    //int a[] = {3,5};
    int n= sizeof(a)/sizeof(a[0]);
    int i,j,tmp,T=2;

    srand((time(NULL)));
    while(T--)
    {
        i=rand()%n;
        j=rand()%n;
        tmp=a[i];
        a[i]=a[j];
        a[j]=tmp;
    }

    for(i=0;i<n;i++)
    printf("%d ",a[i]);
    printf("\n");
    return 0;
}

