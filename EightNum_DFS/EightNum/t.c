#include<stdio.h>//ʵ��Ŀ�� ʵ������ ʵ����� ʵ����
#include<conio.h>
#include<windows.h>
typedef struct Node
{
	int Matrix[3][3];//״̬
	struct Node *parent;//ָ�򸸽ڵ�
	struct Node *child[4];//ָ���ӽڵ��б�
}Node,*pNode;//��Ű�����״̬
typedef struct StackNode
{
	pNode MatrixNode;
	struct StackNode *next;
}StackNode,*pStackNode;//ջ�Ľڵ�ṹ
StackNode* InitStack(void)
{//��ʼ����ջ���ɹ�����StackNode(ptr)��ʧ�ܷ��ؿ�
	StackNode *tS = (StackNode*)malloc(sizeof(StackNode));
	if (!tS)
		return NULL;
	tS->next = NULL;
	return tS;
}
int IsStackEmpty(StackNode *S)
{//�ж϶����Ƿ�Ϊ�գ�����0�ǿգ�1��
	if (S->next == NULL)
		return 1;
	return 0;
}
pStackNode NewStackNode(pNode MatrixNode)
{//�½�һ��������ջ�Ľڵ㣬����<StackNode>�ɹ�����������NULL
	pStackNode NewNode = (pStackNode)malloc(sizeof(StackNode));
	if (!NewNode)
		return NULL;
	NewNode->MatrixNode = MatrixNode;
	NewNode->next = NULL;
	return NewNode;
}
int PushStack(StackNode *S,pNode pN)
{//�ڵ���ջ��ʧ�ܷ���1���ɹ�����0
	pStackNode tNode = NULL;
	tNode = NewStackNode(pN);//�����½�ջ�ڵ�
	if (!tNode)
		return 1;
	tNode->next = S->next;
	S->next = tNode;//ͷ����
	return 0;
}
int PopStack(StackNode *S)
{//�ڵ��ջ��ʧ�ܷ���1���ɹ�����0
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
{//��������İ��������
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
{//���ĳ״̬�Ŀո�����
	int i,j,Flag = 0;
	for (i=0;i<3;i++)
	{
		for (j=0;j<3;j++)
		{
			if (pN->Matrix[i][j] == 0)
			{
				Flag = 1;
				break;//�ҵ��ո�
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
{//���ĳ״̬��Ψһ��ʶ
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
{//�ж���״̬�Ƿ���ͬ
	if (GetGUID(Sta1) == GetGUID(Sta2))
		return 1;
	return 0;
}
int Swap(int *A,int *B)
{//��������λ��
	int tmp = *A;
	*A = *B;
	*B = tmp;
	return 0;
}
int StatusCPY(pNode Dest,pNode Src)
{//����״̬
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
{//����һ���ƶ����״̬��D���ƿո��ƶ��ķ��� 0���� 1���� 2���� 3���ң��ɹ��ƶ��������ظ�����<pNode>��ʧ�ܻ����ظ����ؿ�
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
	if (Sta->parent && IsSameStatus(tNode,Sta->parent))//�����ظ�
		return NULL;
	tNode->child[0] = NULL;
	tNode->child[1] = NULL;
	tNode->child[2] = NULL;
	tNode->child[3] = NULL;
	tNode->parent = Sta;
	return tNode;
}
int Expand(pNode pN)
{//��չĳ״̬�ڵ����״̬
	int x,y;//��¼��ǰ״̬�Ŀո�λ��

	GetSpaceXY(pN,&x,&y);//��ÿո�λ����
	pN->child[0] = GetMovedStatus(pN,x,y,0);
	pN->child[1] = GetMovedStatus(pN,x,y,1);
	pN->child[2] = GetMovedStatus(pN,x,y,2);
	pN->child[3] = GetMovedStatus(pN,x,y,3);
}
int ShowStatus(pNode Sta)
{//��ʾ״̬����
	printf("�����Щ��Щ���\n");
	printf("��%c ��%c ��%c ��\n",Sta->Matrix[0][0]+48=='0'?' ':Sta->Matrix[0][0]+48,Sta->Matrix[0][1]+48=='0'?' ':Sta->Matrix[0][1]+48,Sta->Matrix[0][2]+48=='0'?' ':Sta->Matrix[0][2]+48);
	printf("�����੤�੤��\n");
	printf("��%c ��%c ��%c ��\n",Sta->Matrix[1][0]+48=='0'?' ':Sta->Matrix[1][0]+48,Sta->Matrix[1][1]+48=='0'?' ':Sta->Matrix[1][1]+48,Sta->Matrix[1][2]+48=='0'?' ':Sta->Matrix[1][2]+48);
	printf("�����੤�੤��\n");
	printf("��%c ��%c ��%c ��\n",Sta->Matrix[2][0]+48=='0'?' ':Sta->Matrix[2][0]+48,Sta->Matrix[2][1]+48=='0'?' ':Sta->Matrix[2][1]+48,Sta->Matrix[2][2]+48=='0'?' ':Sta->Matrix[2][2]+48);
	printf("�����ة��ة���\n");
	return 0;
}
int SetStatus(pNode oriStatusc,char *Stastr)
{//����״̬�ַ����ֶ�����Ŀ��״̬
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
{//�ݻ���ջ
	while (!IsStackEmpty(S))
	{
		PopStack(S);
	}
	free(S);
	return 0;
}
pStackNode CopyStackTop(StackNode *S)
{//���Ƴ�ջ���ڵ�
	StackNode *tNode = (pStackNode)malloc(sizeof(StackNode));
	tNode->MatrixNode = S->next->MatrixNode;
	return tNode;
}
int FreeStatusNodes(pNode Status_Begin)
{//�ͷ������������״̬�ڵ�
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
	StackNode *S = NULL;//��ջͷ�ڵ�
	StackNode *tmp = NULL;
	pNode tNode_Begin = NULL,tNode_End = NULL;//��ʼ״̬��Ŀ��״̬
	pNode pTmp = NULL;//�м�״̬
	long Time1,Time2;//DFSǰ����ʼʱ�䣬DFS�е�ʱ��
	int i;//������״̬���α�
	int Get = 0;//�ҵ�����״̬�ı�־
	int timedout = 0;//��־�Ƿ�ʱ
	char Stastr[11] = {0};//��ʱ���������״̬
	char ch;
	system("cls");
	srand(GetTickCount());
	S = InitStack();//��ʼ����ջ
	if (!S)
		return 1;
	tNode_Begin = CreateRandoMatrix();
	tNode_End = CreateRandoMatrix();
	printf("Begin:\n");
	ShowStatus(tNode_Begin);
	printf("End:\n");
	ShowStatus(tNode_End);
	printf("-------------------------------------\n");

	PushStack(S,tNode_Begin);//��ʼ״̬��ջ

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
		Expand(S->next->MatrixNode);//��չջ���ڵ�
		tmp = CopyStackTop(S);
		PopStack(S);//��ǰ��״̬�ڵ��ջ
		for (i=0;i<4;i++)
		{
			if (tmp->MatrixNode->child[i] && IsSameStatus(tmp->MatrixNode->child[i],tNode_End))//�ҵ�Ŀ��״̬
			{
				Get = 1;
				break;
			}
			if (tmp->MatrixNode->child[i])//�������ҵ���һ������״̬������ջ
			{
				PushStack(S,tmp->MatrixNode->child[i]);
			}
		}
		free(tmp);
		if (Get)//�ҵ�Ŀ��״̬��ֱ���˳�
			break;
		Time2 = GetTickCount();
		if (Time2 - Time1 > 20000)//20s���ж���ʱ
		{
			timedout = 1;
			break;
		}
	}
	if (!timedout && !IsStackEmpty(S))//ջ�ǿվ��˳��������ҵ�Ŀ��״̬
	{
		printf("Procedure(Reverse order):\n");
		pTmp = S->next->MatrixNode;
		do//ѭ���������ת������
		{
			ShowStatus(pTmp);
			pTmp = pTmp->parent;
		}while(pTmp);
	}
	else if (!timedout && IsStackEmpty(S))//ջ�գ�����û���ҵ�Ŀ��״̬(�޽�)
	{
		printf("Impossible!\n");
	}
	else//��ʱ
	{
		printf("DFS - Timedout!\n");
	}
	DestroyStack(S);//�ݻ�ջ
	FreeStatusNodes(tNode_Begin);//�ݻ�����״̬�ڵ�
	system("pause");
	return 0;
}
