#include<iostream>
using namespace std;

struct bs_tree_node {
  int m_value;
  bs_tree_node *m_left;
  bs_tree_node *m_right;
};

void add_tree_node(bs_tree_node* &cur, int value);
void in_order_tree(bs_tree_node* tree);
void convert_2_dlist(bs_tree_node* cur);

bs_tree_node* head = NULL;//指向循环队列头节点
bs_tree_node* index = NULL;//指向前一个节点

int main(void) {
  bs_tree_node* pRoot = NULL;
  add_tree_node(pRoot, 10);
  add_tree_node(pRoot, 6);
  add_tree_node(pRoot, 14);
  add_tree_node(pRoot, 4);
  add_tree_node(pRoot, 8);
  add_tree_node(pRoot, 12);
  add_tree_node(pRoot, 16);
  in_order_tree(pRoot);

  printf("\n");
  return 0;
}

/*建立二叉树,递归建立二叉树。引用（一级指针）做输出；参数为 根节点和子数的值*/
/*递归的两个特性 1存在限制条件 2每次递归调用后越来越接近这个限制条件*/
void add_tree_node(bs_tree_node* &cur, int value) {
  if (cur == NULL) {
	//return;
	bs_tree_node* tree = new bs_tree_node();
	tree->m_value = value;
	tree->m_left = NULL;
	tree->m_right = NULL;
	cur = tree;
  } else if(cur->m_value < value) {
	add_tree_node(cur->m_right, value);
  } else if(cur->m_value > value) {
	add_tree_node(cur->m_left, value);
  } else {
	cout << "node repeated" << endl;
  }
}

/*中序遍历 */
void in_order_tree(bs_tree_node* tree) {
  if (NULL == tree) {
	return;
  }
  if (NULL != tree->m_left) {
	in_order_tree(tree->m_left);
  }
  if (NULL != tree) {
	//cout << tree->m_value << " ";
  }
  convert_2_dlist(tree);
  if (NULL != tree->m_right) {
	in_order_tree(tree->m_right);
  }
}

void convert_2_dlist(bs_tree_node* cur)
{
  cur->m_left = index;
  if (NULL == index) {
	head = cur;
  } else {
	index->m_right = cur;
  }
  index = cur;
  cout << cur->m_value << " ";
}
