#include<iostream>
#include<stdlib.h>
#include<stack>

struct list_node {
  int data;
  struct list_node* next;
};

void showlist_node(list_node* head) {
  if (head == NULL) {
    return;
  }
  list_node* pCurrent = head; //链表从第0个开始
  //list_node* pCurrent = head->next; //当面试写复杂的 翻转等操作时候 链表设计别从第一个开始 
  std::cout << pCurrent->data << " ";
  pCurrent = pCurrent->next;
  while (pCurrent != head) {
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

void* remove_by_value(list_node* head, int n) {
  if(head == NULL) {
    return head;
  }
  list_node* tmp = head;
  while(tmp != NULL) { //细心点
    if(tmp->data == n) {
      tmp = tmp->next;
    } else {
      break;
    }
  }
  list_node* pre = tmp; //别犯那种错误了
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

void* insert_clist(list_node* head, list_node* node) {
  if(head == NULL) {
    return NULL;
  }
  list_node* cur = head->next;
  list_node* pre = head;
  if(cur == head) {
    std::cout<<"at last two node\n";
    return NULL;
  }
  while(cur != head) {
    if(pre->data < node->data && cur->data >= node->data) {
      pre->next = node;
      node->next = cur;
      return head;
      //break;
    }
    pre = cur;
    cur = cur->next;
  }
/*
  if(node->data > cur->data) {
    pre->next = node;
    node->next = head;
    return head;
  } else {
    pre->next = node;
    node->next = cur;
    return node;
    //return head->data > node->data ? node : head;
  }
  */
    pre->next = node;
    node->next = head;
    return head->data > node->data ? node : head;
}

int main() {
  list_node head, node1, node2, node3, node4;
  head.data = 0;
  node1.data = 1;
  node2.data = 4;
  node3.data = 6;
  node4.data = 9;

  head.next = &node1;
  node1.next = &node2;
  node2.next = &node3;
  node3.next = &node4;
  node4.next = &head;

  showlist_node(&head);
  list_node num;
  num.data = -40;
  //num.data = 8;
  //num.data = 12;
  list_node* tmp = (list_node*)insert_clist(&head, &num);
  showlist_node(tmp);

  return 0;
}
