#include<stdio.h>
#include<stdlib.h>
#include<string.h>

unsigned int getUnsignedInt(char *buffer, int offset)
{
	printf("buffer0=%c\n",buffer[0]);
	printf("buffer1=%c\n",buffer[1]);
	printf("buffer2=%c\n",buffer[2]);
	printf("buffer3=%c\n",buffer[3]);

	printf("buffer0=%d %x\n",(buffer[offset]) & 0x000000FF,(buffer[offset]) & 0x000000FF);	
	printf("buffer1=%d %x\n",(buffer[offset+1]<<8) & 0x0000FF00,(buffer[offset+1]<<8) & 0x0000FF00);	
	printf("buffer2=%d %x\n",(buffer[offset+2]<<16) & 0x00FF0000,(buffer[offset+2]<<16) & 0x00FF0000);	
	printf("buffer3=%d %x\n",(buffer[offset+3]<<24) & 0xFF000000,(buffer[offset+3]<<24) & 0xFF000000);	
    return (
        ((buffer[offset  ]) & 0x000000FF) | 
        ((buffer[offset+1] <<  8) & 0x0000FF00) | 
        ((buffer[offset+2] << 16) & 0x00FF0000) |
        ((buffer[offset+3] << 24) & 0xFF000000)
    );
}

int main()
{
	char* buf = NULL;
	buf = (char*) malloc ((char)100);
	strcpy(buf,"11111111");
	printf("buf=%d  %x\n",buf,buf);

	//unsigned int v1 = getUnsignedInt(buf, 0);
	unsigned int v1 = getUnsignedInt(buf, 4);
	printf("v1=%d %x\n",v1,v1);
	printf("1 = %d\n",'1');
	return 0;
}
