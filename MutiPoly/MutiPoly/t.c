#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<conio.h>
typedef struct Atom//����ʽ��ĳһ���ĳһ��ĸԪ������ָ��
{
	char x;//����ʽ��ĳһ���е�ĳһδ֪��
	int b;//���δ֪���Ĵ�����ͷ�ڵ�Ϊ�ܴ���
	struct Atom *next;
}Atom,*pAtomList;
typedef struct PNode//����ʽ�е�һ��
{
	double a;//ϵ��
	//char *Atomstr;//��ȥָ�����ֵ��ַ���
	pAtomList Atoms;//δ֪��ָ������
	struct PNode *next;
}PNode,*PList;
PList InitPolyList()
{//��ʼ������ʽ�����ɹ���������ָ�룬ʧ�ܷ���NULL
	PList tNode = (PList)malloc(sizeof(PNode));
	if (!tNode)
		return NULL;
	tNode->a = 0;
	tNode->Atoms = NULL;
	tNode->next = NULL;
	return tNode;
}
pAtomList InitAtomList()
{//��ʼ������ʽԪ�������ɹ���������ָ�룬ʧ�ܷ���NULL
	pAtomList tNode = (pAtomList)malloc(sizeof(Atom));
	if (!tNode)
		return NULL;
	tNode->x = 0;
	tNode->b = 0;
	tNode->next = NULL;
	return tNode;
}
Atom* NewAtomNode(char x,int b)
{//�½�һ��δ֪�����е�δ֪���ڵ㣬�ɹ����ؽڵ㣬ʧ�ܷ��ؿ�
	Atom *tNode = (Atom*)malloc(sizeof(Atom));
	if (!tNode)
		return NULL;
	tNode->b = b;
	tNode->x = x;
	tNode->next = NULL;
	return tNode;
}
int InsertAtomList(pAtomList AtomList,char x,int b)
{//������ʽδ֪����һ���������������ͬ���Զ��ϲ�������1�ڴ����ʧ�ܣ�0��������
	Atom *ptr = NULL;//�����α�
	Atom *tAtomIns = NULL;//�������δ֪��ָ������
	ptr = AtomList->next;
	while (ptr)//�����Ƿ�����ͬ��
	{
		if (ptr->x == x)//��δ֪����ͬ�ֱ�Ӻϲ�
		{
			ptr->b += b;
			break;
		}
		ptr = ptr->next;
	}
	if (!ptr)//����ͬ��
	{
		tAtomIns = NewAtomNode(x,b);
		if (!tAtomIns)
			return 1;
		tAtomIns->next = AtomList->next;
		AtomList->next = tAtomIns;//ͷ����
	}
	AtomList->b += b;//����ͷ�ڵ��ܴ���
/*	if (PolyNode)
	{
		ptr = PolyNode->Atoms->next;
		while (ptr)
		{
			if (ptr->b)
			{
				str[0] = ptr->x;
				str[1] = 0;
				strcat(tstr,str);
				strcat(tstr,itoa(ptr->b,str,10));
			}
			ptr = ptr->next;
		}
		tstr = (char*)realloc(tstr,(strlen(tstr)+1)*sizeof(char));
		PolyNode->Atomstr = tstr;
	}*/
	return 0;
}
pAtomList CreateAtomList(char *Atomstr)
{//����ĳ���δ֪�������ַ�������δ֪�������ɹ���������ʧ�ܷ��ؿ�
	char ch;//��ʱ���ĳδ֪��
	int tnum;//��ʱ����ĳ����
	pAtomList tAtomList;//δ֪����
	tAtomList = InitAtomList();//����δ֪����
	if (!tAtomList)
		return NULL;//�����쳣���ؿ�
	while (*Atomstr)//ѭ��������β
	{
		ch = *Atomstr;//����δ֪��
		Atomstr++;
		if (isalpha(*Atomstr) || *Atomstr == 0)//��һλֱ������ĸ�����
			tnum = 1;//����Ϊ1
		else
			tnum = atof(Atomstr);//�������
		while (isdigit(*Atomstr))//�����֣�����ƶ�
			Atomstr++;
		if (InsertAtomList(tAtomList,ch,tnum))//���Բ���δ֪������
			return NULL;
	}
	return tAtomList;
}
int IsSameAtomList(pAtomList Src_1,pAtomList Src_2)
{//�ж���δ֪�������Ƿ���ͬ
	Atom *ptr_1 = NULL;
	if (Src_1->b != Src_2->b)//δ֪��������ͬ��ֱ�ӷ��ز�ͬ
		return 0;
	else
	{
		Src_2 = Src_2->next;
		while (Src_2)
		{
			ptr_1 = Src_1->next;
			while (ptr_1)//��ͷ��β�ж�
			{
				if (Src_2->x == ptr_1->x && Src_2->b != ptr_1->b)//δ֪����Ӧ��ָ����ͬ
					return 0;
				if (Src_2->x == ptr_1->x && Src_2->b == ptr_1->b)//�ҵ�δ֪����ȫ��ͬ����
					break;
				ptr_1 = ptr_1->next;
			}
			if (!ptr_1)//��ͷ��βδ�ҵ���ͬδ֪������
				return 0;
			Src_2 = Src_2->next;
		}
		return 1;
	}
}
int InsertPolyList(PList pList,double a,char *Atomstr)
{//������ʽ��һ���������������ͬ���Զ��ϲ���������ܴ��ݽ������У�����1�ڴ����ʧ�ܣ�0��������
	PNode *ptr = NULL,*ptr_fast = NULL;//�����α�
	PList tNode = NULL;//����������ڵ�
	pAtomList AtomIns = NULL;//�������δ֪��ָ������

	AtomIns = CreateAtomList(Atomstr);
	if (!AtomIns)
		return 1;//����δ֪��ָ������
	
	ptr = pList;
	ptr_fast = ptr->next;
	while (ptr->next)//�����Ƿ�����ͬ��
	{
		if (IsSameAtomList(ptr_fast->Atoms,AtomIns))//�������δ֪��������ȫ��ͬ��ֱ�Ӻϲ�
		{
			ptr_fast->a += a;
			free(AtomIns);//����������Ľڵ�
			return 0;
		}
		if (ptr_fast->Atoms->b < AtomIns->b)//�ܴ�����С
		{
			tNode = (PNode*)malloc(sizeof(PNode));
			if (!tNode)
				return 1;
			tNode->a = a;
			tNode->Atoms = AtomIns;//�����ڴ����
			tNode->next = ptr->next;
			ptr->next = tNode;//�������ʽ�ڵ�
			return 0;
		}
		ptr = ptr->next;
		ptr_fast = ptr->next;
	}
	//����β��
	tNode = (PNode*)malloc(sizeof(PNode));
	if (!tNode)
		return 1;
	tNode->a = a;
	tNode->Atoms = AtomIns;//�����ڴ����
	tNode->next = ptr->next;
	ptr->next = tNode;//�������ʽ�ڵ�

	return 0;
}
char* GetAtomstr(pAtomList Atoms)
{//��ȡδ֪��������ַ�����ʽ
	Atom *ptr = Atoms->next;
	char x_str[2] = {0},temp[100] = {0};
	char *tstr = (char*)malloc(2048*sizeof(char));
	memset(tstr,0,2048);
	while (ptr)
	{
		x_str[0] = ptr->x;
		strcat(tstr,x_str);
		strcat(tstr,itoa(ptr->b,temp,10));
		ptr = ptr->next;
	}
	tstr = (char*)realloc(tstr,(strlen(tstr)+1)*sizeof(char));
	return tstr;
}
PList CopyPolyList(PList Dest,PList Src)
{//���ƶ���ʽ����
	PNode *ptr = Src->next;//ָ��Դ����ʽ
	char *pStr = NULL;
	while (ptr)//����ʱʵ�ָ���
	{
		pStr = GetAtomstr(ptr->Atoms);
		InsertPolyList(Dest,ptr->a,pStr);
		free(pStr);
		ptr = ptr->next;
	}
	return Dest;
}
PList AddTwoPolyList(PList p1,PList p2)
{//������ʽ��ӣ����ؽ������
	char *pStr = NULL;
	PList ResultList = InitPolyList();//�������ʽ����
	p1 = p1->next;
	while (p1)//����ʱʵ�����
	{
		pStr = GetAtomstr(p1->Atoms);
		InsertPolyList(ResultList,p1->a,pStr);
		free(pStr);
		p1 = p1->next;
	}
	p2 = p2->next;
	while (p2)//����ʱʵ�����
	{
		pStr = GetAtomstr(p2->Atoms);
		InsertPolyList(ResultList,p2->a,pStr);
		free(pStr);
		p2 = p2->next;
	}
	return ResultList;
}
PList SubTwoPolyList(PList p1,PList p2)
{//������ʽ��������ؽ������
	char *pStr = NULL;
	PList ResultList = InitPolyList();//�������ʽ����
	p1 = p1->next;
	while (p1)//����ʱʵ�����
	{
		pStr = GetAtomstr(p1->Atoms);
		InsertPolyList(ResultList,p1->a,pStr);
		free(pStr);
		p1 = p1->next;
	}
	p2 = p2->next;
	while (p2)//����ʱʵ�����
	{
		pStr = GetAtomstr(p2->Atoms);
		InsertPolyList(ResultList,-(p2->a),pStr);
		free(pStr);
		p2 = p2->next;
	}
	return ResultList;
}
PList MulTwoPolyList(PList p1,PList p2)
{//������ʽ��ˣ����ؽ������
	PNode *ptr_Item_1 = NULL,*ptr_Item_2 = NULL;//��������ʽָ��
	Atom *ptr_Atom_1 = NULL,*ptr_Atom_2 = NULL;//����δ֪������ָ��
	PList pRet = InitPolyList();//�������
	PList tList = NULL;//��ʱ����

	ptr_Item_2 = p2->next;
	while (ptr_Item_2)//ִ��PolyItem���
	{
		if (!ptr_Item_2->a)//����ϵ��Ϊ0�Ķ���ʽ��
		{
			ptr_Item_2 = ptr_Item_2->next;
			continue;
		}
		tList = InitPolyList();
		tList = CopyPolyList(tList,p1);
		ptr_Item_1 = tList->next;
		while (ptr_Item_1)
		{
			ptr_Item_1->a *= ptr_Item_2->a;//ϵ�����
			ptr_Atom_2 = ptr_Item_2->Atoms->next;
			while (ptr_Atom_2)
			{
				ptr_Atom_1 = ptr_Item_1->Atoms;
				InsertAtomList(ptr_Atom_1,ptr_Atom_2->x,ptr_Atom_2->b);//ִ��һ��Atom���
				ptr_Atom_2 = ptr_Atom_2->next;
			}
			ptr_Item_1 = ptr_Item_1->next;
		}
		CopyPolyList(pRet,tList);
		AllFree(tList);
		ptr_Item_2 = ptr_Item_2->next;
	}
	return pRet;
}
PList DivTwoPolyList(PList p1,PList p2,PList *last)
{//������ʽ��������ؽ������
	PNode *ptr_Item_1 = NULL,*ptr_Item_2 = NULL;//��������ʽָ��
	Atom *ptr_Atom_1 = NULL,*ptr_Atom_2 = NULL;//����δ֪������ָ��
	PList pRet = InitPolyList();//�������
	PList pBegin = pRet,tptr = NULL,tList = NULL,tList2 = NULL;//��ʱ����
	int n = 0;//����ʽ����

	ptr_Item_2 = p2->next;
	while (ptr_Item_2)//��ʽĳ��ϵ��Ϊ0�������ϵ����Ϊ0�����û���򱨴�
	{
		if (ptr_Item_2->a == 0)
			ptr_Item_2 = ptr_Item_2->next;
		else
			break;
	}
	if (!ptr_Item_2)
	{
		printf("ERROR! - Div 0\n");
		return NULL;
	}
	tptr = p1->next;
	while (tptr)//���㱻��ʽ����
	{
		n++;
		tptr = tptr->next;
	}
	tList = InitPolyList();
	tList = CopyPolyList(tList,p1);//����ʽ�ĸ���
	while (n--)//����n�����
	{
		ptr_Item_1 = tList;
		while (ptr_Item_1->next && !ptr_Item_1->next->a)//��������ʽ��ϵ��Ϊ0����
			ptr_Item_1 = ptr_Item_1->next;
		if (!ptr_Item_1)//����ʽΪ0������0�����
		{
			InsertPolyList(pRet,0,"");
			return pRet;
		}
		ptr_Atom_1 = ptr_Item_1->next->Atoms;//��ת������ʽһ�ĵ�һ����ЧAtom
		ptr_Atom_2 = ptr_Item_2->Atoms->next;
		while (ptr_Atom_2)
		{
			InsertAtomList(ptr_Atom_1,ptr_Atom_2->x,-(ptr_Atom_2->b));//ִ��һ��Atom���
			ptr_Atom_2 = ptr_Atom_2->next;
		}
		ptr_Item_1->next->a /= ptr_Item_2->a;//ϵ�����
		tptr = ptr_Item_1->next->next;
		ptr_Item_1->next->next = NULL;
		CopyPolyList(pRet,ptr_Item_1);
		ptr_Item_1->next->next = tptr;

		tList2 = MulTwoPolyList(pRet,p2);
		AllFree(tList);
		tList = SubTwoPolyList(p1,tList2);//tList����Ϊ��ʽ
		pRet = pRet->next;//��ֻ����
		AllFree(tList2);
	}
	AllFree(tList);
	tList = SubTwoPolyList(p1,MulTwoPolyList(pBegin,p2));

	*last = tList;
	return pBegin;
}
PList GetPolyList(char *PolyList_str)
{//����������ַ�����ö���ʽ�����ɹ���������ʧ�ܷ��ؿ�
	double a;//��ʱ����ĳ��ϵ��
	char *AtomBegin = NULL;//ָ��һ������ʽδ֪��ָ�����ֵĿ�ʼ
	char tch;//����ָ���
	PList tList = InitPolyList();//��ʼ������ʽ����
	if (!tList)
		return NULL;
	while (*PolyList_str)
	{
		//ȡϵ������
		if (isalpha(*PolyList_str) || (isalpha(*(PolyList_str+1)) && *PolyList_str == '+'))
			a = 1;//ĳ�ͷΪ��ĸ��ͷֻ��+�ţ���ϵ��Ϊ1
		else if (isalpha(*(PolyList_str+1)) && *PolyList_str == '-')
			a = -1;//ĳ�ͷֻ��-�ţ�ϵ��Ϊ-1
		else if (*PolyList_str == '+' || *PolyList_str == '-' || isdigit(*PolyList_str))
			a = atof(PolyList_str);//��ͷΪ���֣���¼������Ϊϵ��
		else
			return NULL;//�ַ������Ϸ�
		while (isdigit(*PolyList_str) || *PolyList_str == '+' || *PolyList_str == '-')
			PolyList_str++;//��ǰ������ֱ����������

		//ȡδ֪������
		AtomBegin = PolyList_str;//ָ�����ʽ��δ֪�����ֿ�ͷ
		while (*PolyList_str && *PolyList_str != '+' && *PolyList_str!= '-')
		{
			if (!isalnum(*PolyList_str))
				return NULL;//�зǷ�����
			PolyList_str++;//�����
		}
		tch = *PolyList_str;
		*PolyList_str = 0;//��0�ָ�
		if (InsertPolyList(tList,a,AtomBegin))//���Բ������ʽ
			return NULL;//����ʧ�ܷ��ؿ�
		*PolyList_str = tch;//�ָ��ָ���
	}
	
	return tList;
}
PList GetResult(char ch,PList Poly_1,PList Poly_2,PList *last)
{//������������㷽ʽִ�����㲢���ؽ��������ʧ�ܷ��ؿգ��ɹ���������
	PList Poly_Result = NULL;//�������ʽ
	switch (ch)
	{
	case '+':
		Poly_Result = AddTwoPolyList(Poly_1,Poly_2);
		break;
	case '-':
		Poly_Result = SubTwoPolyList(Poly_1,Poly_2);
		break;
	case '*':
		Poly_Result = MulTwoPolyList(Poly_1,Poly_2);
		break;
	case '/':
		Poly_Result = DivTwoPolyList(Poly_1,Poly_2,last);
		break;
	default:
		printf("Error!\n");
		return NULL;
	}
	return Poly_Result;
}
int OutputResult(PList pResult)
{
	PNode *ptr = pResult->next;
	Atom *ptr_at = NULL;
	int outFlag = 0;
	while (ptr)
	{
		if (pResult->next != ptr && ptr->a > 0)
			printf("+");
		else if (ptr->a == 0)//ĳ��Ϊ0������
		{
			ptr = ptr->next;
			continue;
		}
		outFlag = 1;
		if (ptr->a - (int)(ptr->a) < 1e-10)
			printf("%d%s",(int)ptr->a,GetAtomstr(ptr->Atoms));//ϵ��Ϊ����
		else
			printf("%.2lf%s",ptr->a,GetAtomstr(ptr->Atoms));
		ptr = ptr->next;
	}
	if (!outFlag)
		printf("0");//�������Ϊ0
	return 0;
}
int AllFree(PList pList)
{//�ݻ���������
	PNode *ptr = NULL;
	Atom *ptr_atom1 = NULL,*ptr_atom2 = NULL;
	if (!pList)
		return 0;
	ptr = pList->next;
	while (ptr)//�ݻ�PNode
	{
		if (pList->Atoms)//�ݻ�Atoms
		{
			ptr_atom1 = pList->Atoms;
			ptr_atom2 = ptr_atom1->next;
			while (ptr_atom2)
			{
				free(ptr_atom1);
				ptr_atom1 = ptr_atom2;
				ptr_atom2 = ptr_atom1->next;
			}
			free(ptr_atom1);
		}
		free(pList);
		pList = ptr;
		ptr = ptr->next;
	}

	ptr_atom1 = pList->Atoms;//�ݻ�Atoms
	ptr_atom2 = ptr_atom1->next;
	while (ptr_atom2)
	{
		free(ptr_atom1);
		ptr_atom1 = ptr_atom2;
		ptr_atom2 = ptr_atom1->next;
	}
	free(ptr_atom1);
	return 0;
}

