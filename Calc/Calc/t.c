#include<stdio.h>
#include<stdlib.h>
typedef struct NumStackNode
{
	long num;
	struct NumStackNode *next;
}NumStackNode,*pNumStack;//����ջ�ڵ�
typedef struct OpStackNode
{
	char Op;
	struct OpStackNode *next;
}OpStackNode,*pOpStack;//����ջ�ڵ�
pNumStack InitNumStack(void)
{//��ʼ������ջ���ɹ�����ջָ�룬ʧ�ܷ���NULL
	pNumStack tNumStack = (pNumStack)malloc(sizeof(NumStackNode));
	if (tNumStack)
		tNumStack->next = NULL;
	return tNumStack;
}
pOpStack InitOpStack(void)
{//��ʼ���ַ�ջ���ɹ�����ջָ�룬ʧ�ܷ���NULL
	pOpStack tOpStack = (pOpStack)malloc(sizeof(OpStackNode));
	if (tOpStack)
		tOpStack->next = NULL;
	return tOpStack;
}
int PushNumStack(long num,pNumStack _pNumStack)
{//������ջ���ɹ�����0��ʧ�ܷ���1
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
{//������ջ���ɹ�����0��ʧ�ܷ���1
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
{//����ջ��ջ
	long t;
	pNumStack pStackNode = _pNumStack->next;
	_pNumStack->next = pStackNode->next;
	t = pStackNode->num;
	free(pStackNode);
	return t;
}
char PopOpStack(pOpStack _pOpStack)
{//����ջ��ջ
	char t;
	pOpStack pStackNode = _pOpStack->next;
	_pOpStack->next = pStackNode->next;
	t = pStackNode->Op;
	free(pStackNode);
	return t;
}
char GetLastOp(pOpStack _pOpStack)
{//��ȡ��һ����ջ�ķ��ţ�����NULL����ջ�ķ��ţ�<char>��һ����ջ�ķ���
	if (!_pOpStack->next)
		return NULL;
	return _pOpStack->next->Op;
}
int IsNumEmpty(pNumStack _pNumStack)
{//����1����ջ�գ�0����ջ�ǿ�
	if (!_pNumStack->next)
		return 1;
	return 0;
}
#if 0
int CheckCh(char ch)
{//���������ַ��Ƿ�Ϸ�������0��������1���֣�2����
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
#endif//�����ķ���
#if 0
int StrProc(char *pStr_input,char **pStr_2)
{//���������ַ���������2�м��ַ����ڴ�����쳣��1���벻�Ϸ���0�������
	int result;//�����ַ�����
	*pStr_2 = (char*)malloc(1024*sizeof(char));
	if (!*pStr_2)
		return 2;
	if (*pStr_input == '+' || *pStr_input == '-')
	{
		*(*pStr_2++) = '(';
		*(*pStr_2++) = '0';
		*(*pStr_2++) = *(pStr_input++);
		*(*pStr_2++) = ')';//�����ֵ�������תΪ(0��n)
	}
	while (*pStr_input)//ѭ��¼��
	{
		result = CheckCh(*pStr_input);//��ȡ�����ַ�����
		if (result == 2)
			return 1;
		else
		{
			if (CheckCh(*(pStr_input-1)) == 0 && ((*pStr_input == '+') || (*pStr_input == '-')))
			{
				*(*pStr_2++) = '(';
				*(*pStr_2++) = '0';
				*(*pStr_2++) = *(pStr_input++);
				*(*pStr_2++) = ')';//�����ֵ�������תΪ(0��n)
			}
			else
			{
				*(*pStr_2++) = *(pStr_input++);
			}
		}
	}
	**pStr_2 = 0;//�м䴮����
}
#endif//�����ķ���
int CheckandDelSpc(char *pStr_input)
{//����Ƿ����쳣���벢ȥ���ո񣬷���1�����쳣��0ȥ�ո����
	char *tStr = (char*)malloc(1024*sizeof(char));
	char *ptStr = tStr,//ָ����ʱ��ͷ
		 *pinpStr = pStr_input;//ָ��Ŀ�Ĵ�ͷ
	while(*pStr_input)
	{
		if (*pStr_input == ' ')//ȥ�ո�
			pStr_input++;
		else if (*pStr_input != '+' && *pStr_input != '-' && *pStr_input != '*' &&
				 *pStr_input != '/' && *pStr_input != '(' && *pStr_input != ')' &&
				 (*pStr_input < '0' || *pStr_input > '9'))//�������Ϸ���
		{
			return 1;
		}
		else//������ȷ
		{
			*(tStr++) = *(pStr_input++);
		}
	}
	*tStr = 0;
	while(*ptStr)
		*(pinpStr++) = *(ptStr++);//����ȥ�ո��Ĵ�
	*pinpStr = 0;//��׺���ʽ��β
	return 0;
}
int DoCalc(pOpStack _pOpStack,pNumStack _pNumStack,long *result)
{//������ջʱִ��һ�μ��㣬����0������ɣ�1�������쳣��2����Ƿ�
	char tOp;
	long t2,t1;
	if (GetLastOp(_pOpStack) && GetLastOp(_pOpStack) != '(')
		tOp = PopOpStack(_pOpStack);//��һ�����ų�ջ
	else
		return 2;
	if (!IsNumEmpty(_pNumStack))
		t2 = PopNumStack(_pNumStack);//�ڶ������ֳ�ջ
	else
		return 2;
	if (!IsNumEmpty(_pNumStack))
		t1 = PopNumStack(_pNumStack);//��һ�����ֳ�ջ
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
{//��׺����ʽֱ�����㣬����0������ɣ�1����0����2���벻�Ϸ�
	char ch;
	char *pBegin = pStr_input;//��¼��ʼ��
	int isneg = 0;//����Ƿ���
	long tnum;//��ʱ��������
	while (*pStr_input)
	{
		ch = *pStr_input;
		switch(ch)
		{
			case '*':case '/':case '(':
				PushOpStack(ch,pOp);//������ջ
				pStr_input++;
				break;
			case '+':
				if (*(pStr_input-1) == ')' ||//+֮ǰΪ�����ű�ʾ�Ӻ�
					(*(pStr_input-1) >= '0' && *(pStr_input-1) <= '9'))//+֮ǰΪ���ֱ�ʾ�Ӻ�
				{
					while (GetLastOp(pOp) == '*' || GetLastOp(pOp) == '/')//�����ȼ�����+
					{
						switch (DoCalc(pOp,pNum,&tnum))
						{
						case 1:
							return 1;
						case 2:
							return 2;
						default:
							PushNumStack(tnum,pNum);//ִ����ջ���㲢��������������ջ
							break;
						}
					}
					PushOpStack('+',pOp);//������ջ
				}
				pStr_input++;
				break;
			case '-':
				if (*(pStr_input-1) == ')' ||//-֮ǰΪ�����ű�ʾ����
					(*(pStr_input-1) >= '0' && *(pStr_input-1) <= '9'))//-֮ǰΪ���ֱ�ʾ����
				{
					while (GetLastOp(pOp) == '*' || GetLastOp(pOp) == '/')//�����ȼ�����-
					{
						switch (DoCalc(pOp,pNum,&tnum))
						{
						case 1:
							return 1;
						case 2:
							return 2;
						default:
							PushNumStack(tnum,pNum);//ִ����ջ���㲢��������������ջ
							break;
						}
					}
					PushOpStack('-',pOp);//������ջ
				}
				else
				{
					isneg = 1;//����Ǹ���
				}
				pStr_input++;
				break;
			case ')':
				while (GetLastOp(pOp) != '(')//��ջ�����Ų�Ϊ������
				{
					switch (DoCalc(pOp,pNum,&tnum))
					{
					case 1:
						return 1;
					case 2:
						return 2;
					default:
						PushNumStack(tnum,pNum);//ִ����ջ���㲢��������������ջ
						break;
					}
				}
				PopOpStack(pOp);//�����ų�ջ
				pStr_input++;
				break;
			default:
				tnum = atoi(pStr_input);
				if (isneg)
				{//����
					tnum = -tnum;
					isneg = 0;
				}
				PushNumStack(tnum,pNum);//������ջ
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
			PushNumStack(tnum,pNum);//���ļ��㲢��ջ
			break;
		}
	}
	*result = tnum;
	return 0;
}

int main(int argc,char *argv[])
{
	pNumStack NumStack = NULL;//����ջ
	pOpStack OpStack = NULL;//�����ջ
	char *pStr_input;//ָ�����������ʽ
	long result;//������
	NumStack = InitNumStack();//���Գ�ʼ������ջ
	if (!NumStack)//����ջ��ʼ��ʧ��
	{
		printf("��ʼ������ջʧ�ܣ�\n");
		getchar();
		return -1;
	}
	OpStack = InitOpStack();//���Գ�ʼ������ջ
	if (!OpStack)//����ջ��ʼ��ʧ��
	{
		free(NumStack);//�ع�����ջ�Ĵ���
		printf("��ʼ������ջʧ�ܣ�\n");
		getchar();
		return -1;
	}

	if (argc > 1)//��������ʽ������
	{
		pStr_input = argv[1];
	}
	else//����ʱ��������ʽ
	{
		pStr_input = (char*)malloc(1024*sizeof(char));
		gets_s(pStr_input,1024);
	}

	if (CheckandDelSpc(pStr_input) == 1)//���������ȷ�Բ�ȥ�ո�
	{
		printf("����ʽ���벻�Ϸ���\n");
		getchar();
		return -1;
	}

	switch (Calc(pStr_input,&result,NumStack,OpStack))
	{
	case 1:
		printf("����0����\n");
		getchar();
		return -1;
	case 2:
		printf("����ʽ���벻�Ϸ���\n");
		getchar();
		return -1;
	default:
		printf("= %ld",result);//���������
		getchar();
		break;
	}

	return 0;
}
