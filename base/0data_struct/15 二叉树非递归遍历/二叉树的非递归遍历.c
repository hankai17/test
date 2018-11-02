#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"SeqStack.h"

//二叉树结点 二叉链表
typedef struct _TREENODE{
	char ch;
	struct _TREENODE* lchild;
	struct _TREENODE* rchild;
}TreeNode;

#define FLAG_TRUE 1
#define FLAG_FALSE 0

typedef struct _MYDATA{
	TreeNode* node;
	int flag;
}MyData;

MyData* CreateMyData(TreeNode* node, int flag){
	MyData* mydata = malloc(sizeof(MyData));
	mydata->flag = flag;
	mydata->node = node;
	return mydata;
}

void NonRecursion(TreeNode* root){

	//初始化栈
	SeqStack stack = Init_SeqStack();
	Push_SeqStack(stack, CreateMyData(root, FLAG_FALSE));

	while (Size_SeqStack(stack) > 0){

		//从栈中取出节点
		MyData* temp = (MyData*)Top_SeqStack(stack);
		Pop_SeqStack(stack);

		//如果节点为空，跳过本次循环
		if (temp->node == NULL){
			continue;
		}

		if (temp->flag == FLAG_TRUE){
			printf("%c", temp->node->ch);
		}
		else{
			//先序遍历D L R
			Push_SeqStack(stack, CreateMyData(temp->node, FLAG_TRUE));
			Push_SeqStack(stack, CreateMyData(temp->node->rchild, FLAG_FALSE));
			Push_SeqStack(stack, CreateMyData(temp->node->lchild, FLAG_FALSE));

		}
	}

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

	//递归遍历
	NonRecursion(&node1);
}


int main(){

	CreateTree();


	system("pause");
	return EXIT_SUCCESS;
}