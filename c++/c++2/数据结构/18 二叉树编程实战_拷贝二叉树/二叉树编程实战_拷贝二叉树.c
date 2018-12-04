#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


//二叉树结点 二叉链表
typedef struct _TREENODE{
	char ch;
	struct _TREENODE* lchild;
	struct _TREENODE* rchild;
}TreeNode;

TreeNode* CopyTree(TreeNode* root){
	if (root == NULL){
		return NULL;
	}

	//拷贝左子树
	TreeNode* leftTree = CopyTree(root->lchild);
	//拷贝右子树
	TreeNode* rightTree = CopyTree(root->rchild);

	//创建新节点
	TreeNode* newnode = (TreeNode*)malloc(sizeof(TreeNode));
	newnode->lchild = leftTree;
	newnode->rchild = rightTree;
	newnode->ch = root->ch;

	return newnode;
}

void FreeTreeSpace(TreeNode* root){
	if (root == NULL){
		return;
	}
	
	//释放左子树
	FreeTreeSpace(root->lchild);
	//释放右子树
	FreeTreeSpace(root->rchild);
	//释放当前节点
	free(root);
}

//递归遍历
void Recursion(TreeNode* root){
	if (root == NULL){
		return;
	}
	printf("%c",root->ch);
	Recursion(root->lchild);
	Recursion(root->rchild);
}


void CreateTree(){

	//创建结点
	TreeNode node1, node2, node3, node4, node5, node6, node7, node8;
	node1.ch = 'A'; node1.lchild = NULL; node1.rchild = NULL;
	node2.ch = 'B'; node2.lchild = NULL; node2.rchild = NULL;
	node3.ch = 'C';	node3.lchild = NULL; node3.rchild = NULL;
	node4.ch = 'D';	node4.lchild = NULL; node4.rchild = NULL;
	node5.ch = 'E';	node5.lchild = NULL; node5.rchild = NULL;
	node6.ch = 'F';	node6.lchild = NULL; node6.rchild = NULL;
	node7.ch = 'G';	node7.lchild = NULL; node7.rchild = NULL;
	node8.ch = 'H';	node8.lchild = NULL; node8.rchild = NULL;

	//建立结点关系
	node1.lchild = &node2;
	node1.rchild = &node6;

	node2.lchild = NULL;
	node2.rchild = &node3;

	node3.lchild = &node4;
	node3.rchild = &node5;

	node6.lchild = NULL;
	node6.rchild = &node7;

	node7.lchild = &node8;
	node7.rchild = NULL;

	TreeNode* root = CopyTree(&node1);
	Recursion(&node1);
	printf("\n");
	Recursion(root);
	//释放二叉树
	FreeTreeSpace(root);
}


int main(){

	CreateTree();


	system("pause");
	return EXIT_SUCCESS;
}