#include<iostream>
#include<string>
using namespace std;
int isillegal(char *st1,char *st2)
{//检查st1和st2中是否有相同变元，1：有(非法)，0：无(合法)
	string var[] = {"x","y","z","u","v","w"};
	string str1 = st1;
	string str2 = st2;
	int i;
	for (i=0;i<6;i++)
		if (str1.find(var[i]) != str1.npos && str2.find(var[i]) != str2.npos)
			return 1;
	return 0;
}
int main(void)
{
	int n;//变/常元总个数
	int i,j;
	char *head1 = NULL,*tail1 = NULL,*head2 = NULL,*tail2 = NULL;//四个指针，用于比对原子
	string P1[10] = {""},P2[10] = {""};//P1、P2变/常元列表
	string t = "";//临时保存替换结果
	string MGU[10] = {""};//最一般合一列表
	cout<<"常元限定在abc，变元限定在xyzuvw\n变/常元总个数(不超过10):";
	cin>>n;
	cout<<"谓词P1的变/常元列表( "<<n<<" 个):";
	for (i=0;i<n;i++)
	{
		cin>>P1[i];
	}
	cout<<"谓词P2的变/常元列表( "<<n<<" 个):";
	for (i=0;i<n;i++)
	{
		cin>>P2[i];
	}
	for (i=0;i<n;i++)
	{
		head1 = const_cast<char*>(P1[i].c_str());
		tail1 = const_cast<char*>(P1[i].c_str()) + P1[i].length() - 1;//head1和tail1指在谓词1的第i个原子的头尾
		head2 = const_cast<char*>(P2[i].c_str());
		tail2 = const_cast<char*>(P2[i].c_str()) + P2[i].length() - 1;//head2和tail2指在谓词2的第i个原子的头尾
		if (!isillegal(head1,head2))
		{
			while (*head1 == *head2)
			{//跳过头部相同区间
				head1++;
				head2++;
			}
			while (*tail1 == *tail2)
			{//跳过尾部相同区间
				tail1--;
				tail2--;
			}
			*(tail1 + 1) = 0;
			*(tail2 + 1) = 0;//截断
			if (strchr(head1,'a') || strchr(head1,'b') ||strchr(head1,'c'))
			{//1里包含了常元
				if (strchr(head2,'a') || strchr(head2,'b') ||strchr(head2,'c'))
				{
					cout<<"输入错误！"<<endl;//2里也包含了常元，判定输入错误
				}
				else
				{//2不包含常元
					string t2 = head2;
					string t1 = head1;
					for (j=i+1;j<n;j++)
					{//对P2后续原子进行替换
						if (P2[j].find(t2) != P2[j].npos)
							P2[j].replace(P2[j].find(t2),t2.length(),t1);
					}
					t.clear();
					t = t1 + "/" + t2;
					MGU[i] = t;
				}
			}
			else
			{//1里不包含常元
				string t2 = head2;
				string t1 = head1;
				for (j=i+1;j<n;j++)
				{//对P1后续原子进行替换
					if (P1[j].find(t1) != P1[j].npos)
						P1[j].replace(P1[j].find(t1),t1.length(),t2);
				}
				t.clear();
				t = t2 + "/" + t1;
				MGU[i] = t;
			}
		}
		else
		{//某两个原子无差异或无最一般合一
			if (strcmp(head1,head2) == 0)
				continue;//本次两个原子无差异
			cout<<"无最一般合一"<<endl;
			cin.get();
			cin.get();
			return 1;
		}
	}

	for (i=0;i<n;i++)
		cout<<MGU[i]<<"  ";
	cout<<endl;
	cin.get();
	cin.get();
	return 0;
}
