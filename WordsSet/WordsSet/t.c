//$x0((@x1(P(x1)))#(@x2(Q(x2))))
#include<stdio.h>
#include<windows.h>
int C[10] = {0};//��Ԫ�б�1����ѡ��0��δѡ
int X[10] = {0};//��Ԫ�б�1����ѡ��0��δѡ
int Sk[10] = {0};//˼�ƺ���������f��ͷ��1����ѡ��0��δѡ
//��			&
//��			|
//��			!
//��			#
//�س�����	@
//ȫ������	$
int Deletech(char *ptr)
{//ɾ��ָ��λ�õķ���
	while (*ptr)
	{
		*ptr = *(ptr + 1);
		ptr++;
	}
	return 0;
}
int InsertNeg(char *ptr)
{//��ptrָ��ǰ����ȡ��������ȡ����ָ��ָ��ȡ��
	char *p = ptr + strlen(ptr);
	while (p != ptr)
	{//����㼰���������������ƶ�һλ
		*p = *(p - 1);
		p--;
	}
	*ptr = '!';//����ȡ��
	return 0;
}
char* NoAtom(char *ptr,int d)
{//����ν�ʵı�Ԫ�볣Ԫ��d����ɨ�跽��0������(����ʱptrָ��ν��) 1������(����ʱptrָ��������֮���һ���ַ�)
	int L = 0,R = 0;//����������
	if (d == 0)
	{//��������
		L = 1;
		ptr += 2;//ָ������ν�ʺ�ĵ�һ������
		while (L != R)
		{//������ԭ����Ԫ
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
		return ptr - 1;//����ʱptrָ���ν�ʵ�������
	}
	else
	{//��������
		R = 1;
		ptr -= 2;//ָ������ν�ʺ�����һ������
		while (L != R)
		{//������ԭ����Ԫ
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
		return ptr;//����ʱptrָ���ν�ʵ�ν����
	}
}
char* NoAtom2(char *ptr,int d)
{//�����޶���Ԫ��d����ɨ�跽��0������(����ʱptrָ�����ʷ���) 1������(����ʱptrָ�����ʱ�Ԫ�����һ�����ֱ�ʾ�ĺ�һλ�ַ�)
	if (d == 0)
	{
		ptr += 2;//�������ʷ��ź�x
		while (isdigit(*ptr))
		{//�����������ֱ�ʶ
			ptr++;
		}
		ptr--;
		return ptr;//����ʱptrָ�����һ��Ԫ���ֱ�ʶ
	}
	else
	{
		ptr--;//�������ʷ��ű�Ԫ�����ֱ�ʶ����
		while (isdigit(*ptr))
		{//�����������ֱ�ʶ
			ptr--;
		}
		ptr--;
		return ptr;//����ʱptrָ�����ʷ���
	}
}
char* SkipItem(char *ptr,int d)
{//�����������ʱptrָ�����ţ�d���Ʒ��򣬷�������
	int L = 0,R = 0;//������������
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
{//����ν�ʹ�ʽ��ȡ��
	char *ptr = str;
	int Layer = 0;//��ǵ�ǰ����ת���Ĳ㼶
	int DoNeg[30] = {0};//����ָ���㼶�Ƿ����ȡ������
	while (*ptr)
	{//��֧����������!Ex0��!Vx0���
		if (*ptr == '!' && *(ptr + 1) == '(')
		{//����ȡ������Ե�ǰ�㼶����ȡ������
			Layer++;
			DoNeg[Layer] = 1 - DoNeg[Layer - 1];
			Deletech(ptr);//ɾ��ȡ������
		}
		else if (*ptr == '(' && (*(ptr + 1) < 'a' || *(ptr + 1) > 'z'))
		{//���񵽷�ԭ��ν�ʹ�ʽ�������ţ���㼶+1
			Layer++;
			DoNeg[Layer] = DoNeg[Layer - 1];
		}
		else if (*ptr == ')' && !isdigit(*(ptr-1)))
		{//���񵽲�����ԭ��ν�ʵ�������
			DoNeg[Layer] = 0;//��ǰ�㼶������ȡ����־����
			Layer--;//�㼶�ع�-1
		}

		if (DoNeg[Layer])
		{//�ò㼶Ҫ��ȡ����ִ�в㼶ȡ������
			if (*ptr >= 'A' && *ptr <= 'Z')
			{//����ν��
				InsertNeg(ptr);//��ν��ǰ����ȡ��
				ptr++;
				ptr = NoAtom(ptr,0);//������ν�ʶ�Ӧ�ĳ�Ԫ����Ԫ
			}
			else if (*ptr == '&')
			{//��ȡ����ȡ
				*ptr = '|';
			}
			else if (*ptr == '|')
			{//��ȡ�ĺ�ȡ
				*ptr = '&';
			}
			else if (*ptr == '@')
			{//�����سƷ���
				*ptr = '$';//�سƸ�ȫ��
				ptr = NoAtom2(ptr,0);
			}
			else if (*ptr == '$')
			{//����ȫ�Ʒ���
				*ptr = '@';//ȫ�Ƹ��س�
				ptr = NoAtom2(ptr,0);
			}
		}
		else if (DoNeg[Layer] == 0 && *ptr >= 'A' && *ptr <= 'Z')
		{//�ò㼶��Ҫ��ȡ����������ԭ��ν��
			ptr = NoAtom(ptr,0);//������ν��
		}

		ptr++;
	}
	
	return 0;
}
int ModifyCont(char *str)
{//ȥ���̺�,1��ν�ʹ�ʽ�쳣��0���ɹ�
	char *ptr = str;
	while (*ptr)
	{//���Ѱ���̺�ʽ
		if (*ptr == '#')
		{//�����̺�����
			*ptr = '|';//�̺����Ÿ�Ϊ��ȡ����
			ptr--;
			ptr = SkipItem(ptr,1);//��ǰ�ҵ�ǰһ��Ŀ�ͷ����
			if (*(ptr - 1) == '!')
				Deletech(ptr-1);//ɾ������ǰ��ȡ������
			else
				InsertNeg(ptr);//����Ŀ��ͷ������ǰ����ȡ������
			ptr = str;
			continue;
		}
		ptr++;
	}
	/*
	char *Begin = str;//��ͷָ��
	char *ptr = NULL;//��̬�α�
	int getAO = 0;//��־�Ƿ��ҵ���ȡ����ȡ
	int L = 0,R = 0;//������������������
	ptr = str;//�α�ָ����ָ��ν�ʹ�ʽ��ͷ
	while (*ptr)
	{//ɨ������ν�ʹ�ʽ
		if (*ptr == '#')//�ҵ��̺���ϵ
		{
			ptr--;//ָ����ǰ��תһλ
			if (*ptr != ')')
				return 1;//ν�ʹ�ʽ���﷨����
			R++;//��������+1
			ptr--;//ָ����ǰ��תһλ
			if (*ptr == ')')
			{//ǰһλ���������ţ�����ǰ����һ��С�͵�ν�ʼ�
				R++;//��������+1
				while (ptr != Begin)
				{//δ�ߵ���ʼλ�ã�Ϊ�˶�λ��ν�ʹ�ʽ��ͷ��
					if (*ptr == ')')
						R++;
					else if (*ptr == '(')
					{
						L++;
						if (L == R)
						{//��������ƽ�⣬ν�ʹ�ʽ��ȡ���
							if (*(ptr-1) == '!')
								Deletech(ptr-1);//��ν�ʹ�ʽͷ��ǰһλ��ȡ������ɾ�����ȡ����
							else
								InsertNeg(ptr);//��ptrָ����ַ�֮ǰ����ȡ������
							break;
						}
					}
					ptr--;
				}
			}
			else if (isdigit(*ptr))
			{//ǰһλ�����֣�����ǰ��ֻ��һ��ԭ��ν��
				while (ptr != Begin)
				{//δ�ߵ���ʼλ�ã�Ϊ�˶�λ��ԭ��ν��
					if (*ptr == '(')
					{//���ҵ�ԭ��ν�ʵ�������
						L++;
						break;
					}
					ptr--;
				}
				if (L != R)
					return 1;//ν�ʹ�ʽ�﷨����
				ptr--;//��λ��ԭ��ν�ʵ�ԭ��(��д��ĸ)
				while (ptr != Begin)
				{//δ�ߵ���ʼλ�ã�Ϊ�˶�λ��ȫ�ơ��س�֮ǰ
					if (*ptr == 'A' || *ptr == 'O')//��̬�α궨λ���Ǻ�ȡ����ȡ
					{
						getAO = 1;//�ҵ��˺�ȡ/��ȡ
						break;//����ѭ��
					}
					ptr--;
				}
				if (getAO == 1)
				{//��ǰλ��ԭ��ν�ʵ�ͷ
					if (*(ptr-1) == '!')
						Deletech(ptr-1);//��ԭ��ν��ͷ��ǰһλ��ȡ������ɾ�����ȡ����
					else
						InsertNeg(ptr);//��ptrָ����ַ�֮ǰ����ȡ������
				}
			}
			else
				return 1;//ν�ʹ�ʽ�﷨����
			ptr = Begin;//��ָ̬������ָ��ν�ʹ�ʽ��ͷ
		}
		ptr++;
	}
	*/
	return 0;
}
int DealSame1(char *str)
{//������ͬԼ����Ԫ
	int Same[30][10] = {0};//��Ǹò��Ƿ��ѳ���ĳ��Լ����Ԫ����Լ����Ԫ�ڶ�Ӧ�Ĳ��SameΪ1���������30�㣬���100��Լ����Ԫ
	char *ptr = str;
	int Layer = 0;//���ڱ�ǵ�ǰ�Ĳ㼶
	int Layer_bak;//���ڱ���ҵ��ظ�Լ����Ԫ�Ĳ㼶
	int i;
	while (*ptr)
	{
		if (*ptr == 'P' || *ptr == 'Q' || *ptr == 'R')
			NoAtom(ptr,0);//���ҵ���ν�ʣ�������ν��
		else if (*ptr == '(')
			Layer++;//���ҵ���������㼶��+1
		else if (*ptr == ')')
			Layer--;//���ҵ���������㼶��-1
		if (*ptr == 'x')
		{//����һ��Լ����Ԫ
			if (Same[Layer][atoi(ptr+1)] == 1)
			{//��Լ����Ԫ�ظ�
				for (i=0;i<10;i++)
				{
					if (Same[Layer][i] == 0)
					{//���ҵ�һ��δ��ʹ�ù���Լ����Ԫ
						Same[Layer][i] = 1;//��Ǹ�Լ����Ԫ�ѱ�ʹ��
						*(ptr + 1) = i + 0x30;//�޸�Լ����Ԫ
						Layer_bak = Layer;//���淢���ظ�Լ����Ԫ�Ĳ㼶
						while (*ptr)
						{//���뱻Լ���ķ�Χ
							if (*ptr == '(')
							{
								Layer++;
								ptr++;
								break;
							}
							ptr++;
						}
						while (Layer_bak < Layer)
						{//��Լ����Χδ����
							if (*ptr == '(')
								Layer++;
							else if (*ptr == ')')
								Layer--;
							else if (*ptr == 'x' && isdigit(*(ptr+1)))
								*(ptr + 1) = i + 0x30;//�޸ı�Լ���ı�Ԫ
							ptr++;
						}
						break;
					}
				}
			}
			else//Լ����Ԫ���ظ�
				Same[Layer][atoi(ptr+1)] = 1;//�ҵ�ĳ��Լ����Ԫ����Ӧ�㼶�ı�Ԫ���Ϊ���ظ�
		}
		ptr++;
	}
	return 0;
}
int ReplaceBySk(char *ptr,int PartID,int AllID)
{//ptrָ��ı�Ԫ��sk��������
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
{//���س�������˼�ƺ������
	char *ptr = str;
	int AllID[30] = {0};//ȫ�����ʵ�ID
	int PartID;//�س����ʵ�ID
	int Layer_bak = 0,Layer = 0;//ȫ�����ʲ㼶����ǰ�㼶
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
		{//����ȫ������
			Layer_bak = Layer;
			AllID[Layer_bak] = atoi(ptr + 2);
		}
		else if (*ptr == '@')
		{//�����س�����
			PartID = atoi(ptr + 2);
			Deletech(ptr);
			Deletech(ptr);
			Deletech(ptr);
			if (Layer > Layer_bak)
			{//��Ҫ˼��
				while (*ptr)
				{//���뱻Լ���ķ�Χ
					if (*ptr == '(')
					{
						Layer++;
						ptr++;
						break;
					}
					ptr++;
				}
				while (Layer_bak < Layer)
				{//��Լ����Χδ����
					if (*ptr == '(')
						Layer++;
					else if (*ptr == ')')
						Layer--;
					else if (*ptr == 'x' && isdigit(*(ptr+1)))
					{
						ReplaceBySk(ptr,PartID,AllID[Layer_bak]);//�޸ı�Լ���ı�Ԫ����sk�����滻
						break;
					}
					ptr++;
				}
			}
			else
			{//��Ϊ��ĸ����
				while (*ptr)
				{//���뱻Լ���ķ�Χ
					if (*ptr == '(')
					{
						Layer++;
						ptr++;
						break;
					}
					ptr++;
				}
				while (Layer_bak < Layer)
				{//��Լ����Χδ����
					if (*ptr == '(')
						Layer++;
					else if (*ptr == ')')
						Layer--;
					else if (*ptr == 'x' && isdigit(*(ptr+1)))
					{//��Ϊ��Ԫ
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
{//ɾ��ȫ������
	char *ptr = str;
	while (*ptr)
	{
		if (*ptr == '$')
		{//�ҵ�ȫ�����ʣ���ɾ��
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
{//�Ƿ���Ҫ���룬�ǣ�<ptr>����NULL
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
	char Input[999] = {0};//�����ν�ʹ�ʽ

	gets(Input);
	if (ModifyCont(Input))
		printf("�޸��̺�ʽ�쳣 - �﷨����\n");
	else
	{
		printf("ȥ���̺���\n%s\n",Input);//����̺�ʽ������
		if (DealNeg(Input))
			printf("����ȡ���쳣 - �﷨����\n");
		else
		{
			printf("ȡ������\n%s\n",Input);//���ȡ��������
			if (DealSame1(Input))
				printf("�������ʼ���ͬ��Ԫ�쳣 - �﷨����\n");
			else
			{
				printf("�������ʼ���ͬ��Ԫ��\n%s\n",Input);//������ʼ���ͬ��Ԫ������
				if (SkFunc(Input))
					printf("�����س������쳣 - �﷨����\n");
				else
				{
					printf("�����س����ʣ�\n%s\n",Input);//����س����ʴ�����
					DeleteAllSign(Input);
					printf("ɾ��ȫ�����ʣ�\n%s\n",Input);//���ȫ�����ʴ�����
					if (APart(Input) == NULL)
						printf("����ɾ���־���ظ���Ԫ������ȥ����ȡ���ţ�\n%s\n",Input);//������ʱֱ������־伯
				}
			}
		}
	}

	getchar();
	return 0;
}