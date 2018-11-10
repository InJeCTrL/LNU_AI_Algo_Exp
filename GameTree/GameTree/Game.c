#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#define N 3//假定一字棋棋盘为3x3
typedef struct GameStatus
{//棋局状态
	int Game[N][N];
}GameStatus;
typedef struct Pt
{//棋子位置坐标
	int x,y;
}Pt;
int method;//先后手方式
int InitBlankGame(GameStatus **G)
{//初始化空白棋盘，全填0
	int i,j;
	*G = (GameStatus*)malloc(sizeof(GameStatus));
	for (i=0;i<N;i++)
	{
		for (j=0;j<N;j++)
		{
			(*G)->Game[i][j] = 0;
		}
	}
	return 0;
}
int GetLittleG(int Me,GameStatus G)
{//获得单边评估值
	int i,j;
	int trank = 0;
	int T = 0;
	for (i=0,j=0;i<N && j<N;i++,j++)
	{//左上到右下斜线
		if (G.Game[i][j] == (Me == 1?2:1))
			break;//发现此斜线上有用户落子
		else if (G.Game[i][j] == Me)
			trank++;
	}
	if (trank == N)//已成一线
		return INT_MAX;
	else
		trank = 0;
	if (i >= N)//此斜线累计
		T++;
	for (i=0,j=2;i<N && j>=0;i++,j--)
	{//右上到左下斜线
		if (G.Game[i][j] == (Me == 1?2:1))
			break;//发现此斜线上有用户落子
		else if (G.Game[i][j] == Me)
			trank++;
	}
	if (trank == N)//已成一线
		return INT_MAX;
	else
		trank = 0;
	if (j < 0)//此斜线累计
		T++;
	for (i=0;i<N;i++)
	{//横行扫描
		for (j=0;j<N;j++)
		{
			if (G.Game[i][j] == (Me == 1?2:1))
				break;
			else if (G.Game[i][j] == Me)
				trank++;
		}
		if (trank == N)//已成一线
			return INT_MAX;
		else
			trank = 0;
		if (j >= N)
			T++;
	}
	for (i=0;i<N;i++)
	{//竖列扫描
		for (j=0;j<N;j++)
		{
			if (G.Game[j][i] == (Me == 1?2:1))
				break;
			else if (G.Game[j][i] == Me)
				trank++;
		}
		if (trank == N)//已成一线
			return INT_MAX;
		else
			trank = 0;
		if (j >= N)
			T++;
	}
	return T;
}
int G(GameStatus G)
{//评估函数，method为1：用户先手（1为用户落子），2：用户后手（1为程序落子）
	int machine = 0,user = 0;
	int i,j;
	user = GetLittleG(method,G);
	if (method == 1)
		machine = GetLittleG(2,G);
	else
		machine = GetLittleG(1,G);
	return machine - user;
}
Pt Calc(GameStatus _G)
{//计算下一个应落子位置
	int i,j,m,n;
	Pt PT = {0,0};
	int maxG = INT_MAX,tG = 0,minG = INT_MIN;
	for (i=0;i<N;i++)
	{
		for (j=0;j<N;j++)
		{
			if (_G.Game[i][j] == 0)//为0代表未落子
			{
				_G.Game[i][j] = (method == 1)?2:1;//模拟机器落子
				maxG = INT_MAX;
				for (m=0;m<N;m++)
				{
					for (n=0;n<N;n++)
					{
						if (_G.Game[m][n] == 0)//为0代表未落子
						{
							_G.Game[m][n] = method;//模拟用户落子
							tG = G(_G);//计算评估值
							if (maxG > tG)
							{//与节点，取最小评估值
								maxG = tG;
							}
							_G.Game[m][n] = 0;//恢复棋局，撤销用户临时落子
						}
					}
				}
				if (minG < maxG)
				{//或节点，取最大评估值的棋局
					PT.x = i;
					PT.y = j;
					minG = maxG;
				}
				_G.Game[i][j] = 0;//恢复棋局，撤销机器临时落子
			}
		}
	}
	return PT;
}
int SwitchMethod(void)
{//选择先手或后手
	char ch;
	do
	{
		printf("1.先手\n");
		printf("2.后手\n");
		printf("输入序号选择：");
		ch = getch();
		system("cls");
	}while(ch != '1' && ch != '2');
	return ch-48;
}
int ShowGame(GameStatus G)
{
	int i,j;
	for (i=0;i<N;i++)
	{
		for (j=0;j<N;j++)
		{
			switch (G.Game[i][j])
			{
				case 1:
					printf("●");
					break;
				case 2:
					printf("○");
					break;
				case 0:
					printf("□");
					break;
			}
		}
		printf("\n");
	}
	return 0;
}
int WinOrLose(GameStatus G)
{//返回1实心获胜，返回2空心获胜，返回3平局，返回0未结束
	int i,j;
	int user = 0,Imp = 0;
	for (i=0,j=0;i<N && j<N;i++,j++)
	{//左上到右下斜线
		if (!G.Game[i][j])
		{
			Imp = 1;
			break;
		}
		if (G.Game[i][j] == 1)
			user++;
	}
	if (Imp)
	{
		Imp = 0;
	}
	else
	{
		if (user == N)//此斜线黑子胜利
			return 1;
		else if (user == 0)
			return 2;
	}
	user = 0;
	for (i=0,j=2;i<N && j>=0;i++,j--)
	{//右上到左下斜线
		if (!G.Game[i][j])
		{
			Imp = 1;
			break;
		}
		if (G.Game[i][j] == 1)
			user++;
	}
	if (Imp)
	{
		Imp = 0;
	}
	else
	{
		if (user == N)//此斜线黑子胜利
			return 1;
		else if (user == 0)
			return 2;
	}
	user = 0;
	for (i=0;i<N;i++)
	{//横行扫描
		for (j=0;j<N;j++)
		{
			if (!G.Game[i][j])
			{
				Imp = 1;
				break;
			}
			if (G.Game[i][j] == 1)
				user++;
		}
		if (Imp)
		{
			Imp = 0;
		}
		else
		{
			if (user == N)//此横行黑子胜利
				return 1;
			else if (user == 0)
				return 2;
		}
		user = 0;
	}
	for (j=0;j<N;j++)
	{//竖列扫描
		for (i=0;i<N;i++)
		{
			if (!G.Game[i][j])
			{
				Imp = 1;
				break;
			}
			if (G.Game[i][j] == 1)
				user++;
		}
		if (Imp)
		{
			Imp = 0;
		}
		else
		{
			if (user == N)//此竖列黑子胜利
				return 1;
			else if (user == 0)
				return 2;
		}
		user = 0;
	}
	for (i=0;i<N;i++)
	{
		for (j=0;j<N;j++)
			if (G.Game[i][j])
				user++;
	}
	if (user == 9)
		return 3;
	return 0;
}

