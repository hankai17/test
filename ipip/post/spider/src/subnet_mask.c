#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>





int cidr(std::string ip_s, std::string ip_e) 
{
    uint32_t ip_start, ip_end;
    struct in_addr addr;
    ip_start = ntohl(inet_addr((char*)ip_s.data()));
    ip_end = ntohl(inet_addr((char*)ip_e.data()));

    uint32_t net = ip_start;
    uint32_t mask;
    int i;
    while ( net <= ip_end ) 
    {
        for(i = 32; i >=0; i--) 
        {
            mask = (i == 0) ? 0 : 0xffffffff << (32 - i);
            if ( (net & mask) != net || (net | ~mask) > ip_end )
            {
                addr.s_addr = htonl(net);
                //printf("%s/%d\n", inet_ntoa(addr), i + 1);
                std::cout<<"%s/"<<inet_ntoa(addr)<<"%d\n"<<i + 1;
                mask = 0xffffffff << (32 - i - 1);
                net = (net | ~mask) + 1;
                break;
            }
        }

        if( i == -1 ) 
        {
            //printf("0.0.0.0/0\n");
            std::cout<<"0.0.0.0/0\n";
            break;
        }
        if( net == 0 ) 
        {
            break;
        }
    }
    return 0;
}



int main (int argc,char *argv[]) 
{
    uint32_t ip_start, ip_end;
    struct in_addr addr;

    ip_start = ntohl(inet_addr(argv[1]));
    ip_end = ntohl(inet_addr(argv[2]));

    uint32_t net = ip_start;
    uint32_t mask;
    int i;
    while ( net <= ip_end ) 
    {
        for(i = 32; i >=0; i--) 
        {
            mask = (i == 0) ? 0 : 0xffffffff << (32 - i);
            if ( (net & mask) != net || (net | ~mask) > ip_end )
            {
                addr.s_addr = htonl(net);
                printf("%s/%d\n", inet_ntoa(addr), i + 1);
                mask = 0xffffffff << (32 - i - 1);
                net = (net | ~mask) + 1;
                break;
            }
        }

        if( i == -1 ) 
        {
            printf("0.0.0.0/0\n");
            break;
        }
        if( net == 0 ) 
        {
            break;
        }
    }
    return 0;
}
