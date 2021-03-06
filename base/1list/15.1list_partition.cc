#include<iostream>
#include<stdlib.h>
#include<stack>
#include<vector>

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

void arr_partition(std::vector<list_node*>& arr, int pivot) {
  if(arr.empty()) {
	return;
  }
  int i = 0;
  int big = (int)arr.size();
  int small = -1;
  //while(i < (int)arr.size()) {
  while(i != big) {
	if((arr[i])->data < pivot) {
	  list_node* tmp = arr[++small];
	  arr[++small] = arr[i];
	  arr[i++] = tmp; 
	} else if((arr[i])->data == pivot) {
	  i++;
	} else {
	  list_node* tmp = arr[--big];
	  arr[--big] = arr[i];
	  arr[i] = tmp;
	}
  }
  return;
}

void partition(list_node* head, int pivot) {
  if(head == NULL) {
	return;
  }
  list_node* tmp = head;
  std::vector<list_node*> v; 
  while(tmp != NULL) {
	v.push_back(tmp);
	tmp = tmp->next;
  }
  arr_partition(v, pivot);
  for(std::vector<list_node*>::iterator it = v.begin(); it != v.end(); it++) {
	std::cout<<(*it)->data<<std::endl;
  }
  return;
}

int main()
{
  list_node head;
  list_node node1, node2, node3, node4;
  list_node node5, node6;
  head.data = 9;
  node1.data = 0;
  node2.data = 4;
  node3.data = 5;
  node4.data = 1;
  node5.data = 6;

  head.next = &node1;
  //head.pre = NULL;

  node1.next = &node2;
  //node1.pre = &head;

  node2.next = &node3;
  //node2.pre = &node1;

  node3.next = &node4;
  //node3.pre = &node2;

  node4.next = &node5;
  //node4.pre = &node3;
  //node5.next = &node6;
  node5.next = NULL;

  //showlist_node(&head);
  //std::cout<<((list_node*)middle(&head))->data<<std::endl; 
  //std::cout<<is_palindrome(&head)<<std::endl;
  partition(&head, 4);
  return 0;
}
