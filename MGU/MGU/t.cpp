#include<iostream>
#include<string>
using namespace std;
int isillegal(char *st1,char *st2)
{//���st1��st2���Ƿ�����ͬ��Ԫ��1����(�Ƿ�)��0����(�Ϸ�)
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
	int n;//��/��Ԫ�ܸ���
	int i,j;
	char *head1 = NULL,*tail1 = NULL,*head2 = NULL,*tail2 = NULL;//�ĸ�ָ�룬���ڱȶ�ԭ��
	string P1[10] = {""},P2[10] = {""};//P1��P2��/��Ԫ�б�
	string t = "";//��ʱ�����滻���
	string MGU[10] = {""};//��һ���һ�б�
	cout<<"��Ԫ�޶���abc����Ԫ�޶���xyzuvw\n��/��Ԫ�ܸ���(������10):";
	cin>>n;
	cout<<"ν��P1�ı�/��Ԫ�б�( "<<n<<" ��):";
	for (i=0;i<n;i++)
	{
		cin>>P1[i];
	}
	cout<<"ν��P2�ı�/��Ԫ�б�( "<<n<<" ��):";
	for (i=0;i<n;i++)
	{
		cin>>P2[i];
	}
	for (i=0;i<n;i++)
	{
		head1 = const_cast<char*>(P1[i].c_str());
		tail1 = const_cast<char*>(P1[i].c_str()) + P1[i].length() - 1;//head1��tail1ָ��ν��1�ĵ�i��ԭ�ӵ�ͷβ
		head2 = const_cast<char*>(P2[i].c_str());
		tail2 = const_cast<char*>(P2[i].c_str()) + P2[i].length() - 1;//head2��tail2ָ��ν��2�ĵ�i��ԭ�ӵ�ͷβ
		if (!isillegal(head1,head2))
		{
			while (*head1 == *head2)
			{//����ͷ����ͬ����
				head1++;
				head2++;
			}
			while (*tail1 == *tail2)
			{//����β����ͬ����
				tail1--;
				tail2--;
			}
			*(tail1 + 1) = 0;
			*(tail2 + 1) = 0;//�ض�
			if (strchr(head1,'a') || strchr(head1,'b') ||strchr(head1,'c'))
			{//1������˳�Ԫ
				if (strchr(head2,'a') || strchr(head2,'b') ||strchr(head2,'c'))
				{
					cout<<"�������"<<endl;//2��Ҳ�����˳�Ԫ���ж��������
				}
				else
				{//2��������Ԫ
					string t2 = head2;
					string t1 = head1;
					for (j=i+1;j<n;j++)
					{//��P2����ԭ�ӽ����滻
						if (P2[j].find(t2) != P2[j].npos)
							P2[j].replace(P2[j].find(t2),t2.length(),t1);
					}
					t.clear();
					t = t1 + "/" + t2;
					MGU[i] = t;
				}
			}
			else
			{//1�ﲻ������Ԫ
				string t2 = head2;
				string t1 = head1;
				for (j=i+1;j<n;j++)
				{//��P1����ԭ�ӽ����滻
					if (P1[j].find(t1) != P1[j].npos)
						P1[j].replace(P1[j].find(t1),t1.length(),t2);
				}
				t.clear();
				t = t2 + "/" + t1;
				MGU[i] = t;
			}
		}
		else
		{//ĳ����ԭ���޲��������һ���һ
			if (strcmp(head1,head2) == 0)
				continue;//��������ԭ���޲���
			cout<<"����һ���һ"<<endl;
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
