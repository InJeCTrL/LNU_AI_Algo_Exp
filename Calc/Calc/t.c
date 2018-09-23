#include<stdio.h>
#include<stdlib.h>
typedef struct NumStackNode
{
	long num;
	struct NumStackNode *next;
}NumStackNode,*pNumStack;//数字栈节点
typedef struct OpStackNode
{
	char Op;
	struct OpStackNode *next;
}OpStackNode,*pOpStack;//符号栈节点
pNumStack InitNumStack(void)
{//初始化数字栈，成功返回栈指针，失败返回NULL
	pNumStack tNumStack = (pNumStack)malloc(sizeof(NumStackNode));
	if (tNumStack)
		tNumStack->next = NULL;
	return tNumStack;
}
pOpStack InitOpStack(void)
{//初始化字符栈，成功返回栈指针，失败返回NULL
	pOpStack tOpStack = (pOpStack)malloc(sizeof(OpStackNode));
	if (tOpStack)
		tOpStack->next = NULL;
	return tOpStack;
}
int PushNumStack(long num,pNumStack _pNumStack)
{//数字入栈，成功返回0，失败返回1
	pNumStack tNode = (pNumStack)malloc(sizeof(NumStackNode));
	if (tNode)
	{
		tNode->num = num;
		tNode->next = _pNumStack->next;
		_pNumStack->next = tNode;
		return 0;
	}
	return 1;
}
int PushOpStack(char Op,pOpStack _pOpstack)
{//数字入栈，成功返回0，失败返回1
	pOpStack tNode = (pOpStack)malloc(sizeof(OpStackNode));
	if (tNode)
	{
		tNode->Op = Op;
		tNode->next = _pOpstack->next;
		_pOpstack->next = tNode;
		return 0;
	}
	return 1;
}
long PopNumStack(pNumStack _pNumStack)
{//数字栈出栈
	long t;
	pNumStack pStackNode = _pNumStack->next;
	_pNumStack->next = pStackNode->next;
	t = pStackNode->num;
	free(pStackNode);
	return t;
}
char PopOpStack(pOpStack _pOpStack)
{//符号栈出栈
	char t;
	pOpStack pStackNode = _pOpStack->next;
	_pOpStack->next = pStackNode->next;
	t = pStackNode->Op;
	free(pStackNode);
	return t;
}
char GetLastOp(pOpStack _pOpStack)
{//获取上一个入栈的符号，返回NULL无入栈的符号，<char>上一个入栈的符号
	if (!_pOpStack->next)
		return NULL;
	return _pOpStack->next->Op;
}
int IsNumEmpty(pNumStack _pNumStack)
{//返回1数字栈空，0数字栈非空
	if (!_pNumStack->next)
		return 1;
	return 0;
}
#if 0
int CheckCh(char ch)
{//检查输入的字符是否合法，返回0操作符，1数字，2其它
	switch (ch)
	{
	case '+':case '-':case '*':case '/':case '(':case ')':
		return 0;
	default:
		if (ch<='9' && ch >= '0')
			return 1;
		else
			return 2;
	}
}
#endif//废弃的方法
#if 0
int StrProc(char *pStr_input,char **pStr_2)
{//处理输入字符串，返回2中间字符串内存分配异常，1输入不合法，0处理完成
	int result;//输入字符类型
	*pStr_2 = (char*)malloc(1024*sizeof(char));
	if (!*pStr_2)
		return 2;
	if (*pStr_input == '+' || *pStr_input == '-')
	{
		*(*pStr_2++) = '(';
		*(*pStr_2++) = '0';
		*(*pStr_2++) = *(pStr_input++);
		*(*pStr_2++) = ')';//将数字的正负号转为(0±n)
	}
	while (*pStr_input)//循环录入
	{
		result = CheckCh(*pStr_input);//获取输入字符类型
		if (result == 2)
			return 1;
		else
		{
			if (CheckCh(*(pStr_input-1)) == 0 && ((*pStr_input == '+') || (*pStr_input == '-')))
			{
				*(*pStr_2++) = '(';
				*(*pStr_2++) = '0';
				*(*pStr_2++) = *(pStr_input++);
				*(*pStr_2++) = ')';//将数字的正负号转为(0±n)
			}
			else
			{
				*(*pStr_2++) = *(pStr_input++);
			}
		}
	}
	**pStr_2 = 0;//中间串结束
}
#endif//废弃的方法
int CheckandDelSpc(char *pStr_input)
{//检查是否有异常输入并去除空格，返回1输入异常，0去空格完成
	char *tStr = (char*)malloc(1024*sizeof(char));
	char *ptStr = tStr,//指向临时串头
		 *pinpStr = pStr_input;//指向目的串头
	while(*pStr_input)
	{
		if (*pStr_input == ' ')//去空格
			pStr_input++;
		else if (*pStr_input != '+' && *pStr_input != '-' && *pStr_input != '*' &&
				 *pStr_input != '/' && *pStr_input != '(' && *pStr_input != ')' &&
				 (*pStr_input < '0' || *pStr_input > '9'))//检查输入合法性
		{
			return 1;
		}
		else//输入正确
		{
			*(tStr++) = *(pStr_input++);
		}
	}
	*tStr = 0;
	while(*ptStr)
		*(pinpStr++) = *(ptStr++);//复制去空格后的串
	*pinpStr = 0;//中缀表达式结尾
	return 0;
}
int DoCalc(pOpStack _pOpStack,pNumStack _pNumStack,long *result)
{//用于退栈时执行一次计算，返回0计算完成，1除以零异常，2输入非法
	char tOp;
	long t2,t1;
	if (GetLastOp(_pOpStack) && GetLastOp(_pOpStack) != '(')
		tOp = PopOpStack(_pOpStack);//上一个符号出栈
	else
		return 2;
	if (!IsNumEmpty(_pNumStack))
		t2 = PopNumStack(_pNumStack);//第二个数字出栈
	else
		return 2;
	if (!IsNumEmpty(_pNumStack))
		t1 = PopNumStack(_pNumStack);//第一个数字出栈
	else
		return 2;
	switch (tOp)
	{
		case '+':
			*result = t1 + t2;
			break;
		case '-':
			*result = t1 - t2;
			break;
		case '*':
			*result = t1 * t2;
			break;
		case '/':
			if (t2 == 0)
				return 1;
			*result = t1 / t2;
			break;
	}
	return 0;
}
int Calc(char *pStr_input,long *result,pNumStack pNum,pOpStack pOp)
{//中缀运算式直接运算，返回0运算完成，1除以0错误，2输入不合法
	char ch;
	char *pBegin = pStr_input;//记录开始点
	int isneg = 0;//标记是否负数
	long tnum;//临时保存数字
	while (*pStr_input)
	{
		ch = *pStr_input;
		switch(ch)
		{
			case '*':case '/':case '(':
				PushOpStack(ch,pOp);//符号入栈
				pStr_input++;
				break;
			case '+':
				if (*(pStr_input-1) == ')' ||//+之前为右括号表示加号
					(*(pStr_input-1) >= '0' && *(pStr_input-1) <= '9'))//+之前为数字表示加号
				{
					while (GetLastOp(pOp) == '*' || GetLastOp(pOp) == '/')//若优先级高于+
					{
						switch (DoCalc(pOp,pNum,&tnum))
						{
						case 1:
							return 1;
						case 2:
							return 2;
						default:
							PushNumStack(tnum,pNum);//执行退栈计算并将计算结果重新入栈
							break;
						}
					}
					PushOpStack('+',pOp);//符号入栈
				}
				pStr_input++;
				break;
			case '-':
				if (*(pStr_input-1) == ')' ||//-之前为右括号表示减号
					(*(pStr_input-1) >= '0' && *(pStr_input-1) <= '9'))//-之前为数字表示减号
				{
					while (GetLastOp(pOp) == '*' || GetLastOp(pOp) == '/')//若优先级高于-
					{
						switch (DoCalc(pOp,pNum,&tnum))
						{
						case 1:
							return 1;
						case 2:
							return 2;
						default:
							PushNumStack(tnum,pNum);//执行退栈计算并将计算结果重新入栈
							break;
						}
					}
					PushOpStack('-',pOp);//符号入栈
				}
				else
				{
					isneg = 1;//标记是负数
				}
				pStr_input++;
				break;
			case ')':
				while (GetLastOp(pOp) != '(')//若栈顶符号不为左括号
				{
					switch (DoCalc(pOp,pNum,&tnum))
					{
					case 1:
						return 1;
					case 2:
						return 2;
					default:
						PushNumStack(tnum,pNum);//执行退栈计算并将计算结果重新入栈
						break;
					}
				}
				PopOpStack(pOp);//左括号出栈
				pStr_input++;
				break;
			default:
				tnum = atoi(pStr_input);
				if (isneg)
				{//负数
					tnum = -tnum;
					isneg = 0;
				}
				PushNumStack(tnum,pNum);//数字入栈
				while (*pStr_input >= '0' && *pStr_input <= '9')
					pStr_input++;
				break;
		}
	}
	while (GetLastOp(pOp))
	{
		switch (DoCalc(pOp,pNum,&tnum))
		{
		case 1:
			return 1;
		case 2:
			return 2;
		default:
			PushNumStack(tnum,pNum);//最后的计算并入栈
			break;
		}
	}
	*result = tnum;
	return 0;
}

