#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"CircleLinkList.h"

//����m���Ǹ��˳��� ������һ�����ִ�1��ʼ���� ����m���Ǹ����ֳ���

typedef 
struct _PERSON {
  CircleLinkNode node;
  int ID;
}Person;

void MyPrint(CircleLinkNode* data) {
  Person* p = (Person*)data;
  printf("%d ",p->ID);
}

int MyCompare(CircleLinkNode* data1, CircleLinkNode* data2) {
  Person* p1 = (Person*)data1;
  Person* p2 = (Person*)data2;
  return p1->ID == p2->ID;
}

int main() {
  int n = 8;
  int m = 3;
  //��ʼ��ѭ������
  CircleLinkList* clist = Init_CircleLinkList();
  //��������
  int i = 0;
  for (; i < n;i++) {
	Person* p = (Person*)malloc(sizeof(Person));
	p->ID = i + 1;
	//��������
	PushBack_CircleLinkList(clist, (CircleLinkNode*)p); //personǿתΪԭʼ�ڵ�ָ��
  }
  //��ӡ����
  Print_CircleLinkList(clist, MyPrint); printf("\n");

  CLinkList* list = (CLinkList*)clist;
  CircleLinkNode* pCurrent = list->header.next;
  int index = 1;
  while (Size_CircleLinkList(clist) > 1) {
	if (index == m) { //ȥ������
	  //�����ǰID
	  printf("%d ", ((Person*)pCurrent)->ID);
	  //��������ɾ����ǰID
	  RemoveByVal_CircleLinkList(clist, pCurrent, MyCompare); //ȥ��pCurrent
	  //������һ���ڵ�
	  CircleLinkNode* pNext = pCurrent->next; //���¿�ʼ�� ����1
	  free(pCurrent);
	  pCurrent = pNext;
	  if (pCurrent == &(list->header)) {
		pCurrent = pCurrent->next;
	  }
	  index = 1;
	}
	pCurrent = pCurrent->next;
	if (pCurrent == &(list->header)) {
	  pCurrent = pCurrent->next;
	}
	index++;
  }

  //������ʣ�����һ��
  Person* pLast = (Person*)Get_CircleLinkList(clist, 0);
  printf("%d\n", pLast->ID);
  free(pLast);
  //��������
  Destroy_CircleLinkList(clist);
  return 0;
}