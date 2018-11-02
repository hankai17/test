#include<iostream>
#include<stdlib.h>

using namespace std;

struct ListNode {
  int data;
  struct ListNode* next;
  struct ListNode* pre;
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

//比我写的简练
ListNode* rever(ListNode* head) {
  if (head == NULL) {
    return NULL;
  }
  ListNode* pre = NULL;
  ListNode* next;
  while (head!= NULL) {
    next = head->next;

    head->next = pre;
    head->pre = next;

    pre = head;
    head = next;
  }
  return pre;
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
  head.pre = NULL;

  node1.next = &node2;
  node1.pre = &head;

  node2.next = &node3;
  node2.pre = &node1;

  node3.next = &node4;
  node3.pre = &node2;

  node4.next = &node5;
  node4.pre = &node3;

  node5.next = &node6;
  node5.pre = &node4;

  node6.next = &node7;
  node6.pre = &node5;

  node7.next = NULL;
  node7.pre = &node6;

  ShowListNode(&head);
  std::cout<<std::endl;

  ListNode* tmp = rever(&head);
  ShowListNode(tmp);
  return 0;
}
