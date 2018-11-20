#ifndef LINKSTACK_H
#define LINKSTACK_H

#include<stdlib.h>

#ifdef __cplusplus
extern "C"{
#endif

	//ջ�Ľڵ�
	typedef struct _STACKNODE{
		struct _STACKNODE* next;
	}StackNode;


	//ջ�ṹ��
	typedef struct _LINKSTACK{
		StackNode* head; //ָ�������ĵ�һ���ڵ�
		int size;
	}LStack;

	typedef void* LinkStack;

	//��ʼ��
	LinkStack Init_LinkStack();
	//ѹջ
	void Push_LinkStack(LinkStack stack, void* data);
	//��ջ
	void Pop_LinkStack(LinkStack stack);
	//���ջ��Ԫ��
	void* Top_LinkStack(LinkStack stack);
	//��С
	int Size_LinkStack(LinkStack stack);
	//����
	void Destroy_LinkStack(LinkStack stack);

#ifdef __cplusplus
	}
#endif

#endif