
#include <iostream>
#include <string.h>
#include <stdlib.h>

typedef struct{
    size_t len;
    char * data;
}icr_struct_t;

int main1()
{
    char *p=new char[10];
    memcpy(p,"abcdef",strlen("abcdef")+1);
    std::cout<<p<<std::endl;

    char **q=new char* [2];

    q[0]=new char[10];
    memcpy(q[0],"q000000",strlen("q000000")+1);
    q[1]=new char[10];
    memcpy(q[1],"q100000",strlen("q000000")+1);

    std::cout<<q[0]<<std::endl;
    std::cout<<q[1]<<std::endl;

    return 0;
}

int main()
{
   icr_struct_t ** urikey_black_array; 
   urikey_black_array=new icr_struct_t * [2];
   urikey_black_array[0]->data= new char [1024];
   //strcpy(urikey_black_array[0]->data,"abcdefg",strlen("abcdefg")+1);
   strcpy(urikey_black_array[0]->data,"abcdefg");
   urikey_black_array[0]->len=strlen("abcdefg")+1;
   urikey_black_array[0]->
    icr_struct_t a;
    a.Test::a

    
    main1();
   
    std::cout<<"data:"<<urikey_black_array[0]->data<<std::endl;
    urikey_black_array[0]

    return 0;
}
