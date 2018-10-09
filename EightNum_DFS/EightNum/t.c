#include<stdio.h>//实验目的 实验内容 实验代码 实验结果
#include<conio.h>
#include<windows.h>
typedef struct Node
{
	int Matrix[3][3];//状态
	struct Node *parent;//指向父节点
	struct Node *child[4];//指向子节点列表
}Node,*pNode;//存放八数码状态
typedef struct StackNode
{
	pNode MatrixNode;
	struct StackNode *next;
}StackNode,*pStackNode;//栈的节点结构
StackNode* InitStack(void)
{//初始化链栈，成功返回StackNode(ptr)，失败返回空
	StackNode *tS = (StackNode*)malloc(sizeof(StackNode));
	if (!tS)
		return NULL;
	tS->next = NULL;
	return tS;
}
int IsStackEmpty(StackNode *S)
{//判断队列是否为空，返回0非空，1空
	if (S->next == NULL)
		return 1;
	return 0;
}
pStackNode NewStackNode(pNode MatrixNode)
{//新建一个待插入栈的节点，返回<StackNode>成功创建，返回NULL
	pStackNode NewNode = (pStackNode)malloc(sizeof(StackNode));
	if (!NewNode)
		return NULL;
	NewNode->MatrixNode = MatrixNode;
	NewNode->next = NULL;
	return NewNode;
}
int PushStack(StackNode *S,pNode pN)
{//节点入栈，失败返回1，成功返回0
	pStackNode tNode = NULL;
	tNode = NewStackNode(pN);//尝试新建栈节点
	if (!tNode)
		return 1;
	tNode->next = S->next;
	S->next = tNode;//头插入
	return 0;
}
int PopStack(StackNode *S)
{//节点出栈，失败返回1，成功返回0
	pStackNode ptr = NULL;
	if (IsStackEmpty(S))
		return 1;
	else
	{
		ptr = S->next;
		S->next = ptr->next;
		free(ptr);
		return 0;
	}
}
pNode CreateRandoMatrix(void)
{//创建随机的八数码矩阵
	int i,rx,ry;
	pNode tNode = (pNode)malloc(sizeof(Node));
	if (!tNode)
		return NULL;
	for (i=0;i<9;i++)
	{
		do
		{
			rx = rand() % 3;
			ry = rand() % 3;
		}while(tNode->Matrix[rx][ry] >= 0 && tNode->Matrix[rx][ry] <= 8);
		tNode->Matrix[rx][ry] = i;
	}
	tNode->parent = NULL;
	tNode->child[0] = NULL;
	tNode->child[1] = NULL;
	tNode->child[2] = NULL;
	tNode->child[3] = NULL;
	return tNode;
}
int GetSpaceXY(pNode pN,int *x,int *y)
{//获得某状态的空格坐标
	int i,j,Flag = 0;
	for (i=0;i<3;i++)
	{
		for (j=0;j<3;j++)
		{
			if (pN->Matrix[i][j] == 0)
			{
				Flag = 1;
				break;//找到空格
			}
		}
		if (Flag)
			break;
	}
	*x = i;
	*y = j;
	return 0;
}
long GetGUID(pNode Sta)
{//获得某状态的唯一标识
	int i,j;
	long ID = 0;
	for (i=0;i<3;i++)
	{
		for (j=0;j<3;j++)
			ID = ID * 10 + Sta->Matrix[i][j];
	}
	return ID;
}
int IsSameStatus(pNode Sta1,pNode Sta2)
{//判断两状态是否相同
	if (GetGUID(Sta1) == GetGUID(Sta2))
		return 1;
	return 0;
}
int Swap(int *A,int *B)
{//交换两数位置
	int tmp = *A;
	*A = *B;
	*B = tmp;
	return 0;
}
int StatusCPY(pNode Dest,pNode Src)
{//复制状态
	int i,j;
	for (i=0;i<3;i++)
	{
		for (j=0;j<3;j++)
		{
			Dest->Matrix[i][j] = Src->Matrix[i][j];
		}
	}
	return 0;
}
pNode GetMovedStatus(pNode Sta,int x,int y,int D)
{//返回一种移动后的状态，D控制空格移动的方向 0：上 1：下 2：左 3：右，成功移动并且无重复返回<pNode>，失败或发生重复返回空
	pNode tNode = NULL;
	int tmp;
	tNode = (pNode)malloc(sizeof(Node));
	if (!tNode)
		return NULL;
	StatusCPY(tNode,Sta);
	switch (D)
	{
	case 0:
		if (x-1 < 0)
			return NULL;
		Swap(&tNode->Matrix[x-1][y],&tNode->Matrix[x][y]);
		break;
	case 1:
		if (x+1 > 2)
			return NULL;
		Swap(&tNode->Matrix[x+1][y],&tNode->Matrix[x][y]);
		break;
	case 2:
		if (y-1 < 0)
			return NULL;
		Swap(&tNode->Matrix[x][y-1],&tNode->Matrix[x][y]);
		break;
	case 3:
		if (y+1 > 2)
			return NULL;
		Swap(&tNode->Matrix[x][y+1],&tNode->Matrix[x][y]);
		break;
	}
	if (Sta->parent && IsSameStatus(tNode,Sta->parent))//发生重复
		return NULL;
	tNode->child[0] = NULL;
	tNode->child[1] = NULL;
	tNode->child[2] = NULL;
	tNode->child[3] = NULL;
	tNode->parent = Sta;
	return tNode;
}
int Expand(pNode pN)
{//扩展某状态节点的子状态
	int x,y;//记录当前状态的空格位置

	GetSpaceXY(pN,&x,&y);//获得空格位坐标
	pN->child[0] = GetMovedStatus(pN,x,y,0);
	pN->child[1] = GetMovedStatus(pN,x,y,1);
	pN->child[2] = GetMovedStatus(pN,x,y,2);
	pN->child[3] = GetMovedStatus(pN,x,y,3);
}
int ShowStatus(pNode Sta)
{//显示状态矩阵
	printf("┌─┬─┬─┐\n");
	printf("│%c │%c │%c │\n",Sta->Matrix[0][0]+48=='0'?' ':Sta->Matrix[0][0]+48,Sta->Matrix[0][1]+48=='0'?' ':Sta->Matrix[0][1]+48,Sta->Matrix[0][2]+48=='0'?' ':Sta->Matrix[0][2]+48);
	printf("├─┼─┼─┤\n");
	printf("│%c │%c │%c │\n",Sta->Matrix[1][0]+48=='0'?' ':Sta->Matrix[1][0]+48,Sta->Matrix[1][1]+48=='0'?' ':Sta->Matrix[1][1]+48,Sta->Matrix[1][2]+48=='0'?' ':Sta->Matrix[1][2]+48);
	printf("├─┼─┼─┤\n");
	printf("│%c │%c │%c │\n",Sta->Matrix[2][0]+48=='0'?' ':Sta->Matrix[2][0]+48,Sta->Matrix[2][1]+48=='0'?' ':Sta->Matrix[2][1]+48,Sta->Matrix[2][2]+48=='0'?' ':Sta->Matrix[2][2]+48);
	printf("└─┴─┴─┘\n");
	return 0;
}
int SetStatus(pNode oriStatusc,char *Stastr)
{//根据状态字符串手动设置目标状态
	int i,j;
	for (i=0;i<3;i++)
	{
		for (j=0;j<3;j++)
		{
			oriStatusc->Matrix[i][j] = (*Stastr) - 48;
			Stastr++;
		}
	}
	return 0;
}
int DestroyStack(StackNode *S)
{//摧毁链栈
	while (!IsStackEmpty(S))
	{
		PopStack(S);
	}
	free(S);
	return 0;
}
pStackNode CopyStackTop(StackNode *S)
{//复制出栈顶节点
	StackNode *tNode = (pStackNode)malloc(sizeof(StackNode));
	tNode->MatrixNode = S->next->MatrixNode;
	return tNode;
}
int FreeStatusNodes(pNode Status_Begin)
{//释放所有申请过的状态节点
	int i;
	StackNode *tS = InitStack();
	StackNode *tmp = NULL;
	PushStack(tS,Status_Begin);
	while (!IsStackEmpty(tS))
	{
		tmp = CopyStackTop(tS);
		PopStack(tS);
		for (i=0;i<4;i++)
		{
			if (tmp->MatrixNode->child[i])
			{
				PushStack(tS,tmp->MatrixNode->child[i]);
			}
		}
		free(tmp->MatrixNode);
		free(tmp);
	}
	DestroyStack(tS);
	return 0;
}

