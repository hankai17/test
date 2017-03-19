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

