#include "mtasker.hh"
#include <iostream>

using namespace std;

MTasker<> MT;

void menuHandler(void *p)
{
  int num=(int)p;
  cout<<"Key handler for key "<<num<<" launched"<<endl;
 
  MT.waitEvent(num);
  cout<<"Key "<<num<<" was pressed!"<<endl;
}


int main()
{
  char line[10];

  for(int i=0;i<10;++i) 
    MT.makeThread(menuHandler,(void *)i);

  int numWoken;
  for(;;) {
    while(MT.schedule()); // do everything we can do
    if(MT.noProcesses())  // exit if no processes are left
      break;

    if(!fgets(line,9,stdin)) // read a keypress
      break;
    
    numWoken=MT.sendEvent(*line-'0'); // wake up waiters
    cout<<"Woke "<<numWoken<<" tasks"<<endl;
  }
  cout<<"No more tasks running"<<endl;
}
