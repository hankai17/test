#include "mtasker.hh"
#include <iostream>

using namespace std;

MTasker<> MT;

void printer(void *p)
{
  char c=(char)p;
  for(;;) {
    cout<<c<<endl;
    MT.yield();
  }

}


int main()
{
  MT.makeThread(printer,(void*)'a');
  MT.makeThread(printer,(void*)'b');

  for(;;) {
    while(MT.schedule()); // do everything we can do
  }
}
