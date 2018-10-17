#include<stdio.h>
int move(int num,int src,int tmp,int targ)
{//移动src塔上的num个圆盘到targ圆盘上，通过tmp圆盘
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
	int n = 5;//初始在1号杆上的盘数
	move(n,1,2,3);//将n个盘子从1号杆移到3号杆，借助2号盘
	getchar();
	return 0;
}
