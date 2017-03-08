/*************************************************************************
  > File Name: test.cpp
  > Author: guaz
  > Mail: hankai17@126.com 
  > Created Time: 2017-02-22 17:28:52
 ************************************************************************/

#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<gperftools/profiler.h>
using namespace std;
int loopop()
{
    int n = 0;
    for(int i=0; i<1000; i++)
    {
        for(int j=0; j<1000; j++)
        {
            n |= i%100 + j/100;
        }
    }
    return n;
}

int main(int argc,char** argv)
{
    //ProfilerStart("google.log");
    char program[1024] = {0};
    snprintf(program,1023,"%s_%d.prof",argv[0],getpid());
    ProfilerStart(program);
    printf("result: %d\n",(loopop()));
    ProfilerStop();

    return 0;
}

/*

   int *fun(int n)
   {
   int *p1 = new int[n];
   int *p2 = new int[n];
   sleep(1);
   return p2;
   }

   int main()
   {
   int n;
//scanf("%d",&n);
ProfilerStart("google.log");
int *p = fun(4);
ProfilerStop();

delete [] p;

sleep(1);
cout<<"hello google"<<endl;
int a = 100;
a += 1;
cout<<" "<<endl;
sleep(1);
cout<<"over"<<endl;
return 0;
}
*/

