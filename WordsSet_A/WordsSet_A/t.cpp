/*
	谓词开头Pn(n为数字)，谓词括号内可有任意个常元Cn(n为数字)、变元Xn(n为数字)
	限制谓词括号内的常元变元只存放一个！！
	Skolem函数开头fn(n为数字，n同特称量词的n)
	限制特称量词与全称量词之后只能跟上一个变元！！
	设有已使用的特称量词列表、已占用的常元列表和变元列表

	符号列表：
	∧(合取)			&
	∨(析取)			|
	﹁(取反)			!
	→(蕴含)			#
	特称量词			@Xn
	全程量词			$Xn

	//Example:
	//	$X1(@X2P1(X2)#@X3P2(X3))
*/
#include<iostream>
#include<cstdio>
#include<string>
#include<list>
#include<map>
using namespace std;

string Input = "";//输入的谓词公式
string::iterator strit_In;//输入串迭代器
list<int> Cns = { 0 };//已占用的常元列表，1：占用，0：空闲
list<int> EXns = { 0 };//已使用的特称量词列表，1：已使用，0：未使用
list<int> Xns = { 0 };//已占用的变元列表，1：占用，0：空闲

string NextToken(void)
{//词法分析器，调用即获取下一个单词
	if (strit_In == Input.end())
		return "END";//迭代器扫描到底，返回END
	string tmp = "";
	switch (*strit_In)
	{
		case '&':case '|':case '!':case '#':case '(':case ')':
			tmp += *strit_In;
			strit_In++;
			return tmp;
			break;
		case '@'://@Xn	特称量词
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
		case '$'://$Xn	全称量词
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
		case 'f'://fn(Xn)	Skolem函数
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
	cin>>Input;//输入串
	strit_In = Input.begin();//输入字符串迭代器回到起点
	string ts;
	do
	{
		ts = NextToken();
		cout << ts << endl;
	} while (ts != "ERROR" && ts != "END");

	cin >> Input;
	return 0;
}