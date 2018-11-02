#include<iostream>
#include<stdlib.h>
using namespace std;

struct list_node {
  int data;
  struct list_node* next;
};

list_node* merge_recursive(list_node* head1, list_node* head2) {
  if (NULL == head1) { //怎么觉着不是很严谨
	return head2;
  }
  if (NULL == head2) {
	return head1;
  }
  list_node* head = NULL;

  if (head1->data < head2->data) {
	head = head1;
	head->next = merge_recursive(head1->next, head2);
  } else {
	head = head2;
	head->next = merge_recursive(head1, head2->next);
  }
  return head;
}

list_node * merage(list_node* head1, list_node *head2) {
  //判断是否为空
  if (NULL == head1) {
	return head2;
  } 
  if (NULL == head2) {
	return head1;
  }

  list_node* head = NULL;
  list_node* p1 = NULL;
  list_node* p2 = NULL;
  if (head1->next->data < head2->next->data) {
	head = head1;
	p1 = head1->next->next;
	p2 = head2->next;
  } else {
	head = head2;
	p1 = head2->next->next;
	p2 = head1->next;
  }
  //大前提，p1 p2 都不为空
  list_node * pcur = head->next;
  while (p1 != NULL && p2 != NULL) {
	if (p1->data < p2->data) {
	  pcur->next = p1;
	  pcur = p1;
	  p1 = p1->next;
	} else {
	  pcur->next = p2;
	  pcur = p2;
	  p2 = p2->next;
	}
  }
  //跳出循环。1 p1 == NULL 2 p2 == NULL 
  if (NULL == p1) {
	pcur->next = p2;
  }
  if (NULL == p2) {
	pcur->next = p1;
  }
  return head;
}

list_node* mymerge(list_node* pHead1, list_node* pHead2) {
  if (pHead1 == NULL || pHead2 == NULL) {
	return NULL;
  }
  list_node* pHead = (list_node*)malloc(sizeof(list_node));
  list_node* pCurrent;
  list_node* pCurrent1 = pHead1->next;
  list_node* pCurrent2 = pHead2->next;

  /*确定头节点*/
  if (pCurrent1->data < pCurrent2->data) {
	pHead = pHead1;
	pCurrent = pHead1->next;
	pCurrent1 = pCurrent1->next;
  } else {
	pHead = pHead2;
	pCurrent = pHead2->next;
	pCurrent2 = pCurrent2->next;
  }
  /*构造出一种结构 pCurrent指向排好的序列，pCurrent1,2是后面的待比较序列*/
  while (pCurrent2 != NULL && pCurrent1 != NULL) {
	if (pCurrent1->data < pCurrent2->data) {
	  pCurrent->next = pCurrent1;
	  pCurrent = pCurrent1;
	  pCurrent1 = pCurrent1->next;
	} else {
	  pCurrent->next = pCurrent2;
	  pCurrent = pCurrent2;
	  pCurrent2 = pCurrent2->next;
	}
  }

  if (pCurrent2 == NULL) {
	pCurrent->next = pCurrent1;
  }
  if (pCurrent1 == NULL) {
	pCurrent->next = pCurrent2;
  }
  return pHead;
}

void print_listnode(list_node*p) {
  if (p == NULL) {
	return;
  }
  //list_node*pCurrent = p->next;
  list_node*pCurrent = p;
  while (pCurrent != NULL) {
	cout << pCurrent->data << " ";
	pCurrent = pCurrent->next;
  }
  return;
}

int main() {
  list_node head1, head2;
  list_node node1, node2, node3, node4;
  list_node node5, node6, node7;
  head1.data = 0;
  head2.data = 0;
  node1.data = 1;
  node2.data = 2;
  node3.data = 4;
  node4.data = 7;
  node5.data = 1;
  node6.data = 9;
  node7.data = 14;

  head1.next = &node1;
  node1.next = &node2;
  node2.next = &node3;
  node3.next = &node4;
  node4.next = NULL;

  head2.next = &node5;
  node5.next = &node6;
  node6.next = &node7;
  node7.next = NULL;

  print_listnode(&head1);
  cout << endl;
  print_listnode(&head2);
  cout << endl;
  //list_node* tmp=mymerge(&head1, &head2);
  //list_node* tmp = merage(&head1, &head2);
  list_node*tmp = merge_recursive(&head1, &head2);
  print_listnode(tmp);

  return 0;
}


