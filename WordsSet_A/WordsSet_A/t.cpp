/*
	ν�ʿ�ͷPn(nΪ����)��ν�������ڿ����������ԪCn(nΪ����)����ԪXn(nΪ����)
	����ν�������ڵĳ�Ԫ��Ԫֻ���һ������
	Skolem������ͷfn(nΪ���֣�nͬ�س����ʵ�n)
	�����س�������ȫ������֮��ֻ�ܸ���һ����Ԫ����
	������ʹ�õ��س������б���ռ�õĳ�Ԫ�б�ͱ�Ԫ�б�

	�����б�
	��(��ȡ)			&
	��(��ȡ)			|
	��(ȡ��)			!
	��(�̺�)			#
	�س�����			@Xn
	ȫ������			$Xn

	//Example:
	//	$X1(@X2P1(X2)#@X3P2(X3))
*/
#include<iostream>
#include<cstdio>
#include<string>
#include<list>
#include<map>
using namespace std;

string Input = "";//�����ν�ʹ�ʽ
string::iterator strit_In;//���봮������
list<int> Cns = { 0 };//��ռ�õĳ�Ԫ�б�1��ռ�ã�0������
list<int> EXns = { 0 };//��ʹ�õ��س������б�1����ʹ�ã�0��δʹ��
list<int> Xns = { 0 };//��ռ�õı�Ԫ�б�1��ռ�ã�0������

string NextToken(void)
{//�ʷ������������ü���ȡ��һ������
	if (strit_In == Input.end())
		return "END";//������ɨ�赽�ף�����END
	string tmp = "";
	switch (*strit_In)
	{
		case '&':case '|':case '!':case '#':case '(':case ')':
			tmp += *strit_In;
			strit_In++;
			return tmp;
			break;
		case '@'://@Xn	�س�����
			tmp += *strit_In;
			strit_In++;
			if (*strit_In == 'X')
			{
				tmp += "X";
				strit_In++;
				if (*strit_In == '0')
				{//@X0
					tmp += "0";
					strit_In++;
				}
				else if (isdigit(*strit_In))
				{//@X[1-~]
					do
					{
						tmp += *strit_In;
						strit_In++;
					} while (isdigit(*strit_In));
				}
				else
					return "ERROR";
			}
			else
				return "ERROR";
			return tmp;
			break;
		case '$'://$Xn	ȫ������
			tmp += *strit_In;
			strit_In++;
			if (*strit_In == 'X')
			{
				tmp += "X";
				strit_In++;
				if (*strit_In == '0')
				{//$X0
					tmp += "0";
					strit_In++;
				}
				else if (isdigit(*strit_In))
				{//$X[1-~]
					do
					{
						tmp += *strit_In;
						strit_In++;
					} while (isdigit(*strit_In));
				}
				else
					return "ERROR";
			}
			else
				return "ERROR";
			return tmp;
			break;
		case 'P'://Pn(Xn|Cn)
			tmp += *strit_In;
			strit_In++;
			if (*strit_In == '0')
			{//$P0
				tmp += "0";
				strit_In++;
			}
			else if (isdigit(*strit_In))
			{//$P[1-~]
				do
				{
					tmp += *strit_In;
					strit_In++;
				} while (isdigit(*strit_In));
			}
			else
				return "ERROR";
			if (*strit_In == '(')
			{//$Pn(
				tmp += "(";
				strit_In++;
				if (*strit_In == 'X')
				{//$Pn(X
					tmp += "X";
					strit_In++;
					if (*strit_In == '0')
					{//$Pn(X0
						tmp += "0";
						strit_In++;
					}
					else if (isdigit(*strit_In))
					{//$Pn(X[1-~]
						do
						{
							tmp += *strit_In;
							strit_In++;
						} while (isdigit(*strit_In));
					}
					else
						return "ERROR";
				}
				else if (*strit_In == 'C')
				{//$Pn(C
					tmp += "C";
					strit_In++;
					if (*strit_In == '0')
					{//$Pn(C0
						tmp += "0";
						strit_In++;
					}
					else if (isdigit(*strit_In))
					{//$Pn(C[1-~]
						do
						{
							tmp += *strit_In;
							strit_In++;
						} while (isdigit(*strit_In));
					}
					else
						return "ERROR";
				}
				else
					return "ERROR";
				if (*strit_In == ')')
				{//$Pn(Xn|Cn)
					tmp += ')';
					strit_In++;
					return tmp;
				}
				else
					return "ERROR";
			}
			else
				return "ERROR";
			return tmp;
			break;
		case 'f'://fn(Xn)	Skolem����
			tmp += 'f';
			strit_In++;
			if (*strit_In == '0')
			{//f0
				tmp += "0";
				strit_In++;
			}
			else if (isdigit(*strit_In))
			{//f[1-~]
				do
				{
					tmp += *strit_In;
					strit_In++;
				} while (isdigit(*strit_In));
			}
			else
				return "ERROR";
			if (*strit_In == '(')
			{//fn(
				tmp += '(';
				strit_In++;
			}
			else
				return "ERROR";
			if (*strit_In == 'X')
			{//fn(X
				tmp += 'X';
				strit_In++;
			}
			else
				return "ERROR";
			if (*strit_In == '0')
			{//fn(X0
				tmp += "0";
				strit_In++;
			}
			else if (isdigit(*strit_In))
			{//fn(X[1-~]
				do
				{
					tmp += *strit_In;
					strit_In++;
				} while (isdigit(*strit_In));
			}
			else
				return "ERROR";
			if (*strit_In == ')')
			{//fn(Xn)
				tmp += ')';
				strit_In++;
			}
			else
				return "ERROR";
			return tmp;
			break;
		default:
			return "ERROR";
			break;
	}
}

int main(void)
{
	cin>>Input;//���봮
	strit_In = Input.begin();//�����ַ����������ص����
	string ts;
	do
	{
		ts = NextToken();
		cout << ts << endl;
	} while (ts != "ERROR" && ts != "END");

	cin >> Input;
	return 0;
}