#include<iostream>
#include<stdlib.h>
#include<stack>

struct list_node {
  int data;
  struct list_node* next;
};

void showlist_node(list_node* head) {
  if (head == NULL) {
	std::cout<<"null !"<<std::endl;
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

void* remove_by_value_me(list_node* head, int n) {
  if(head == NULL) {
	return head;
  }
  list_node* cur = head;
  while(cur->data == n) {
	cur = cur->next;
  }
  list_node* new_head = cur; //还是那种错误 
  list_node* next = cur->next;
  while(next != NULL) {
	if(next->data == n) {
	  cur = next->next;
	  next = cur->next;
	} else {
	  next = next->next;
	  cur = next;
	}
  }
  return new_head;
}

void* remove_by_value(list_node* head, int n) {
  if(head == NULL) {
	return head;
  }
  list_node* tmp = head;
  while(tmp != NULL) {
	if(tmp->data == n) {
	  tmp = tmp->next;
	} else {
	  break;
	}
  }
  list_node* pre = tmp;
  list_node* cur = tmp;
  while(cur != NULL) {
	if (cur->data == n) {
	  pre->next = cur->next;
	} else {
	  pre = cur;
	}
	cur = cur->next;
  }
  return tmp;
}

void* remove_dup(list_node* head) {
  if(head == NULL) {
	return head;
  }

  list_node* cur = head;
  list_node* next = head;
  while(next != NULL) {
	int n = cur->data;
	next = (list_node*)remove_by_value(cur->next, n);
	cur->next = next;

	cur = next;
  }
  return head;
}

int main() {
  list_node head, node1, node2, node3, node4, node5;
  head.data = 2;
  node1.data = 2;
  node2.data = 2;
  node3.data = 2;
  node4.data = 2;
  node5.data = 2;

  head.next = &node1;
  node1.next = &node2;
  node2.next = &node3;
  node3.next = &node4;
  node4.next = &node5;
  node5.next = NULL;

  showlist_node(&head);
  //list_node* tmp = (list_node*)remove_by_value(&head, 2);
  list_node* tmp = (list_node*)remove_dup(&head);
  showlist_node(tmp);

  return 0;
}
