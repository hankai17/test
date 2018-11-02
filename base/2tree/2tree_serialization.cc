#include<iostream>
#include<stack>
#include<stdlib.h>
#include<queue>


#define FLAG_TRUE 1
#define FLAG_FALSE 0

struct tree_node {
  int data;
  struct tree_node* ldata;
  struct tree_node* rdata;
};

void print(tree_node* root) {
  if (root == NULL) {
    return;
  }
  std::stack<tree_node*> s;
  s.push(root);
  while(!s.empty()) {
    root = s.top();
    s.pop();
    std::cout<<root->data<<" ";
    if (root->rdata != NULL) {
      s.push(root->rdata);
    }
    if (root->ldata != NULL) {
      s.push(root->ldata);
    }
  }
}

//打印二叉树 先序遍历 先打印根
void recursor_print(tree_node* root) {
  if (root == NULL) {
    return;
  }
  std::cout<<root->data<<" ";
  recursor_print(root->ldata);
  recursor_print(root->rdata);
}

int get_height(tree_node* head, int l) {
  if (head == NULL) {
    return l;
  }
  return std::max(get_height(head->ldata,l + 1),get_height(head->rdata, l + 1));
}

void set_edge_map(tree_node* head, int l, tree_node* (*map)[2] ) {
  if (head == NULL) {
    return;
  }
  map[l][0] = map[l][0] == NULL ? head : map[l][0];
  map[l][1] = head;

  set_edge_map(head->ldata, l + 1, map);
  set_edge_map(head->rdata, l + 1, map);
}

void print_leaf(tree_node* head, int l) {
  if (head == NULL) {
    return;
  }
  if (head->ldata == NULL && head->rdata == NULL) {
    std::cout<<head->data<<" "<<std::endl;
  }
  print_leaf(head->ldata, l + 1);
  print_leaf(head->rdata, l + 1);
}

std::string serial_by_pre(tree_node* head) {
  if (head == NULL) {
    return "#!";
  }
  char tmp[4] = {0};
  sprintf(tmp, "%d", head->data);
  std::string res = std::string(tmp) + "!";
  res += serial_by_pre(head->ldata);
  res += serial_by_pre(head->rdata);
  return res;
}

//tree_node* resume_serial(std::queue<char> q) { error
tree_node* resume_serial(std::queue<char>& q) {
  char c = q.front();
  q.pop();
  if (c == '#') {
    return NULL;
  }

  tree_node* node = new tree_node;
  node->data = c - 48;
  node->ldata = resume_serial(q);
  node->rdata = resume_serial(q);
  return node;
}

int main() {
  //创建结点
  tree_node  node1, node2, node3, node4, node5, node6, node7, node8;
  node1.data = 1; node1.ldata = NULL; node1.rdata = NULL;
  node2.data = 2; node2.ldata = NULL; node2.rdata = NULL;
  node3.data = 3; node3.ldata = NULL; node3.rdata = NULL;
  node4.data = 4; node4.ldata = NULL; node4.rdata = NULL;
  node5.data = 5; node5.ldata = NULL; node5.rdata = NULL;
  node6.data = 6; node6.ldata = NULL; node6.rdata = NULL;
  node7.data = 7; node7.ldata = NULL; node7.rdata = NULL;
  node8.data = 8; node8.ldata = NULL; node8.rdata = NULL;

  //建立结点关系
  //node1.ldata = &node2;
  node1.ldata = NULL;
  node1.rdata = &node6;

  node2.ldata = NULL;
  node2.rdata = &node3;

  node3.ldata = &node4;
  node3.rdata = &node5;

  node6.ldata = NULL;
  node6.rdata = &node7;

  node7.ldata = &node8;
  node7.rdata = NULL;

  recursor_print(&node1);
  std::cout<<"\n";
  print(&node1);
  std::cout<<"\n";

  std::string res = serial_by_pre(&node1);
  std::cout<<res<<std::endl;
  //1!2!#!3!4!#!#!5!#!#!6!#!7!8!#!#!#!
 
  std::queue<char> q;
  for (int i = 0; i < (int)res.size(); i++) { //只能小于10
    if (i % 2 == 0) {
      q.push(res[i]);
    }
  }

  tree_node* n = resume_serial(q);
  print(n);

  return EXIT_SUCCESS;
}
