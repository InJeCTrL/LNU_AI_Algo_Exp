#include<stdio.h>
#include<conio.h>
#include<windows.h>
typedef struct Node
{
	int Matrix[3][3];//״̬
	struct Node *parent;//ָ�򸸽ڵ�
	struct Node *child[4];//ָ���ӽڵ��б�
}Node,*pNode;//��Ű�����״̬
typedef struct QueueNode
{
	pNode MatrixNode;
	struct QueueNode *next;
}QueueNode,*pQueueNode;//���еĽڵ�ṹ
typedef struct Queue
{
	pQueueNode QBegin;//������ʼ��ͷ�ڵ�
	pQueueNode Head;//��ͷ
	pQueueNode Tail;//��β
}Queue;//����
Queue* InitQueue(void)
{//��ʼ�������У��ɹ�����Queue(ptr)��ʧ�ܷ��ؿ�
	Queue *tQ = (Queue*)malloc(sizeof(Queue));
	if (!tQ)
		return NULL;
	tQ->QBegin = (pQueueNode)malloc(sizeof(QueueNode));
	if (!tQ->QBegin)
	{
		free(tQ);//�ع���ջ
		return NULL;
	}
	tQ->QBegin->next = NULL;
	tQ->Head = tQ->QBegin;
	tQ->Tail = tQ->QBegin;
	return tQ;
}
int IsQueueEmpty(Queue *Q)
{//�ж϶����Ƿ�Ϊ�գ�����0�ǿգ�1��
	if (Q->QBegin->next == NULL)
		return 1;
	return 0;
}
pQueueNode NewQueueNode(pNode MatrixNode)
{//�½�һ����������еĽڵ㣬����<QueueNode>�ɹ�����������NULL
	pQueueNode NewNode = (pQueueNode)malloc(sizeof(QueueNode));
	if (!NewNode)
		return NULL;
	NewNode->MatrixNode = MatrixNode;
	NewNode->next = NULL;
	return NewNode;
}
int PushQueue(Queue *Q,pNode pN)
{//�ڵ���ӣ�ʧ�ܷ���1���ɹ�����0
	pQueueNode tNode = NULL;
	tNode = NewQueueNode(pN);//�����½����нڵ�
	if (!tNode)
		return 1;
	Q->Tail->next = tNode;//����β������
	Q->Tail = tNode;
	Q->Head = Q->QBegin->next;
	return 0;
}
int PullQueue(Queue *Q)
{//�ڵ���ӣ�ʧ�ܷ���1���ɹ�����0
	pNode ptr = NULL;
	if (IsQueueEmpty(Q))
		return 1;
	else
	{
		ptr = Q->Head->MatrixNode;
		Q->Head = Q->Head->next;
		free(Q->QBegin->next);
		Q->QBegin->next = Q->Head;
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
int DestroyQueue(Queue *Q)
{//�ݻٶ���
	while (!IsQueueEmpty(Q))
	{
		PullQueue(Q);
	}
	free(Q->QBegin);
	free(Q);
	return 0;
}
int FreeStatusNodes(pNode Status_Begin)
{//�ͷ������������״̬�ڵ�
	int i;
	Queue *tQ = InitQueue();
	PushQueue(tQ,Status_Begin);
	while (!IsQueueEmpty(tQ))
	{
		for (i=0;i<4;i++)
		{
			if (tQ->Head->MatrixNode->child[i])
			{
				PushQueue(tQ,tQ->Head->MatrixNode->child[i]);
			}
		}
		free(tQ->Head->MatrixNode);
		PullQueue(tQ);
	}
	DestroyQueue(tQ);
	return 0;
}

int main(void)
{
	Queue *Q = NULL;
	pNode tNode_Begin = NULL,tNode_End = NULL;//��ʼ״̬��Ŀ��״̬
	pNode pTmp = NULL;//�м�״̬
	int i;//������״̬���α�
	int Get = 0;//�ҵ�����״̬�ı�־
	char Stastr[11] = {0};//��ʱ���������״̬
	char ch;
	system("cls");
	srand(GetTickCount());
	Q = InitQueue();//��ʼ��������
	if (!Q)
		return 1;
	tNode_Begin = CreateRandoMatrix();
	tNode_End = CreateRandoMatrix();
	printf("Begin:\n");
	ShowStatus(tNode_Begin);
	printf("End:\n");
	ShowStatus(tNode_End);
	printf("-------------------------------------\n");

	PushQueue(Q,tNode_Begin);//��ʼ״̬���

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

	while (!IsQueueEmpty(Q))
	{
		if (IsSameStatus(Q->Head->MatrixNode,tNode_End))
		{
			Get = 1;
			break;
		}
		Expand(Q->Head->MatrixNode);//��չ��ͷ�ڵ�
		for (i=0;i<4;i++)
		{
			if (Q->Head->MatrixNode->child[i] && IsSameStatus(Q->Head->MatrixNode->child[i],tNode_End))//�ҵ�Ŀ��״̬
			{
				Get = 1;
				break;
			}
			if (Q->Head->MatrixNode->child[i])//�������ҵ���һ������״̬�������
			{
				PushQueue(Q,Q->Head->MatrixNode->child[i]);
			}
		}
		if (Get)//�ҵ�Ŀ��״̬��ֱ���˳�
			break;
		PullQueue(Q);//��ǰ��״̬�ڵ����
	}
	if (!IsQueueEmpty(Q))//���зǿվ��˳��������ҵ�Ŀ��״̬
	{
		printf("Procedure(Reverse order):\n");
		pTmp = Q->Head->MatrixNode;
		do//ѭ���������ת������
		{
			ShowStatus(pTmp);
			pTmp = pTmp->parent;
		}while(pTmp);
	}
	else//���пգ�����û���ҵ�Ŀ��״̬(�޽�)
	{
		printf("Impossible!\n");
	}
	DestroyQueue(Q);//�ݻٶ���
	FreeStatusNodes(tNode_Begin);//�ݻ�����״̬�ڵ�
	system("pause");
	return 0;
}
