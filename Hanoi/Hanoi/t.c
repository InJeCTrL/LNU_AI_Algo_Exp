#include<stdio.h>
int move(int num,int src,int tmp,int targ)
{//�ƶ�src���ϵ�num��Բ�̵�targԲ���ϣ�ͨ��tmpԲ��
	if (num == 1)
	{
		printf("%d ==> %d\n",src,targ);
	}
	else if (num == 2)
	{
		printf("%d ==> %d\n",src,tmp);
		printf("%d ==> %d\n",src,targ);
		printf("%d ==> %d\n",tmp,targ);
	}
	else
	{
		move(num-1,src,targ,tmp);
		move(1,src,tmp,targ);
		move(num-1,tmp,src,targ);
	}
	return 0;
}

int main(void)
{
	int n = 5;//��ʼ��1�Ÿ��ϵ�����
	move(n,1,2,3);//��n�����Ӵ�1�Ÿ��Ƶ�3�Ÿˣ�����2����
	getchar();
	return 0;
}
