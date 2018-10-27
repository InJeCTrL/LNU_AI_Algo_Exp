/************************************************
	���TSP�����޸ĵ��Ŵ��㷨��100%�����ʡ����ܱ���

*************************************************/
#include<windows.h>
#include<stdio.h>
#define pNum 25//�ٶ�25���ص�
typedef struct GeneInfo
{
	int Traceroute[pNum];//·�����˴�δ����سǣ���Ӧ�Ⱥ�������Ҫ�������
	double fitness;//��Ӧ��
	float selectper;//ѡ�����
	float totalper;//���۸���
}GeneInfo;//������Ϣ

int CalcFitness(int **Map,GeneInfo *G)
{//������Ӧ�Ȳ�������
	int i,j;
	double TotalFitness = 0.0;//����Ӧ��
	for (i=0;i<pNum;i++)
	{
		G[i].fitness = 100.00 / Fitness(Map,G[i]);//������Ӧ�ȣ����TSP������Ҫ�Լ�����ȡһ�ٱ�����
		TotalFitness += G[i].fitness;//��������Ӧ��
	}
	for (i=0;i<pNum;i++)
	{
		G[i].selectper = G[i].fitness / TotalFitness;//����ѡ�����
		if (i == 0)
			G[i].totalper = G[i].selectper;//��һ��Ⱦɫ��Ļ��۸��ʼ�Ϊѡ�����
		else
			G[i].totalper = G[i-1].totalper + G[i].selectper;//����������۸���
	}
	return 0;
}
int Mutate(GeneInfo *Gs,int Num)
{//���죬ѡȡ����������Num����ͷ�Ļ������н����������
	int i,j,k;
	int tmp;
	
	for (k=0;k<pNum;k++)
	{//������Ⱦɫ�����
		for (i=1,j=Num;i<j;i++,j--)
		{//ִ������
			tmp = Gs[k].Traceroute[i];
			Gs[k].Traceroute[i] = Gs[k].Traceroute[j];
			Gs[k].Traceroute[j] = tmp;
		}
	}
	return 0;
}
int CrossOver(GeneInfo *Gs)
{//���棬ѡȡ����ֹ�㿪ͷ��ʼ���м�60%���䣬����������ֹ���⽻���滻
	int i,j,k;
	int startP,CNum;//ѡȡ�Ŀ�ʼ�㡢����Ļ������
	int excep[pNum] = {0};//���ĳ���Ƿ���Ҫ�ų�
	int tmp1[pNum] = {-1},tmp2[pNum] = {-1};//��ʱȾɫ��
	startP = pNum / 10;
	if (startP == 0)
		startP++;//�����ų���ֹ��
	CNum = pNum * 0.6;
	for (i=0;i<pNum-1;i+=2)
	{
		for (j=0;j<pNum;j++)
		{
			excep[j] = 0;//�ų�����
			tmp1[j] = -1;
			tmp2[j] = -1;//��ʱȾɫ�����
		}
		for (j=startP;j<startP+CNum;j++)
		{
			excep[Gs[i].Traceroute[j]] = 1;//���A�������
		}

		for (j=1,k=1;j<pNum;j++)
		{//AȾɫ����ɽ���
			if (excep[Gs[i+1].Traceroute[j]] == 1)
				continue;//��һ��Ⱦɫ���α��������䷶Χ���ֹ��ĵ㣬���Թ�
			while (k >= startP && k<= startP + CNum - 1)
				k++;//��ʱȾɫ���α��ߵ����䷶Χ�����Թ�
			tmp1[k] = Gs[i+1].Traceroute[j];
			k++;
		}
		for (j=0;j<pNum;j++)
		{
			excep[j] = 0;
		}
		for (j=startP;j<startP+CNum;j++)
		{
			excep[Gs[i+1].Traceroute[j]] = 1;//���B�������
		}
		for (j=1,k=1;j<pNum;j++)
		{//BȾɫ����ɽ���
			if (excep[Gs[i].Traceroute[j]] == 1)
				continue;//ǰһ��Ⱦɫ���α��������䷶Χ���ֹ��ĵ㣬���Թ�
			while (k >= startP && k<= startP + CNum - 1)
				k++;//��ʱȾɫ���α��ߵ����䷶Χ�����Թ�
			tmp2[k] = Gs[i].Traceroute[j];
			k++;
		}
		for (j=1;j<pNum;j++)
		{//����ʱȾɫ������Ч�����Ƶ�AȾɫ����
			if (tmp1[j] != -1)
			{
				Gs[i].Traceroute[j] = tmp1[j];
			}
		}
		for (j=1;j<pNum;j++)
		{//����ʱȾɫ������Ч�����Ƶ�BȾɫ����
			if (tmp2[j] != -1)
			{

				Gs[i+1].Traceroute[j] = tmp2[j];
			}
		}
	}
	return 0;
}
int SelectAndCopy(GeneInfo *G)
{//ѡ����
	int tRand;//�����ʱ�����
	GeneInfo tGs[pNum];//��ʱȾɫ���б�
	int i,j;
	
	for (i=0;i<pNum;i++)
	{
		tRand = rand() % ((int)(G[pNum-1].totalper) + 1);//������������˴�ѡ0~�����۸���
		for (j=0;j<pNum;j++)
		{
			if ((j == 0 || tRand >= (int)(G[j-1].totalper * 100)) && tRand < (int)(G[j].totalper * 100))
			{
				tGs[i] = G[j];//����Ⱦɫ�嵽��ʱ�б�
				break;
			}
		}
	}
	for (i=0;i<pNum;i++)
		G[i] = tGs[i];//������������ʱ�б�
	
	return 0;
}
long Fitness(int **Map,GeneInfo G)
{//��Ӧ�Ⱥ���������·������
	int i;
	long routeTotal = 0;//��ʱ��¼��·��
	for (i=0;i<pNum-1;i++)
	{
		routeTotal += Map[G.Traceroute[i]][G.Traceroute[i+1]];//����ȥ��
	}
	routeTotal += Map[G.Traceroute[pNum-1]][G.Traceroute[0]];//��·�̲���س�
	return routeTotal;//ֱ�ӷ���·������
}
int** CreateRandoMap(void)
{//��������ľ�����󣬾���1~9
	int **MatLine = NULL;//��
	int i,j;
	
	MatLine = (int**)malloc(pNum * sizeof(int*));//����N��
	for (i=0;i<pNum;i++)
	{
		MatLine[i] = (int*)malloc(pNum * sizeof(int));//N��N�о��󴴽�
	}
	for (i=0;i<pNum;i++)
		MatLine[i][i] = 0;//�����������ʱΪ0
	for (i=0;i<pNum;i++)
	{
		for (j=0;j<pNum;j++)
		{
			if (i != j)
				MatLine[j][i] = MatLine[i][j] = rand() % 9 + 1;//����1~9
		}
	}
	return MatLine;
}
int SameTrace(GeneInfo *GeneList,int *MiddleTrace)
{//TraceRoute�Ƿ��ظ�
	int i,j;
	for (i=0;i<pNum;i++)
	{
		for (j=0;j<pNum;j++)
		{
			if (GeneList[i].Traceroute[j] != MiddleTrace[j])
				break;//�������ظ�
		}
		if (j == pNum)
			return 1;//�ҵ��ظ�
	}
	return 0;
}
int NewRandomTrace(int start,int *retTrace)
{//����һ����Ч���м��г����飬��Χ0��N-1���ų�excep��ֵΪ1�ĵ����
	int excep[pNum] = {0};//�Ƿ��ų�ĳ���
	int i;
	int Ret[pNum];//���ص��м��г�
	excep[start] = 1;//��ֹ���ų��������ٳ���
	for (i=1;i<pNum;i++)
	{
		do
		{
			Ret[i] = rand() % pNum;
		}while(excep[Ret[i]] == 1);//���������֮ǰѡ�������������Ѱ��
		excep[Ret[i]] = 1;//�ų�����ҵ������У��������ٳ���
	}
	for (i=1;i<pNum;i++)
		retTrace[i] = Ret[i];
	return 0;
}
int InitGenes(int **Map,GeneInfo *GeneList,int start)
{//��ʼ��Ⱦɫ���б�
	int i,j;
	int tTrace[pNum] = {start};//��ʱ�м��г�
	
	for (i=0;i<pNum;i++)
	{
		GeneList[i].Traceroute[0] = start;//��ʼ�㸳ֵ
		do
		{
			NewRandomTrace(start,tTrace);//����һ���м��г�
		}while(SameTrace(GeneList,tTrace));//���ɵ�·���ظ�������
		for (j=0;j<pNum;j++)
		{//������һ����Ч·�������Ƶ�Ⱦɫ���б���
			GeneList[i].Traceroute[j] = tTrace[j];
		}
	}
	CalcFitness(Map,GeneList);

	return 0;
}
int ShowMap(int **Map)
{//��ʾ��ͼ
	int i,j;
	printf("��ͼ:\n");
	for (i=0;i<pNum;i++)
	{
		for (j=0;j<pNum;j++)
		{
			printf(" %d",Map[i][j]);
		}
		printf("\n");
	}

	return 0;
}
int GetMinIndex(GeneInfo *G)
{//������·����Ӧ��Ⱦɫ��index
	int i;
	double fitmax = 0.0;
	int retindex = 0;
	for (i=0;i<pNum;i++)
	{
		if (fitmax < G[i].fitness)
		{
			fitmax = G[i].fitness;
			retindex = i;
		}
	}
	return retindex;
}
int ShowTraceRoute(GeneInfo G)
{//��ʾ·��
	int i;
	printf("\n%d",G.Traceroute[0]);
	for (i=1;i<pNum;i++)
		printf("->%d",G.Traceroute[i]);
	printf("->%d",G.Traceroute[0]);
	return 0;
}

