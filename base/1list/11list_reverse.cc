#include<iostream>
#include<stdlib.h>
using namespace std;

struct ListNode {
  int data;
  struct ListNode* next;
};

void ShowListNode(ListNode* head) {
  if (head == NULL) {
	return;
  }
  ListNode* pCurrent = head; //链表从第0个开始
  //ListNode* pCurrent = head->next; //当面试写复杂的 翻转等操作时候 链表设计别从第一个开始 
  while (pCurrent != NULL) {
	cout << pCurrent->data << " ";
	pCurrent = pCurrent->next;
  }
}

ListNode* rever(ListNode* h) {
  if (h == NULL) {
	return NULL;
  }
  ListNode* pre = NULL;
  ListNode* head = h;
  ListNode* next = h->next;
  while (next != NULL) {
	head->next = pre;
	pre = head;
	head = next;
	next = next->next;
  }
  head->next = pre;
  return head;
}

//比我写的简练
ListNode* rever1(ListNode* h) {
  if (h == NULL) {
	return NULL;
  }
  ListNode* pre = NULL;
  ListNode* head = h;
  ListNode* next;
  while (head!= NULL) {
	next = head->next;
	head->next = pre;
	pre = head;
	head = next;
  }
  return pre;
}

/*
   ListNode* rever2(ListNode* h)
   {
   if (h == NULL) {
   return NULL;
   }
   ListNode * next = NULL;
   ListNode *pre = NULL;
   while (h->next != NULL)
   {
   next = h->next;
   h->next = pre;
   pre->next = h->next;
   h->next = next;
   }
   return h;
   }
 */ //BUG???????

//这种方法太傻了 
//看似把三个变量都赋值为非NULL 看似好理解其实不好写
//还是pre初始化null好 从最开头写好
ListNode* ReverseListNode(ListNode* head) {
  if (head == NULL) {
	return NULL;
  }
  ListNode* pPre = head;
  ListNode* pCurrent = head->next;
  ListNode* pNext = pCurrent->next;
  pPre->next = NULL; //这一句不好写
  while (pNext != NULL) {
	pCurrent->next = pPre;
	pPre = pCurrent;
	pCurrent = pNext;
	pNext = pNext->next;
  }
  pCurrent->next = pPre;
  return pCurrent;
}

int main() {
  ListNode head;
  ListNode node1, node2, node3, node4;
  ListNode node5, node6, node7;
  head.data = 0;
  node1.data = 1;
  node2.data = 2;
  node3.data = 4;
  node4.data = 7;
  node5.data = 1;
  node6.data = 9;
  node7.data = 14;

  head.next = &node1;
  node1.next = &node2;
  node2.next = &node3;
  node3.next = &node4;
  node4.next = &node5;
  node5.next = &node6;
  node6.next = &node7;
  node7.next = NULL;

  ShowListNode(&head);
  std::cout<<std::endl;

  //ListNode* tmp = rever(&head);
  //ListNode* tmp = rever1(&head);
  //ListNode* tmp = rever2(&head); //BUG
  ListNode* tmp = ReverseListNode(&head);
  ShowListNode(tmp);
  return 0;
}