int main(int argc,char *argv[])
{
	pNumStack NumStack = NULL;//数字栈
	pOpStack OpStack = NULL;//运算符栈
	char *pStr_input;//指向输入的运算式
	long result;//运算结果
	NumStack = InitNumStack();//尝试初始化数字栈
	if (!NumStack)//数字栈初始化失败
	{
		printf("初始化数字栈失败！\n");
		getchar();
		return -1;
	}
	OpStack = InitOpStack();//尝试初始化符号栈
	if (!OpStack)//符号栈初始化失败
	{
		free(NumStack);//回滚数字栈的创建
		printf("初始化符号栈失败！\n");
		getchar();
		return -1;
	}

	if (argc > 1)//四则运算式已输入
	{
		pStr_input = argv[1];
	}
	else//运行时输入运算式
	{
		pStr_input = (char*)malloc(1024*sizeof(char));
		gets_s(pStr_input,1024);
	}

	if (CheckandDelSpc(pStr_input) == 1)//检查输入正确性并去空格
	{
		printf("运算式输入不合法！\n");
		getchar();
		return -1;
	}

	switch (Calc(pStr_input,&result,NumStack,OpStack))
	{
	case 1:
		printf("除以0错误！\n");
		getchar();
		return -1;
	case 2:
		printf("运算式输入不合法！\n");
		getchar();
		return -1;
	default:
		printf("= %ld",result);//输出运算结果
		getchar();
		break;
	}

	return 0;
}
