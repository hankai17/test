//树专业定义：有且只有一个称为根的节点
//有若干个互不相交的子树 这些树本身也是一个树 
//通俗点 树是由节点和边组成，每个节点只有一个父节点但可以有多个子节点
//但有一个节点例外 该节点没有父节点 称为根节点 

//叶子节点 没有子节点的节点
//非终端节点 就是非叶子节点
//深度 有几层 从根1开始
//度 子节点的个数
//一般树 任意一个节点的子节点 个数不受限制
//二叉树 任意一个节点 的个数最多两个 且子节点位置不能更改 
//一般二叉树
//满二叉树 都是挂两个 组成一个大三角形 高级对称的那个 
// 完全二叉树  如果删除了满二叉树最低层 最右边的连续若干个节点 这样形成的二叉树就是完全二叉树 
//森林 n个互不相交的树的集合 
/*
一般树的存储
双亲表示法
孩子表示法
双亲孩子表示法
二叉树表示法
二叉树的遍历
先序遍历：先访问根节点 再先序访问左子树再先序访问柚子树
已知两种遍历序列求原始二叉树
*/
#include <stdio.h>
#include <malloc.h>
#include <iostream>
int sum1 = 0;
struct BTNode
{
	char data;
	struct BTNode *pLchild;
	struct BTNode *pRchild;
};


void coutLeaf(struct BTNode * T)
{
	if (T != NULL)
	{
		if (T->pLchild == NULL && T->pRchild == NULL)
		{
			sum1++;
		}
		if (T->pLchild)
		{
			coutLeaf(T->pLchild);
		}
		if (T->pRchild)
		{
			coutLeaf(T->pRchild);
		}
	}

}

void coutLeaf1(struct BTNode * T, int * sum)
{
	if (T != NULL)
	{
		if (T->pLchild == NULL && T->pRchild == NULL)
		{
			(*sum)++;
		}
		if (T->pLchild)
		{
			coutLeaf1(T->pLchild, sum);
		}
		if (T->pRchild)
		{
			coutLeaf1(T->pRchild, sum);
		}
	}

}

struct BTNode * CreateBTree()
{
	struct BTNode *pA = (struct BTNode*)malloc(sizeof(struct BTNode));
	struct BTNode *pB = (struct BTNode*)malloc(sizeof(struct BTNode));
	struct BTNode *pC = (struct BTNode*)malloc(sizeof(struct BTNode));
	struct BTNode *pD = (struct BTNode*)malloc(sizeof(struct BTNode));
	struct BTNode *pE = (struct BTNode*)malloc(sizeof(struct BTNode));

	pA->data = 'A';
	pB->data = 'B';
	pC->data = 'C';
	pD->data = 'D';
	pE->data = 'E';
	pA->pLchild = pB;
	pA->pRchild = pC;
	pB->pLchild = pB->pRchild = NULL;
	pC->pLchild = pD;
	pC->pRchild = NULL;
	pD->pLchild = NULL;
	pD->pRchild = pE;
	pE->pLchild = pE->pRchild = NULL;

	return pA;
}

void PreTraverseBTree(struct BTNode *pT)
{
	if (pT != NULL)
	{
		printf("%c\n", pT->data);//pT->pLchild可以代表整个左子树 
		if (NULL != pT->pLchild)
		{
			PreTraverseBTree(pT->pLchild);
		}
		if (NULL != pT->pRchild)
		{
			PreTraverseBTree(pT->pRchild);
		}

		//先访问根节点 再访问左右子树 
	}
}

int Depth(BTNode * T)
{
	int deptleft = 0;
	int deptright = 0;
	int deptval = 0;

	if (T == NULL)
	{
		deptval = 0;
		return deptval;
	}
	deptleft = Depth(T->pLchild);
	deptright = Depth(T->pRchild);

	deptval = 1 + (deptleft > deptright ? deptleft : deptright);

	return deptval;

}

BTNode * CopyTree(BTNode * T)
{
	BTNode * newNode = NULL;
	BTNode * newLp = NULL;
	BTNode * newRp = NULL;
	if (T == NULL)
	{
		return NULL;
	}
	if (T->pLchild != NULL)
	{
		newLp = CopyTree(T->pLchild);
	}
	else
	{
		newLp = NULL;
	}
	if (T->pRchild != NULL)
	{
		newRp = CopyTree(T->pRchild);
	}
	else
	{
		newRp = NULL;
	}

	newNode = (BTNode *)malloc(sizeof(BTNode));
	if (newNode == NULL)
	{
		return NULL;
	}
	newNode->pLchild = newLp;
	newNode->pRchild = newRp;
	newNode->data = T->data;
	return newNode;




}








int main()
{
	int  sum = 0;
	struct BTNode *pT = CreateBTree();
	PreTraverseBTree(pT);
	//InTraverseBTree(pT);
	//PostTraverseBTree(pT);
	//coutLeaf1(pT,&sum);
	//printf("sum: %d",sum);
	//printf("高度： %d ", Depth(pT));

	system("pause");
	return 0;
}

