#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct _TREENODE{
	char ch;
	struct _TREENODE* lchild;
	struct _TREENODE* rchild;
}TreeNode;


//井号法创建二叉树
TreeNode* CreateBinaryTree(){
	
	char ch;
	scanf("%c",&ch);
	getchar();

	if (ch == '#'){
		return NULL;
	}

	TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
	node->ch = ch;
	node->lchild = CreateBinaryTree();
	node->rchild = CreateBinaryTree();
	
	return node;
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

//打印二叉树
void Recusion(TreeNode* root){
	if (root == NULL){
		return;
	}
	printf("%c",root->ch);
	Recusion(root->lchild);
	Recusion(root->rchild);
}


int main(){

	TreeNode* root = CreateBinaryTree();
	Recusion(root);
	FreeTreeSpace(root);

	system("pause");
	return EXIT_SUCCESS;
}