int main(void)
{
	int **DistanceMAP = NULL;//��ͼ
	GeneInfo Gs[pNum];//Ⱦɫ���б�����ΪpNum
	int StartP;//��ֹ��
	double MutatePer;//�������
	int MFlag = 0;//�Ƿ����(����������)
	int MaxT = 15;//����Ŵ�����
	int indexMin;//���·��Ⱦɫ���±�

	srand(GetTickCount());
	DistanceMAP = CreateRandoMap();//����N��N������������
	ShowMap(DistanceMAP);
	printf("\n��ֹ�� ������:\n");
	scanf("%d %lf",&StartP,&MutatePer);
	MutatePer *= pNum*pNum;//�õ�����������
	MFlag = (int)MutatePer;
	InitGenes(DistanceMAP,Gs,StartP);//��ʼ�������б�
	while (MaxT--)
	{
		SelectAndCopy(Gs);//ѡ��-����
		CrossOver(Gs);//����
		if (MFlag)
			Mutate(Gs,MFlag);//����
		CalcFitness(DistanceMAP,Gs);//������Ӧ��
	}
	indexMin = GetMinIndex(Gs);
	ShowTraceRoute(Gs[indexMin]);//��ʾ���·��
	printf("\n%ld\n",Fitness(DistanceMAP,Gs[indexMin]));
	system("pause");
	return 0;
}
