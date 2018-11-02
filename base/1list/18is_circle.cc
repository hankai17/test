#include<iostream>
#include<stdlib.h>
#include<stack>

struct list_node {
  int data;
  struct list_node* next;
  //struct list_node* pre;
};

void showlist_node(list_node* head) {
  if (head == NULL) {
	return;
  }
  list_node* pCurrent = head; //链表从第0个开始
  //list_node* pCurrent = head->next; //当面试写复杂的 翻转等操作时候 链表设计别从第一个开始 
  while (pCurrent != NULL) {
	std::cout << pCurrent->data << " ";
	pCurrent = pCurrent->next;
  }
}

//phn模型: pre head next
list_node* rever(list_node* head) {
  if (head == NULL) {
	return NULL;
  }
  list_node* pre = NULL;
  list_node* next;
  while (head!= NULL) {
	next = head->next;

	head->next = pre;
	//head->pre = next;

	pre = head;
	head = next;
  }
  return pre;
}

void* middle(list_node* head) {
  if(head == NULL) {
	return NULL;
  }
  list_node* mid = head;
  list_node* cur = head;
  while(cur->next != NULL && cur->next->next != NULL) {
	mid = mid->next;
	cur = cur->next->next;
  }
  return mid;
}

bool is_palindrome(list_node* head) {
  if(head == NULL) {
	return true;
  }
  list_node* mid = (list_node*)middle(head);
  list_node* right = mid->next;
  std::stack<list_node*> s;
  while(right != NULL) {
	s.push(right);
	right = right->next;
  }
  while(!s.empty()) {
	list_node* tmp = s.top();
	s.pop();
	if(tmp->data != head->data) {
	  return false;
	}
	head = head->next;
  }
  return true;
}

bool is_circle_me(list_node* head) {
  if(head == NULL) {
	return false;
  }
  list_node* slow = head;
  list_node* fast = head->next;
  while(slow != fast) {
	if(fast->next == NULL || fast->next->next == NULL) {
	  return false;
	}
	slow = slow->next;
	fast = fast->next->next;
  }
  return true;
}
//书上应该有问题
bool is_circle(list_node* head) {
  if(head == NULL || head->next == NULL || head->next->next == NULL) {
	return false;
  }
  list_node* slow = head->next;
  list_node* fast = head->next->next;
  while(slow != fast) {
	if(fast->next == NULL || fast->next->next == NULL) {
	  return false;
	}
	slow = slow->next;
	fast = fast->next->next;
  }
  return true;
}

int main() {
  list_node head, node1, node2, node3, node4;
  head.data = 0;
  node1.data = 1;
  node2.data = 2;
  node3.data = 2;
  node4.data = 1;

  head.next = &node1;
  node1.next = &head;
  //node2.next = &node3;
  //node3.next = &node4;
  //node4.next = NULL:

  std::cout<<is_circle(&head)<<std::endl;
  return 0;
}
