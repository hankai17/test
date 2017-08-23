#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "pcre.h"
#include <iostream>
#include <string>
using namespace std;

#define OVECCOUNT 30 /* should be a multiple of 3 */
#define EBUFLEN 128
#define BUFLEN 1024

int main()
{
	pcre *re;
	const char *error;
	int erroffset;
	int ovector[OVECCOUNT];
	int rc, i;

	char buffer[128];
	memset(buffer,'\0',128);

	//char src [] = "<head><title>Hello World</title></head>";
	//char src[] = "http://27.221.83.109/youku/6976FE28DA14883F24ECAB5ED4/03000801005948C326206207F4CE63BD1481DD-0ABB-7EF4-DFC5-D136DFA1091D.mp4?sid=09099";
	char src[] = "http://1.2.3.4/youku/6976FE28DA14883F24ECAB5ED4/03000801005948C326206207F4CE63BD1481DD-0ABB-7EF4-DFC5-D136DFA1091D.mp4?sid=09sasdf";
    //http://119.188.38.109/youku/6975C6985254283163F9B76B34/0300080100595F8240B10B3308C414648B740B-A170-3C09-B46E-589931EF01EA.mp4?sid=05034846269831210458c&ctype=12&ccode=0401&duration=195&expire=18000&psid=e6aaee3f57eb868afde7c4c1a42436ec&ups_client_netip=123.120.160.63&ups_ts=1503484626&ups_userid=&utid=8uEzEZvS1ksCAXt4sEu7jkpw&vid=XMjg3NTY1MTYwMA%3D%3D&vkey=A4bd9a53c61ed5015eeb6677ae0290f40&nk=411390035197_25058077435&ns=2187496_22034880&special=true"
    //const char* yk_key = "http://(\\d{1,3}\\.){3}\\d{1,3}/youku/[^/]*?/([^\?]+)\?.*";

	//char pattern [] = "<title>(.*)</title>";
	//char pattern[] = "http://(\\d{1,3}\\.){3}\\d{1,3}/youku/[^/]*?/([^\?]*?)";
	char pattern[] = "http://(\\d{1,3}\\.){3}\\d{1,3}/youku/[^/]*?/([^\?]+)\?.*";

	printf("String : %s\n", src);
	printf("Pattern: \"%s\"\n", pattern);

	re = pcre_compile(pattern, 0, &error, &erroffset, NULL);
	if (re == NULL) {
		printf("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return 1;
	}

	rc = pcre_exec(re, NULL, src, strlen(src), 0, 0, ovector, OVECCOUNT);
	if (rc < 0) {
		if (rc == PCRE_ERROR_NOMATCH) printf("Sorry, no match ...\n");
		else printf("Matching error %d\n", rc);
		free(re);
		return 1;
	}

	printf("\nOK, has matched ...\n\n");

	for (i = 0; i < rc; i++)
	{
		char *substring_start = src + ovector[2*i];
		int substring_length = ovector[2*i+1] - ovector[2*i];
		printf("%2d: %.*s\n", i, substring_length, substring_start);
	}

	free(re);
	return 0;
}