int main(void)
{
	PList Poly_1 = NULL,Poly_2 = NULL;//��������ʽ
	PList Poly_Result = NULL;//�������ʽ
	PList Last = NULL;//���ڱ����������ʽ
	char ch;//����ķ���
	char *inputPoly = NULL;//����Ķ���ʽ�ַ���������
	
	printf("Poly Format: x1y+2x2y5+3z\n\n");//��ʾ�����ʽ

	//�����һ������ʽ
	inputPoly = (char*)malloc(65535*sizeof(char));
	printf("input first poly:\n");
	gets(inputPoly);
	inputPoly = (char*)realloc(inputPoly,(strlen(inputPoly)+1)*sizeof(char));
	Poly_1 = GetPolyList(inputPoly);
	free(inputPoly);

	//����ڶ�������ʽ
	inputPoly = (char*)malloc(65535*sizeof(char));
	printf("input second poly:\n");
	gets(inputPoly);
	inputPoly = (char*)realloc(inputPoly,(strlen(inputPoly)+1)*sizeof(char));
	Poly_2 = GetPolyList(inputPoly);
	free(inputPoly);

	//�������㷽ʽ
	printf("Type + - * /:\n");
	ch = getch();
	putch(ch);//����
	printf("\nResult:\n");

	Poly_Result = GetResult(ch,Poly_1,Poly_2,&Last);//���Ի�ȡ���
	if (!Poly_Result)//����ʧ��
	{
		AllFree(Poly_1);
		AllFree(Poly_2);
		AllFree(Poly_Result);
		return 1;
	}
	OutputResult(Poly_Result);
	if (Last)//������
	{
		printf("����");
		OutputResult(Last);
	}
	printf("\n");
	AllFree(Poly_1);
	AllFree(Poly_2);
	AllFree(Poly_Result);
	system("pause");
	return 0;
}
