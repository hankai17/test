#include<iostream>
#include<vector>
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
  while (pCurrent != NULL)
  {
	cout << pCurrent->data << " ";
	pCurrent = pCurrent->next;
  }
}
//from to 是从1开始的
ListNode* rever_partion(ListNode* h, int from, int to) {
  int len = 0;
  ListNode* node1 = h;
  ListNode* fpre = NULL;
  ListNode* tpos = NULL;
  while(node1 != NULL) {
	len++;
	fpre = (len == from - 1) ? node1 : fpre;
	tpos = (len == to + 1) ? node1 : tpos;
	node1 = node1->next;
  }
  //  fpre   cur    ...   to
  //        node1  node2
  if (h == NULL || from < 1 || to > len || from > to) {
	return NULL;
  }
  node1 = (fpre == NULL) ? h : fpre->next;
  ListNode* node2 = node1->next;
  node1->next = tpos;
  ListNode* next = NULL;

  //while(node2 != NULL) {
  while(node2 != tpos) {
	next = node2->next;
	node2->next = node1;
	node1 = node2;
	node2 = next;
  }

  if(fpre != NULL) {
	fpre->next = node1;
	return h;
  }
  return node1;
}

//2 1 3 4
void get_from_to(ListNode* h, int* f, int* t) {
  //assert
  ListNode* tmp = h;
  ListNode* next = h->next;
  int i = 0;
  while(next != NULL) {
  	i++;
    if(next->data < tmp->data) {
    	*f = i;
        break;
    } 
   
    tmp = next;
     next = next->next;
  }
  //cout<<"f="<<*f<<endl;
  
    ListNode* tmp1 = h;
  ListNode* next1 = h->next;
  int j = 0;
  while(next1 != NULL) {
  	j++;
    if(next1->data > tmp1->data) {
      *t = j;
      if(*t >= *f)  break;
    } 
   
    tmp1 = next1;
     next1 = next1->next;
  }
   //cout<<"t="<<*t<<endl;
}
int main() {
  ListNode head, node1, node2, node3, node4, node5, node6, node7;
  head.data = 2;
  node1.data = 1;
  node2.data = 3;
  node3.data = 4;

  head.next = &node1;
  node1.next = &node2;
  node2.next = &node3;
  node3.next = NULL;

  ShowListNode(&head);
  std::cout<<"-----------"<<std::endl;
  ListNode* tmp = rever_partion(&head, 1, 2);
  //rever_partion1(&head, 2, 4);
  ShowListNode(tmp);
  return 0;
}


int main1() {
  int num, tmp1;
/*
  cin>>num;

  ListNode head;
  cin >> head.data;
  ListNode* pre = &head;
  for(int i = 1; i < num; i++) {
  	ListNode* n = new(ListNode);
    cin >> n->data;
    pre->next = n;
    pre = n;
  }
  pre->next = NULL;
 */ 
  ListNode head;
  ListNode d1;
  ListNode d2;
  ListNode d3;

  head.data = 2;
  d1.data = 1;
  d2.data = 3;
  d3.data = 4;

  head.next = &d1;
  d1.next = &d2;
  d2.next = &d3;
  d3.next = NULL;
  
  ShowListNode(&head);
  std::cout<<"-----------"<<std::endl;
  
  //get from and to
  //int from, to;
  //get_from_to(&head, &from, &to);
  //  cout<<"f="<<from<<" to="<<to<<endl;
   
  ListNode* tmp = rever_partion(&head, 1, 2);

  ShowListNode(&head);
  
  return 0;
}
