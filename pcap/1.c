#include<stdio.h>
#include<pcap.h>

int main()
{
    char errbuf[1024];
    char *dev;
    dev=pcap_lookupdev(errbuf);    
    if(dev==NULL)
    {
        fprintf(stderr,"coundn't find default device: %s \n",errbuf);
        return 2;
    }
    printf("Device: %s \n",dev);
    return 0;
}
