#include<iostream>
#include<map>
//#include<hash_set>
//#include<hash_map>
//#include<unordered_map>

class list_node {
  public:
	list_node* next;
	list_node* rand;
	int data;
	list_node(int d):data(d) {
	  next = NULL;
	  rand = NULL; 
	}
};

void* copy_list(list_node* l) {
  std::map<list_node*,list_node*> node_hash;
  list_node* cur = l;
  list_node* node;
  while(cur != NULL) { //建立 <旧-新>节点 映射关系
	node = new list_node(cur->data);
	node_hash.insert(std::map<list_node*, list_node*>::value_type(cur, node));
	cur = cur->next;
  }
  cur = l;
  while(cur != NULL) {
	std::map<list_node*, list_node*>::iterator it = node_hash.find(cur);
	if(it != node_hash.end()) {
	  //std::cout<<"data = "<<it->second->data<<std::endl;
	  std::map<list_node*,list_node*>::iterator it1 = node_hash.find(cur->rand);
	  if(it1 != node_hash.end()) {
		it->second->rand = it1->second;
		if(cur->rand == NULL) 
		it->second->rand = NULL;
	  }
	  std::map<list_node*,list_node*>::iterator it2 = node_hash.find(cur->next);
	  if(it2 != node_hash.end()) {
		it->second->next = it2->second;
		if(cur->next == NULL) 
		it->second->next = NULL;
	  }
	}
	cur = cur->next;
  }
  std::map<list_node*,list_node*>::iterator it3 = node_hash.find(l);
  return it3->second;
}

int main() {
  list_node l1(1);
  list_node l2(2);
  list_node l3(3);

  l1.rand = &l3;
  l1.next = &l2;

  l2.rand = NULL;
  l2.next = &l3;

  l3.rand = &l1;
  l3.next = NULL;

  list_node* l = (list_node*)copy_list(&l1);
  while(l != NULL) {
	if(l->rand != NULL) {
	  std::cout<<l->data<<"-"<<l->rand->data<<std::endl;
	} else {
	  std::cout<<l->data<<"-"<<std::endl;
	}
	l = l->next;
  }
  //delete l!!!

  return 0;
}

// l1  l2  l3
// l3  nul l1
