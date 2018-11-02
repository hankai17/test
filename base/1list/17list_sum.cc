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

//phn模型
list_node* rever(list_node* head) {
  if (head == NULL) {
	return NULL;
  }
  list_node* pre = NULL;
  list_node* next;
  while (head != NULL) {
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

void* add_list(list_node* l1, list_node* l2) {
  if(l1 == NULL || l2 == NULL) {
	return 0; //不恰当
  }
  list_node* h1 = rever(l1);
  list_node* h2 = rever(l2);
  int ca = 0;
  list_node* n1 = h1;
  list_node* n2 = h2;
  list_node* node = NULL;
  list_node* pre = NULL; //链表从后往前 也符合相加规律
  int no1 = 0;
  int no2 = 0;
  int no = 0;
  while(n1 != NULL || n2 != NULL) {
	no1 = n1 != NULL ? n1->data : 0;
	no2 = n2 != NULL ? n2->data : 0;
	no = no1 + no2 + ca;

	node = new list_node;
	node->data = no % 10;
	ca = no / 10;

	node->next = pre; //前插
	pre = node; 

	n1 = n1 != NULL ? n1->next : NULL;
	n2 = n2 != NULL ? n2->next : NULL;
  }

  if (ca == 1) {
	node = new list_node;
	node->data = 1;
	node->next = pre;
	pre = node;
  }
  //rever(l1);
  //rever(l2);
  return pre;
}

list_node* sum(list_node* h1, list_node* h2) {
  //err check
  list_node* r1 = rever(h1);
  list_node* r2 = rever(h2);

  int ca = 0;
  list_node* h = NULL;
  std::cout<<"----"<<std::endl;
  int dat1, dat2;
  while(r1 != NULL || r2 != NULL) { //犯错1 边界不清晰
    dat1 = r1 == NULL ? 0 : r1->data; //犯错3 段错误
    dat2 = r2 == NULL ? 0 : r2->data;
	
	list_node* n = new(list_node);
	n->data = (dat1 + dat2 + ca) % 10; //个位数  //犯错2 数学不会了
	ca = (dat1 + dat2 + ca) / 10;
	n->next = h;
	h = n;

    r1 = r1 == NULL ? NULL : r1->next; //犯错4 不移动
    r2 = r2 == NULL ? NULL : r2->next;
  }
  std::cout<<"ca="<<ca<<std::endl;
  if (ca == 1) {
	list_node* n = new(list_node);
	n->data = 1;
	n->next = h;
	h = n;
  }
  return h;
}

int main() {
  list_node head, node1, node2;
  head.data = 9;
  node1.data = 9;
  node2.data = 9;

  head.next = &node1;
  node1.next = &node2;
  node2.next = NULL;

  list_node head1, node11;
  head1.data = 9;
  node11.data = 9;

  head1.next = &node11;
  node11.next = NULL;

  //list_node* tmp = (list_node*)add_list(&head, &head1); //9 3 7 + 6 2 = 999
  list_node* tmp = (list_node*)sum(&head, &head1); //9 3 7 + 6 2 = 999
  showlist_node(tmp);
  return 0;
}

