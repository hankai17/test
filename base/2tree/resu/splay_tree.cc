#include <iostream>
#include "splay_tree.hh"
using namespace std;

static int arr[] = { 10,50,40,30,20,60 };
#define TBL_SIZE(a) ( (sizeof(a)) / (sizeof(a[0])) )

/*伸展树测试*/
int main() {
  int i, len;
  SplayTree<int>* tree = new SplayTree<int>();

  cout << "== 依次添加: ";
  len = TBL_SIZE(arr);
  for (i = 0; i < len; i++) {
	cout << arr[i] << " ";
	tree->insert(arr[i]);
  }

  cout << "\n== 前序遍历: ";
  tree->preOrder();

  cout << "\n== 中序遍历: ";
  tree->inOrder();

  cout << "\n== 后序遍历: ";
  tree->postOrder();
  cout << endl;

  cout << "== 最小值: " << tree->minimum() << endl;
  cout << "== 最大值: " << tree->maximum() << endl;
  cout << "== 树的详细信息: " << endl;
  tree->print();

  i = 10;
  cout << "\n== 旋转节点(" << i << ")为根节点";
  tree->splay(i);
  cout << "\n== 树的详细信息: " << endl;
  tree->print();

  // 销毁二叉树
  tree->destroy();

  return 0;
}

