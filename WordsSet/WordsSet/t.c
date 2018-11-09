//$x0((@x1(P(x1)))#(@x2(Q(x2))))
#include<stdio.h>
#include<windows.h>
int C[10] = {0};//常元列表，1：已选，0：未选
int X[10] = {0};//变元列表，1：已选，0：未选
int Sk[10] = {0};//思科函数函数名f开头，1：已选，0：未选
//∧			&
//∨			|
//﹁			!
//→			#
//特称量词	@
//全程量词	$
int Deletech(char *ptr)
{//删除指定位置的符号
	while (*ptr)
	{
		*ptr = *(ptr + 1);
		ptr++;
	}
	return 0;
}
int InsertNeg(char *ptr)
{//在ptr指向前插入取反，插入取反后指针指向取反
	char *p = ptr + strlen(ptr);
	while (p != ptr)
	{//插入点及插入点后的内容向后移动一位
		*p = *(p - 1);
		p--;
	}
	*ptr = '!';//插入取反
	return 0;
}
char* NoAtom(char *ptr,int d)
{//跳过谓词的变元与常元，d控制扫描方向0：向右(传进时ptr指向谓词) 1：向左(传进时ptr指向右括号之后的一个字符)
	int L = 0,R = 0;//左右括号数
	if (d == 0)
	{//从左向右
		L = 1;
		ptr += 2;//指针跳过谓词后的第一个括号
		while (L != R)
		{//跳过变原、常元
			if (*ptr == ')')
			{
				R++;
			}
			else if (*ptr == '(')
			{
				L++;
			}
			ptr++;
		}
		return ptr - 1;//传回时ptr指向该谓词的右括号
	}
	else
	{//从右向左
		R = 1;
		ptr -= 2;//指针跳过谓词后的最后一个括号
		while (L != R)
		{//跳过变原、常元
			if (*ptr == ')')
			{
				R++;
			}
			else if (*ptr == '(')
			{
				L++;
			}
			ptr++;
		}
		return ptr;//传回时ptr指向该谓词的谓词名
	}
}
char* NoAtom2(char *ptr,int d)
{//跳过限定变元，d控制扫描方向0：向右(输入时ptr指向量词符号) 1：向左(输入时ptr指向量词变元的最后一个数字表示的后一位字符)
	if (d == 0)
	{
		ptr += 2;//跳过量词符号和x
		while (isdigit(*ptr))
		{//跳过所有数字标识
			ptr++;
		}
		ptr--;
		return ptr;//返回时ptr指向最后一个元数字标识
	}
	else
	{
		ptr--;//进入量词符号变元的数字标识部分
		while (isdigit(*ptr))
		{//跳过所有数字标识
			ptr--;
		}
		ptr--;
		return ptr;//返回时ptr指向量词符号
	}
}
char* SkipItem(char *ptr,int d)
{//跳过整项，传入时ptr指向括号，d控制方向，返回两端
	int L = 0,R = 0;//左右括号数量
	if (d == 0)
	{
		L = 1;
		ptr++;
		while (*ptr)
		{
			if (*ptr == '(')
				L++;
			else if (*ptr == ')')
			{
				R++;
				if (L == R)
					break;
			}
			ptr++;
		}
	}
	else
	{
		R = 1;
		ptr--;
		while (*ptr)
		{
			if (*ptr == '(')
			{
				L++;
				if (L == R)
					break;
			}
			else if (*ptr == ')')
				R++;
			ptr--;
		}
	}
	return ptr;
}
int DealNeg(char *str) 
{//处理谓词公式的取反
	char *ptr = str;
	int Layer = 0;//标记当前进行转换的层级
	int DoNeg[30] = {0};//决定指定层级是否进行取反操作
	while (*ptr)
	{//不支持输入类似!Ex0、!Vx0语句
		if (*ptr == '!' && *(ptr + 1) == '(')
		{//捕获到取反，则对当前层级进行取反操作
			Layer++;
			DoNeg[Layer] = 1 - DoNeg[Layer - 1];
			Deletech(ptr);//删除取反符号
		}
		else if (*ptr == '(' && (*(ptr + 1) < 'a' || *(ptr + 1) > 'z'))
		{//捕获到非原子谓词公式的左括号，则层级+1
			Layer++;
			DoNeg[Layer] = DoNeg[Layer - 1];
		}
		else if (*ptr == ')' && !isdigit(*(ptr-1)))
		{//捕获到不属于原子谓词的右括号
			DoNeg[Layer] = 0;//当前层级结束，取反标志归零
			Layer--;//层级回滚-1
		}

		if (DoNeg[Layer])
		{//该层级要求取反再执行层级取反操作
			if (*ptr >= 'A' && *ptr <= 'Z')
			{//遇到谓词
				InsertNeg(ptr);//在谓词前插入取反
				ptr++;
				ptr = NoAtom(ptr,0);//跳过此谓词对应的常元、变元
			}
			else if (*ptr == '&')
			{//合取改析取
				*ptr = '|';
			}
			else if (*ptr == '|')
			{//析取改合取
				*ptr = '&';
			}
			else if (*ptr == '@')
			{//遇到特称符号
				*ptr = '$';//特称改全称
				ptr = NoAtom2(ptr,0);
			}
			else if (*ptr == '$')
			{//遇到全称符号
				*ptr = '@';//全称改特称
				ptr = NoAtom2(ptr,0);
			}
		}
		else if (DoNeg[Layer] == 0 && *ptr >= 'A' && *ptr <= 'Z')
		{//该层级不要求取反，且遇到原子谓词
			ptr = NoAtom(ptr,0);//跳过此谓词
		}

		ptr++;
	}
	
	return 0;
}
int ModifyCont(char *str)
{//去除蕴含,1：谓词公式异常，0：成功
	char *ptr = str;
	while (*ptr)
	{//向后寻找蕴含式
		if (*ptr == '#')
		{//遇到蕴含符号
			*ptr = '|';//蕴含符号改为析取符号
			ptr--;
			ptr = SkipItem(ptr,1);//向前找到前一项的开头括号
			if (*(ptr - 1) == '!')
				Deletech(ptr-1);//删除括号前的取反符号
			else
				InsertNeg(ptr);//在项目开头的括号前加入取反符号
			ptr = str;
			continue;
		}
		ptr++;
	}
	/*
	char *Begin = str;//开头指针
	char *ptr = NULL;//动态游标
	int getAO = 0;//标志是否找到合取与析取
	int L = 0,R = 0;//左括号数、右括号数
	ptr = str;//游标指针先指向谓词公式开头
	while (*ptr)
	{//扫描整个谓词公式
		if (*ptr == '#')//找到蕴含关系
		{
			ptr--;//指针向前跳转一位
			if (*ptr != ')')
				return 1;//谓词公式有语法错误
			R++;//右括号数+1
			ptr--;//指针向前跳转一位
			if (*ptr == ')')
			{//前一位仍是右括号，代表前方是一个小型的谓词集
				R++;//右括号数+1
				while (ptr != Begin)
				{//未走到起始位置，为了定位到谓词公式的头部
					if (*ptr == ')')
						R++;
					else if (*ptr == '(')
					{
						L++;
						if (L == R)
						{//左右括号平衡，谓词公式获取完毕
							if (*(ptr-1) == '!')
								Deletech(ptr-1);//若谓词公式头的前一位是取反，则删除这个取反符
							else
								InsertNeg(ptr);//在ptr指向的字符之前插入取反符号
							break;
						}
					}
					ptr--;
				}
			}
			else if (isdigit(*ptr))
			{//前一位是数字，代表前方只有一个原子谓词
				while (ptr != Begin)
				{//未走到起始位置，为了定位到原子谓词
					if (*ptr == '(')
					{//已找到原子谓词的左括号
						L++;
						break;
					}
					ptr--;
				}
				if (L != R)
					return 1;//谓词公式语法错误
				ptr--;//定位到原子谓词的原子(大写字母)
				while (ptr != Begin)
				{//未走到起始位置，为了定位到全称、特称之前
					if (*ptr == 'A' || *ptr == 'O')//动态游标定位的是合取或析取
					{
						getAO = 1;//找到了合取/析取
						break;//不再循环
					}
					ptr--;
				}
				if (getAO == 1)
				{//当前位是原子谓词的头
					if (*(ptr-1) == '!')
						Deletech(ptr-1);//若原子谓词头的前一位是取反，则删除这个取反符
					else
						InsertNeg(ptr);//在ptr指向的字符之前插入取反符号
				}
			}
			else
				return 1;//谓词公式语法错误
			ptr = Begin;//动态指针重新指向谓词公式开头
		}
		ptr++;
	}
	*/
	return 0;
}
int DealSame1(char *str)
{//处理相同约束变元
	int Same[30][10] = {0};//标记该层是否已出现某个约束变元，若约束变元在对应的层的Same为1则改名，共30层，最多100个约束变元
	char *ptr = str;
	int Layer = 0;//用于标记当前的层级
	int Layer_bak;//用于标记找到重复约束变元的层级
	int i;
	while (*ptr)
	{
		if (*ptr == 'P' || *ptr == 'Q' || *ptr == 'R')
			NoAtom(ptr,0);//若找到了谓词，则跳过谓词
		else if (*ptr == '(')
			Layer++;//若找到左括号则层级数+1
		else if (*ptr == ')')
			Layer--;//若找到右括号则层级数-1
		if (*ptr == 'x')
		{//捕获到一个约束变元
			if (Same[Layer][atoi(ptr+1)] == 1)
			{//该约束变元重复
				for (i=0;i<10;i++)
				{
					if (Same[Layer][i] == 0)
					{//若找到一个未被使用过的约束变元
						Same[Layer][i] = 1;//标记该约束变元已被使用
						*(ptr + 1) = i + 0x30;//修改约束变元
						Layer_bak = Layer;//保存发现重复约束变元的层级
						while (*ptr)
						{//进入被约束的范围
							if (*ptr == '(')
							{
								Layer++;
								ptr++;
								break;
							}
							ptr++;
						}
						while (Layer_bak < Layer)
						{//被约束范围未结束
							if (*ptr == '(')
								Layer++;
							else if (*ptr == ')')
								Layer--;
							else if (*ptr == 'x' && isdigit(*(ptr+1)))
								*(ptr + 1) = i + 0x30;//修改被约束的变元
							ptr++;
						}
						break;
					}
				}
			}
			else//约束变元不重复
				Same[Layer][atoi(ptr+1)] = 1;//找到某个约束变元，对应层级的变元标记为有重复
		}
		ptr++;
	}
	return 0;
}
int ReplaceBySk(char *ptr,int PartID,int AllID)
{//ptr指向的变元用sk函数代替
	char *tptr = ptr;
	int len = strlen(ptr);
	tptr = ptr + len + 3;
	while (tptr - 4 >= ptr)
	{
		*tptr = *(tptr - 4);
		tptr--;
	}
	*ptr = 'f';
	*(ptr + 1) = PartID + 0x30;
	*(ptr + 2) = '(';
	*(ptr + 3) = 'x';
	*(ptr + 4) = AllID + 0x30;
	*(ptr + 5) = ')';
	return 0;
}
int SkFunc(char *str)
{//将特称量词用思科函数替代
	char *ptr = str;
	int AllID[30] = {0};//全称量词的ID
	int PartID;//特称量词的ID
	int Layer_bak = 0,Layer = 0;//全称量词层级，当前层级
	while (*ptr)
	{
		if (*ptr == '(')
			Layer++;
		else if (*ptr == ')')
		{
			Layer--;
			if (Layer_bak >= 1)
				Layer_bak--;
		}
		else if (*ptr == '$')
		{//捕获到全称量词
			Layer_bak = Layer;
			AllID[Layer_bak] = atoi(ptr + 2);
		}
		else if (*ptr == '@')
		{//捕获到特称量词
			PartID = atoi(ptr + 2);
			Deletech(ptr);
			Deletech(ptr);
			Deletech(ptr);
			if (Layer > Layer_bak)
			{//需要思科
				while (*ptr)
				{//进入被约束的范围
					if (*ptr == '(')
					{
						Layer++;
						ptr++;
						break;
					}
					ptr++;
				}
				while (Layer_bak < Layer)
				{//被约束范围未结束
					if (*ptr == '(')
						Layer++;
					else if (*ptr == ')')
						Layer--;
					else if (*ptr == 'x' && isdigit(*(ptr+1)))
					{
						ReplaceBySk(ptr,PartID,AllID[Layer_bak]);//修改被约束的变元，用sk函数替换
						break;
					}
					ptr++;
				}
			}
			else
			{//改为字母即可
				while (*ptr)
				{//进入被约束的范围
					if (*ptr == '(')
					{
						Layer++;
						ptr++;
						break;
					}
					ptr++;
				}
				while (Layer_bak < Layer)
				{//被约束范围未结束
					if (*ptr == '(')
						Layer++;
					else if (*ptr == ')')
						Layer--;
					else if (*ptr == 'x' && isdigit(*(ptr+1)))
					{//改为常元
						*ptr = 'a';
						Deletech(ptr+1);
					}
					ptr++;
				}
			}
			Layer_bak = 0;
			Layer = 0;
			ptr = str;
			continue;
		}
		ptr++;
	}
	return 0;
}
int DeleteAllSign(char *str)
{//删除全称量词
	char *ptr = str;
	while (*ptr)
	{
		if (*ptr == '$')
		{//找到全称量词，则删除
			Deletech(ptr);
			Deletech(ptr);
			Deletech(ptr);
		}
		else
			ptr++;
	}
	return 0;
}
char* APart(char *str)
{//是否需要分离，是：<ptr>，否：NULL
	char *ptr = str;
	while (*ptr)
	{
		if (*ptr == '&')
			return ptr;
		ptr++;
	}
	return NULL;
}

int main(void)
{
	char Input[999] = {0};//输入的谓词公式

	gets(Input);
	if (ModifyCont(Input))
		printf("修改蕴含式异常 - 语法错误！\n");
	else
	{
		printf("去除蕴含：\n%s\n",Input);//输出蕴含式处理结果
		if (DealNeg(Input))
			printf("处理取反异常 - 语法错误！\n");
		else
		{
			printf("取反化简：\n%s\n",Input);//输出取反处理结果
			if (DealSame1(Input))
				printf("处理量词间相同变元异常 - 语法错误！\n");
			else
			{
				printf("处理量词间相同变元：\n%s\n",Input);//输出量词间相同变元处理结果
				if (SkFunc(Input))
					printf("处理特称量词异常 - 语法错误！\n");
				else
				{
					printf("处理特称量词：\n%s\n",Input);//输出特称量词处理结果
					DeleteAllSign(Input);
					printf("删除全称量词：\n%s\n",Input);//输出全称量词处理结果
					if (APart(Input) == NULL)
						printf("无需删除字句间重复变元，无需去除合取符号：\n%s\n",Input);//无需拆分时直接输出字句集
				}
			}
		}
	}

	getchar();
	return 0;
}