int main(void)
{
	GameStatus *G = NULL;//棋局
	Pt P = {0,0};//落子位置
	int Ux,Uy;//用户选择的落子坐标
	int Result;//保存棋局结果
	InitBlankGame(&G);//初始化棋局
	method = SwitchMethod();//选择先后手
	if (method == 1)
	{//用户先手，先暂停
		scanf("%d,%d",&Ux,&Uy);
	}
	else
	{//程序先手，首选中间
		G->Game[N/2][N/2] = 1;
		ShowGame(*G);
		scanf("%d,%d",&Ux,&Uy);
	}
	G->Game[Ux][Uy] = method;//执行用户的落子
	ShowGame(*G);
	do
	{
		P = Calc(*G);
		G->Game[P.x][P.y] = (method == 1)?2:1;
		system("cls");
		ShowGame(*G);
		scanf("%d,%d",&Ux,&Uy);
		G->Game[Ux][Uy] = method;//执行用户的落子
		system("cls");
		ShowGame(*G);
	}while((Result = WinOrLose(*G)) == 0);
	if (Result == 1)
	{
		printf("实心获胜\n");
	}
	else if (Result == 2)
	{
		printf("空心获胜\n");
	}
	else
	{
		printf("平局\n");
	}
	getch();
	return 0;
}