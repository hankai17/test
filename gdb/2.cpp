/*************************************************************************
	> File Name: 2.cpp
	> Author: Bill
	> Mail: hankai17@126.com 
	> Created Time: 2017-01-17 18:14:20
 ************************************************************************/

#include <iostream>
#include <unistd.h>
#include <pthread.h>

void processA();
void processB();
void * processAworker(void *arg);

int main(int argc, const char *argv[])
{
  int pid;

  pid = fork();

  if(pid != 0)
    processA(); //parent
  else
    processB(); //child

  return 0;
}

void processA()
{
  pid_t pid = getpid();
  char prefix[] = "ProcessA: "; char tprefix[] = "thread ";
  int tstatus;
  pthread_t pt;

  printf("%s%lu %s\n", prefix, pid, "step1");

  tstatus = pthread_create(&pt, NULL, processAworker, NULL);
  if( tstatus != 0 )
    {
    printf("ProcessA: Can not create new thread.");
    }
 
  processAworker(NULL);
  sleep(1);
}

void * processAworker(void *arg)
  {
  pid_t pid = getpid();
  pthread_t tid = pthread_self();
  char prefix[] = "ProcessA: ";
  char tprefix[] = "thread ";

  printf("%s%lu %s%lu %s\n", prefix, pid, tprefix, tid, "step2");
  printf("%s%lu %s%lu %s\n", prefix, pid, tprefix, tid, "step3");

  return NULL;
  }

void processB()
  {
  pid_t pid = getpid();
  char prefix[] = "ProcessB: ";
  printf("%s%lu %s\n", prefix, pid, "step1");
  printf("%s%lu %s\n", prefix, pid, "step2");
  printf("%s%lu %s\n", prefix, pid, "step3");

  }



