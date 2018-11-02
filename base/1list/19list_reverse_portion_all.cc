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
  std::cout<<std::endl;
}

//phn模型
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

void* stack_pop(std::stack<list_node*>& s) {
  if((int)s.size() == 0) {
	return NULL;
  }
  list_node* head = NULL;
  list_node* cur = NULL;
  while(!s.empty()) {
	cur = s.top();
	s.pop();

	cur->next = head;
	head = cur;
  }
  return head;
}//写反了

void* stack_pop1(std::stack<list_node*>& s) {
  if((int)s.size() == 0) {
	return NULL;
  }
  list_node* next = NULL;

  list_node* cur = s.top();
  s.pop();

  while(!s.empty()) {
	next = s.top();
	s.pop();

	cur->next = next;
	cur = next;
  }
  return cur;
} //1 2 3  so 1->2->3 return 3

void* resign(std::stack<list_node*>& s,list_node* left, list_node* right) {
  list_node* cur = s.top();
  s.pop();
  if(left != NULL) {
	left->next = cur;
  }

  list_node* next = NULL;
  while(!s.empty()) {
	next = s.top();
	s.pop();

	cur->next = next;
	cur = next;
  }
  cur->next = right;
  return cur;
}

void* rever_by(list_node* head, int k) {
  if(head == NULL || k < 2) {
	return head;
  }
  std::stack<list_node*> s;
  list_node* cur = head;
  list_node* new_head = head;
  list_node* pre = NULL;
  list_node* next = NULL;

  while(cur != NULL) {
	next = cur->next;
	s.push(cur);
	if((int)s.size() == k) { //压满后
	  pre = (list_node*)resign(s, pre, next);
	  new_head = new_head == head ? cur : new_head;
	}
	cur = next;
  }

  return new_head;
}

int main() {
  list_node head, node1, node2, node3, node4, node5, node6, node7, node8;
  head.data = 1;
  node1.data = 2;
  node2.data = 3;
  node3.data = 4;
  node4.data = 5;
  node5.data = 6;
  node6.data = 7;
  node7.data = 8;
  node8.data = 9;

  head.next = &node1;
  node1.next = &node2;
  node2.next = &node3;
  node3.next = &node4;
  node4.next = &node5;
  node5.next = &node6;
  node6.next = &node7;
  node7.next = &node8;
  node8.next = NULL;
  showlist_node(&head);

  list_node* tmp = (list_node*)rever_by(&head, 3);
  showlist_node(tmp);
  return 0;
}
