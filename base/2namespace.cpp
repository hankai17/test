
#include <iostream>

namespace{
    int ns_int; 
    char ns_char;
    char *ns_char_ptr;
};

typedef struct Class {
    int a;
    char c;
}C;

typedef struct Test {
    int a;
    char c;
}T;

int main1()
{
   std::cout<<ns_int<<std::endl; 
   std::cout<<ns_char<<std::endl; 
   std::cout<<ns_char_ptr<<std::endl; 
   return 0;
}

int main()
{
    T t1;
    t1.Test::c
   
    C c1;
    c1.
    

    return 0;
}
