#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#define N 3//�ٶ�һ��������Ϊ3x3
typedef struct GameStatus
{//���״̬
	int Game[N][N];
}GameStatus;
typedef struct Pt
{//����λ������
	int x,y;
}Pt;
int method;//�Ⱥ��ַ�ʽ
int InitBlankGame(GameStatus **G)
{//��ʼ���հ����̣�ȫ��0
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
{//��õ�������ֵ
	int i,j;
	int trank = 0;
	int T = 0;
	for (i=0,j=0;i<N && j<N;i++,j++)
	{//���ϵ�����б��
		if (G.Game[i][j] == (Me == 1?2:1))
			break;//���ִ�б�������û�����
		else if (G.Game[i][j] == Me)
			trank++;
	}
	if (trank == N)//�ѳ�һ��
		return INT_MAX;
	else
		trank = 0;
	if (i >= N)//��б���ۼ�
		T++;
	for (i=0,j=2;i<N && j>=0;i++,j--)
	{//���ϵ�����б��
		if (G.Game[i][j] == (Me == 1?2:1))
			break;//���ִ�б�������û�����
		else if (G.Game[i][j] == Me)
			trank++;
	}
	if (trank == N)//�ѳ�һ��
		return INT_MAX;
	else
		trank = 0;
	if (j < 0)//��б���ۼ�
		T++;
	for (i=0;i<N;i++)
	{//����ɨ��
		for (j=0;j<N;j++)
		{
			if (G.Game[i][j] == (Me == 1?2:1))
				break;
			else if (G.Game[i][j] == Me)
				trank++;
		}
		if (trank == N)//�ѳ�һ��
			return INT_MAX;
		else
			trank = 0;
		if (j >= N)
			T++;
	}
	for (i=0;i<N;i++)
	{//����ɨ��
		for (j=0;j<N;j++)
		{
			if (G.Game[j][i] == (Me == 1?2:1))
				break;
			else if (G.Game[j][i] == Me)
				trank++;
		}
		if (trank == N)//�ѳ�һ��
			return INT_MAX;
		else
			trank = 0;
		if (j >= N)
			T++;
	}
	return T;
}
int G(GameStatus G)
{//����������methodΪ1���û����֣�1Ϊ�û����ӣ���2���û����֣�1Ϊ�������ӣ�
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
{//������һ��Ӧ����λ��
	int i,j,m,n;
	Pt PT = {0,0};
	int maxG = INT_MAX,tG = 0,minG = INT_MIN;
	for (i=0;i<N;i++)
	{
		for (j=0;j<N;j++)
		{
			if (_G.Game[i][j] == 0)//Ϊ0����δ����
			{
				_G.Game[i][j] = (method == 1)?2:1;//ģ���������
				maxG = INT_MAX;
				for (m=0;m<N;m++)
				{
					for (n=0;n<N;n++)
					{
						if (_G.Game[m][n] == 0)//Ϊ0����δ����
						{
							_G.Game[m][n] = method;//ģ���û�����
							tG = G(_G);//��������ֵ
							if (maxG > tG)
							{//��ڵ㣬ȡ��С����ֵ
								maxG = tG;
							}
							_G.Game[m][n] = 0;//�ָ���֣������û���ʱ����
						}
					}
				}
				if (minG < maxG)
				{//��ڵ㣬ȡ�������ֵ�����
					PT.x = i;
					PT.y = j;
					minG = maxG;
				}
				_G.Game[i][j] = 0;//�ָ���֣�����������ʱ����
			}
		}
	}
	return PT;
}
int SwitchMethod(void)
{//ѡ�����ֻ����
	char ch;
	do
	{
		printf("1.����\n");
		printf("2.����\n");
		printf("�������ѡ��");
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
					printf("��");
					break;
				case 2:
					printf("��");
					break;
				case 0:
					printf("��");
					break;
			}
		}
		printf("\n");
	}
	return 0;
}
int WinOrLose(GameStatus G)
{//����1ʵ�Ļ�ʤ������2���Ļ�ʤ������3ƽ�֣�����0δ����
	int i,j;
	int user = 0,Imp = 0;
	for (i=0,j=0;i<N && j<N;i++,j++)
	{//���ϵ�����б��
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
		if (user == N)//��б�ߺ���ʤ��
			return 1;
		else if (user == 0)
			return 2;
	}
	user = 0;
	for (i=0,j=2;i<N && j>=0;i++,j--)
	{//���ϵ�����б��
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
		if (user == N)//��б�ߺ���ʤ��
			return 1;
		else if (user == 0)
			return 2;
	}
	user = 0;
	for (i=0;i<N;i++)
	{//����ɨ��
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
			if (user == N)//�˺��к���ʤ��
				return 1;
			else if (user == 0)
				return 2;
		}
		user = 0;
	}
	for (j=0;j<N;j++)
	{//����ɨ��
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
			if (user == N)//�����к���ʤ��
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
	GameStatus *G = NULL;//���
	Pt P = {0,0};//����λ��
	int Ux,Uy;//�û�ѡ�����������
	int Result;//������ֽ��
	InitBlankGame(&G);//��ʼ�����
	method = SwitchMethod();//ѡ���Ⱥ���
	if (method == 1)
	{//�û����֣�����ͣ
		scanf("%d,%d",&Ux,&Uy);
	}
	else
	{//�������֣���ѡ�м�
		G->Game[N/2][N/2] = 1;
		ShowGame(*G);
		scanf("%d,%d",&Ux,&Uy);
	}
	G->Game[Ux][Uy] = method;//ִ���û�������
	ShowGame(*G);
	do
	{
		P = Calc(*G);
		G->Game[P.x][P.y] = (method == 1)?2:1;
		system("cls");
		ShowGame(*G);
		scanf("%d,%d",&Ux,&Uy);
		G->Game[Ux][Uy] = method;//ִ���û�������
		system("cls");
		ShowGame(*G);
	}while((Result = WinOrLose(*G)) == 0);
	if (Result == 1)
	{
		printf("ʵ�Ļ�ʤ\n");
	}
	else if (Result == 2)
	{
		printf("���Ļ�ʤ\n");
	}
	else
	{
		printf("ƽ��\n");
	}
	getch();
	return 0;
}