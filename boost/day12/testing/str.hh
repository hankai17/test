#ifndef _MYSTRING
#define _MYSTRING
#include<string.h> 

class mystring { 
  char* buffer; 
  int length;
  public: 
    void setbuffer(char* s) { buffer = s; length = strlen(s); } 
    char& operator[ ] (const int index) { return buffer[index]; }
    int size( ) { return length; }
 }; 
 
#endif
