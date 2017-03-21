#include<stdio.h>
#include<string.h>

int main()
{
    char a[100] = "asdfsld!dkslsdlf!oiern!qlskf";
    char* s = strtok(a,"!");
    while(s != NULL)
    {
        printf("%s\n",s);
        s = strtok(NULL,"!");
    }
    return 0;
}
