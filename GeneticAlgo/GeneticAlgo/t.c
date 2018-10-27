/************************************************
	针对TSP问题修改的遗传算法，100%交叉率、可能变异

*************************************************/
#include<windows.h>
#include<stdio.h>
#define pNum 25//假定25个地点
typedef struct GeneInfo
{
	int Traceroute[pNum];//路径，此处未算入回城，适应度函数中需要补充计算
	double fitness;//适应度
	float selectper;//选择概率
	float totalper;//积累概率
}GeneInfo;//基因信息

int CalcFitness(int **Map,GeneInfo *G)
{//计算适应度并填入表格
	int i,j;
	double TotalFitness = 0.0;//总适应度
	for (i=0;i<pNum;i++)
	{
		G[i].fitness = 100.00 / Fitness(Map,G[i]);//更新适应度，针对TSP问题需要对计算结果取一百倍倒数
		TotalFitness += G[i].fitness;//更新总适应度
	}
	for (i=0;i<pNum;i++)
	{
		G[i].selectper = G[i].fitness / TotalFitness;//计算选择概率
		if (i == 0)
			G[i].totalper = G[i].selectper;//第一个染色体的积累概率即为选择概率
		else
			G[i].totalper = G[i-1].totalper + G[i].selectper;//正常计算积累概率
	}
	return 0;
}
int Mutate(GeneInfo *Gs,int Num)
{//变异，选取变异基因个数Num，开头的基因序列进行逆序操作
	int i,j,k;
	int tmp;
	
	for (k=0;k<pNum;k++)
	{//对所有染色体变异
		for (i=1,j=Num;i<j;i++,j--)
		{//执行逆序
			tmp = Gs[k].Traceroute[i];
			Gs[k].Traceroute[i] = Gs[k].Traceroute[j];
			Gs[k].Traceroute[j] = tmp;
		}
	}
	return 0;
}
int CrossOver(GeneInfo *Gs)
{//交叉，选取除起止点开头开始，中间60%不变，其余基因除起止点外交叉替换
	int i,j,k;
	int startP,CNum;//选取的开始点、不变的基因个数
	int excep[pNum] = {0};//标记某点是否需要排除
	int tmp1[pNum] = {-1},tmp2[pNum] = {-1};//临时染色体
	startP = pNum / 10;
	if (startP == 0)
		startP++;//必须排除起止点
	CNum = pNum * 0.6;
	for (i=0;i<pNum-1;i+=2)
	{
		for (j=0;j<pNum;j++)
		{
			excep[j] = 0;//排除清零
			tmp1[j] = -1;
			tmp2[j] = -1;//临时染色体清空
		}
		for (j=startP;j<startP+CNum;j++)
		{
			excep[Gs[i].Traceroute[j]] = 1;//标记A不变基因
		}

		for (j=1,k=1;j<pNum;j++)
		{//A染色体完成交叉
			if (excep[Gs[i+1].Traceroute[j]] == 1)
				continue;//后一条染色体游标遇到不变范围出现过的点，则略过
			while (k >= startP && k<= startP + CNum - 1)
				k++;//临时染色体游标走到不变范围，则略过
			tmp1[k] = Gs[i+1].Traceroute[j];
			k++;
		}
		for (j=0;j<pNum;j++)
		{
			excep[j] = 0;
		}
		for (j=startP;j<startP+CNum;j++)
		{
			excep[Gs[i+1].Traceroute[j]] = 1;//标记B不变基因
		}
		for (j=1,k=1;j<pNum;j++)
		{//B染色体完成交叉
			if (excep[Gs[i].Traceroute[j]] == 1)
				continue;//前一条染色体游标遇到不变范围出现过的点，则略过
			while (k >= startP && k<= startP + CNum - 1)
				k++;//临时染色体游标走到不变范围，则略过
			tmp2[k] = Gs[i].Traceroute[j];
			k++;
		}
		for (j=1;j<pNum;j++)
		{//将临时染色体中有效基因复制到A染色体中
			if (tmp1[j] != -1)
			{
				Gs[i].Traceroute[j] = tmp1[j];
			}
		}
		for (j=1;j<pNum;j++)
		{//将临时染色体中有效基因复制到B染色体中
			if (tmp2[j] != -1)
			{

				Gs[i+1].Traceroute[j] = tmp2[j];
			}
		}
	}
	return 0;
}
int SelectAndCopy(GeneInfo *G)
{//选择复制
	int tRand;//存放临时随机数
	GeneInfo tGs[pNum];//临时染色体列表
	int i,j;
	
	for (i=0;i<pNum;i++)
	{
		tRand = rand() % ((int)(G[pNum-1].totalper) + 1);//生成随机数，此处选0~最大积累概率
		for (j=0;j<pNum;j++)
		{
			if ((j == 0 || tRand >= (int)(G[j-1].totalper * 100)) && tRand < (int)(G[j].totalper * 100))
			{
				tGs[i] = G[j];//复制染色体到临时列表
				break;
			}
		}
	}
	for (i=0;i<pNum;i++)
		G[i] = tGs[i];//生成真正的临时列表
	
	return 0;
}
long Fitness(int **Map,GeneInfo G)
{//适应度函数，返回路径长度
	int i;
	long routeTotal = 0;//临时记录总路程
	for (i=0;i<pNum-1;i++)
	{
		routeTotal += Map[G.Traceroute[i]][G.Traceroute[i+1]];//计算去程
	}
	routeTotal += Map[G.Traceroute[pNum-1]][G.Traceroute[0]];//总路程补充回程
	return routeTotal;//直接返回路径长度
}
int** CreateRandoMap(void)
{//创建随机的距离矩阵，距离1~9
	int **MatLine = NULL;//行
	int i,j;
	
	MatLine = (int**)malloc(pNum * sizeof(int*));//创建N行
	for (i=0;i<pNum;i++)
	{
		MatLine[i] = (int*)malloc(pNum * sizeof(int));//N行N列矩阵创建
	}
	for (i=0;i<pNum;i++)
		MatLine[i][i] = 0;//行列坐标相等时为0
	for (i=0;i<pNum;i++)
	{
		for (j=0;j<pNum;j++)
		{
			if (i != j)
				MatLine[j][i] = MatLine[i][j] = rand() % 9 + 1;//距离1~9
		}
	}
	return MatLine;
}
int SameTrace(GeneInfo *GeneList,int *MiddleTrace)
{//TraceRoute是否重复
	int i,j;
	for (i=0;i<pNum;i++)
	{
		for (j=0;j<pNum;j++)
		{
			if (GeneList[i].Traceroute[j] != MiddleTrace[j])
				break;//本条不重复
		}
		if (j == pNum)
			return 1;//找到重复
	}
	return 0;
}
int NewRandomTrace(int start,int *retTrace)
{//生成一个有效的中间行程数组，范围0到N-1，排除excep中值为1的点序号
	int excep[pNum] = {0};//是否排除某序号
	int i;
	int Ret[pNum];//返回的中间行程
	excep[start] = 1;//起止点排除，不能再出现
	for (i=1;i<pNum;i++)
	{
		do
		{
			Ret[i] = rand() % pNum;
		}while(excep[Ret[i]] == 1);//若随机到了之前选择过的序号则继续寻找
		excep[Ret[i]] = 1;//排除这个找到的序列，后续不再出现
	}
	for (i=1;i<pNum;i++)
		retTrace[i] = Ret[i];
	return 0;
}
int InitGenes(int **Map,GeneInfo *GeneList,int start)
{//初始化染色体列表
	int i,j;
	int tTrace[pNum] = {start};//临时中间行程
	
	for (i=0;i<pNum;i++)
	{
		GeneList[i].Traceroute[0] = start;//起始点赋值
		do
		{
			NewRandomTrace(start,tTrace);//生成一个中间行程
		}while(SameTrace(GeneList,tTrace));//生成的路径重复则重试
		for (j=0;j<pNum;j++)
		{//已生成一个有效路径，复制到染色体列表中
			GeneList[i].Traceroute[j] = tTrace[j];
		}
	}
	CalcFitness(Map,GeneList);

	return 0;
}
int ShowMap(int **Map)
{//显示地图
	int i,j;
	printf("地图:\n");
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
{//获得最短路径对应的染色体index
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
{//显示路径
	int i;
	printf("\n%d",G.Traceroute[0]);
	for (i=1;i<pNum;i++)
		printf("->%d",G.Traceroute[i]);
	printf("->%d",G.Traceroute[0]);
	return 0;
}

int main(void)
{
	int **DistanceMAP = NULL;//地图
	GeneInfo Gs[pNum];//染色体列表，长度为pNum
	int StartP;//起止点
	double MutatePer;//变异概率
	int MFlag = 0;//是否变异(变异基因个数)
	int MaxT = 15;//最大遗传次数
	int indexMin;//最短路的染色体下标

	srand(GetTickCount());
	DistanceMAP = CreateRandoMap();//创建N行N列随机距离矩阵
	ShowMap(DistanceMAP);
	printf("\n起止点 变异率:\n");
	scanf("%d %lf",&StartP,&MutatePer);
	MutatePer *= pNum*pNum;//得到变异基因个数
	MFlag = (int)MutatePer;
	InitGenes(DistanceMAP,Gs,StartP);//初始化基因列表
	while (MaxT--)
	{
		SelectAndCopy(Gs);//选择-复制
		CrossOver(Gs);//交叉
		if (MFlag)
			Mutate(Gs,MFlag);//变异
		CalcFitness(DistanceMAP,Gs);//计算适应度
	}
	indexMin = GetMinIndex(Gs);
	ShowTraceRoute(Gs[indexMin]);//显示最短路径
	printf("\n%ld\n",Fitness(DistanceMAP,Gs[indexMin]));
	system("pause");
	return 0;
}
