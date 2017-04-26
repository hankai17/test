#include <stdio.h>
#include "ipip.h"

int main() 
{
    init("17monipdb.dat");
    //char *ip = "20.108.249.250";
    char *ip = "125.39.240.113";
    char result[128];
    find(ip, result);
    printf("%s\n",result);
    destroy();

    return 0;
}
