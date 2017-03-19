#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "seqqueue.h"
#include "seqlist.h"

using namespace std;


void main()
{
	int i = 0;
	int a[10];
	int ret = 0;
	SeqQueue * queue = NULL;
	queue = SeqQueue_Create(10);
	if (queue == NULL)
	{
		return;
	}
	for (i = 0; i < 5; i++)
	{
		a[i] = i + 1;
		ret = SeqQueue_Append(queue, &a[i]);
	}

	printf("len: %d \n", SeqQueue_Length(queue));
	printf("header: %d \n", *((int *)SeqQueue_Header(queue)));
	printf("capacity: %d \n", *((int *)SeqQueue_Capacity(queue)));

	while (SeqQueue_Length(queue)>0)
	{
		int tmp=  *((int *)SeqQueue_Retrieve(queue));
		printf("tmp:%d ", tmp);
	}
	SeqQueue_Destory(queue);



	system("pause");
	return;
}