int main(void)
{
	StackNode *S = NULL;//链栈头节点
	StackNode *tmp = NULL;
	pNode tNode_Begin = NULL,tNode_End = NULL;//初始状态与目标状态
	pNode pTmp = NULL;//中间状态
	long Time1,Time2;//DFS前的起始时间，DFS中的时间
	int i;//遍历子状态的游标
	int Get = 0;//找到最终状态的标志
	int timedout = 0;//标志是否超时
	char Stastr[11] = {0};//临时保存输入的状态
	char ch;
	system("cls");
	srand(GetTickCount());
	S = InitStack();//初始化链栈
	if (!S)
		return 1;
	tNode_Begin = CreateRandoMatrix();
	tNode_End = CreateRandoMatrix();
	printf("Begin:\n");
	ShowStatus(tNode_Begin);
	printf("End:\n");
	ShowStatus(tNode_End);
	printf("-------------------------------------\n");

	PushStack(S,tNode_Begin);//初始状态入栈

	printf("Press i to modify the matrix, or others to start.\n");
	ch = getch();
	if (ch == 'i')
	{
		printf("Input matrix vector:\n");
		scanf("%s",Stastr);
		SetStatus(tNode_End,Stastr);
		system("cls");
		printf("Begin:\n");
		ShowStatus(tNode_Begin);
		printf("End:\n");
		ShowStatus(tNode_End);
		printf("-------------------------------------\n");
	}
	Time1 = GetTickCount();
	while (!IsStackEmpty(S))
	{
		if (IsSameStatus(S->next->MatrixNode,tNode_End))
		{
			Get = 1;
			break;
		}
		Expand(S->next->MatrixNode);//扩展栈顶节点
		tmp = CopyStackTop(S);
		PopStack(S);//当前的状态节点出栈
		for (i=0;i<4;i++)
		{
			if (tmp->MatrixNode->child[i] && IsSameStatus(tmp->MatrixNode->child[i],tNode_End))//找到目标状态
			{
				Get = 1;
				break;
			}
			if (tmp->MatrixNode->child[i])//仅仅是找到了一个可用状态，则入栈
			{
				PushStack(S,tmp->MatrixNode->child[i]);
			}
		}
		free(tmp);
		if (Get)//找到目标状态则直接退出
			break;
		Time2 = GetTickCount();
		if (Time2 - Time1 > 20000)//20s则判定超时
		{
			timedout = 1;
			break;
		}
	}
	if (!timedout && !IsStackEmpty(S))//栈非空就退出，代表找到目标状态
	{
		printf("Procedure(Reverse order):\n");
		pTmp = S->next->MatrixNode;
		do//循环倒序输出转换过程
		{
			ShowStatus(pTmp);
			pTmp = pTmp->parent;
		}while(pTmp);
	}
	else if (!timedout && IsStackEmpty(S))//栈空，代表没有找到目标状态(无解)
	{
		printf("Impossible!\n");
	}
	else//超时
	{
		printf("DFS - Timedout!\n");
	}
	DestroyStack(S);//摧毁栈
	FreeStatusNodes(tNode_Begin);//摧毁所有状态节点
	system("pause");
	return 0;
}
