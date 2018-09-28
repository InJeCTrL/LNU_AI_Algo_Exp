#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<conio.h>
typedef struct Atom//多项式的某一项的某一字母元素与其指数
{
	char x;//多项式的某一项中的某一未知数
	int b;//这个未知数的次数，头节点为总次幂
	struct Atom *next;
}Atom,*pAtomList;
typedef struct PNode//多项式中的一项
{
	double a;//系数
	//char *Atomstr;//除去指数部分的字符串
	pAtomList Atoms;//未知数指数集合
	struct PNode *next;
}PNode,*PList;
PList InitPolyList()
{//初始化多项式链表，成功返回链表指针，失败返回NULL
	PList tNode = (PList)malloc(sizeof(PNode));
	if (!tNode)
		return NULL;
	tNode->a = 0;
	tNode->Atoms = NULL;
	tNode->next = NULL;
	return tNode;
}
pAtomList InitAtomList()
{//初始化多项式元素链表，成功返回链表指针，失败返回NULL
	pAtomList tNode = (pAtomList)malloc(sizeof(Atom));
	if (!tNode)
		return NULL;
	tNode->x = 0;
	tNode->b = 0;
	tNode->next = NULL;
	return tNode;
}
Atom* NewAtomNode(char x,int b)
{//新建一个未知数链中的未知数节点，成功返回节点，失败返回空
	Atom *tNode = (Atom*)malloc(sizeof(Atom));
	if (!tNode)
		return NULL;
	tNode->b = b;
	tNode->x = x;
	tNode->next = NULL;
	return tNode;
}
int InsertAtomList(pAtomList AtomList,char x,int b)
{//将多项式未知数的一项插入链表，发现相同则自动合并，返回1内存分配失败，0正常插入
	Atom *ptr = NULL;//链表游标
	Atom *tAtomIns = NULL;//待插入的未知数指数集合
	ptr = AtomList->next;
	while (ptr)//查找是否有相同项
	{
		if (ptr->x == x)//有未知数相同项，直接合并
		{
			ptr->b += b;
			break;
		}
		ptr = ptr->next;
	}
	if (!ptr)//无相同项
	{
		tAtomIns = NewAtomNode(x,b);
		if (!tAtomIns)
			return 1;
		tAtomIns->next = AtomList->next;
		AtomList->next = tAtomIns;//头插入
	}
	AtomList->b += b;//更新头节点总次幂
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
{//根据某项的未知数部分字符串返回未知数链，成功返回链，失败返回空
	char ch;//临时存放某未知数
	int tnum;//临时保存某次幂
	pAtomList tAtomList;//未知数链
	tAtomList = InitAtomList();//生成未知数链
	if (!tAtomList)
		return NULL;//分配异常返回空
	while (*Atomstr)//循环遍历到尾
	{
		ch = *Atomstr;//保存未知数
		Atomstr++;
		if (isalpha(*Atomstr) || *Atomstr == 0)//下一位直接是字母或结束
			tnum = 1;//次幂为1
		else
			tnum = atof(Atomstr);//保存次幂
		while (isdigit(*Atomstr))//是数字，向后移动
			Atomstr++;
		if (InsertAtomList(tAtomList,ch,tnum))//尝试插入未知数链表
			return NULL;
	}
	return tAtomList;
}
int IsSameAtomList(pAtomList Src_1,pAtomList Src_2)
{//判断两未知数部分是否相同
	Atom *ptr_1 = NULL;
	if (Src_1->b != Src_2->b)//未知数数量不同，直接返回不同
		return 0;
	else
	{
		Src_2 = Src_2->next;
		while (Src_2)
		{
			ptr_1 = Src_1->next;
			while (ptr_1)//从头到尾判断
			{
				if (Src_2->x == ptr_1->x && Src_2->b != ptr_1->b)//未知数对应的指数不同
					return 0;
				if (Src_2->x == ptr_1->x && Src_2->b == ptr_1->b)//找到未知数完全相同的项
					break;
				ptr_1 = ptr_1->next;
			}
			if (!ptr_1)//从头到尾未找到相同未知数的项
				return 0;
			Src_2 = Src_2->next;
		}
		return 1;
	}
}
int InsertPolyList(PList pList,double a,char *Atomstr)
{//将多项式的一项插入链表，发现相同则自动合并，结果按总次幂降序排列，返回1内存分配失败，0正常插入
	PNode *ptr = NULL,*ptr_fast = NULL;//链表游标
	PList tNode = NULL;//新增的链表节点
	pAtomList AtomIns = NULL;//待插入的未知数指数集合

	AtomIns = CreateAtomList(Atomstr);
	if (!AtomIns)
		return 1;//生成未知数指数集合
	
	ptr = pList;
	ptr_fast = ptr->next;
	while (ptr->next)//查找是否有相同项
	{
		if (IsSameAtomList(ptr_fast->Atoms,AtomIns))//若两项的未知数部分完全相同，直接合并
		{
			ptr_fast->a += a;
			free(AtomIns);//放弃待插入的节点
			return 0;
		}
		if (ptr_fast->Atoms->b < AtomIns->b)//总次幂略小
		{
			tNode = (PNode*)malloc(sizeof(PNode));
			if (!tNode)
				return 1;
			tNode->a = a;
			tNode->Atoms = AtomIns;//各种内存分配
			tNode->next = ptr->next;
			ptr->next = tNode;//插入多项式节点
			return 0;
		}
		ptr = ptr->next;
		ptr_fast = ptr->next;
	}
	//插入尾部
	tNode = (PNode*)malloc(sizeof(PNode));
	if (!tNode)
		return 1;
	tNode->a = a;
	tNode->Atoms = AtomIns;//各种内存分配
	tNode->next = ptr->next;
	ptr->next = tNode;//插入多项式节点

	return 0;
}
char* GetAtomstr(pAtomList Atoms)
{//获取未知数链表的字符串形式
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
{//复制多项式链表
	PNode *ptr = Src->next;//指向源多项式
	char *pStr = NULL;
	while (ptr)//插入时实现复制
	{
		pStr = GetAtomstr(ptr->Atoms);
		InsertPolyList(Dest,ptr->a,pStr);
		free(pStr);
		ptr = ptr->next;
	}
	return Dest;
}
PList AddTwoPolyList(PList p1,PList p2)
{//两多项式相加，返回结果链表
	char *pStr = NULL;
	PList ResultList = InitPolyList();//结果多项式链表
	p1 = p1->next;
	while (p1)//插入时实现相加
	{
		pStr = GetAtomstr(p1->Atoms);
		InsertPolyList(ResultList,p1->a,pStr);
		free(pStr);
		p1 = p1->next;
	}
	p2 = p2->next;
	while (p2)//插入时实现相加
	{
		pStr = GetAtomstr(p2->Atoms);
		InsertPolyList(ResultList,p2->a,pStr);
		free(pStr);
		p2 = p2->next;
	}
	return ResultList;
}
PList SubTwoPolyList(PList p1,PList p2)
{//两多项式相减，返回结果链表
	char *pStr = NULL;
	PList ResultList = InitPolyList();//结果多项式链表
	p1 = p1->next;
	while (p1)//插入时实现相加
	{
		pStr = GetAtomstr(p1->Atoms);
		InsertPolyList(ResultList,p1->a,pStr);
		free(pStr);
		p1 = p1->next;
	}
	p2 = p2->next;
	while (p2)//插入时实现相减
	{
		pStr = GetAtomstr(p2->Atoms);
		InsertPolyList(ResultList,-(p2->a),pStr);
		free(pStr);
		p2 = p2->next;
	}
	return ResultList;
}
PList MulTwoPolyList(PList p1,PList p2)
{//两多项式相乘，返回结果链表
	PNode *ptr_Item_1 = NULL,*ptr_Item_2 = NULL;//两个多项式指针
	Atom *ptr_Atom_1 = NULL,*ptr_Atom_2 = NULL;//两个未知数部分指针
	PList pRet = InitPolyList();//结果链表
	PList tList = NULL;//临时链表

	ptr_Item_2 = p2->next;
	while (ptr_Item_2)//执行PolyItem相乘
	{
		if (!ptr_Item_2->a)//过滤系数为0的多项式项
		{
			ptr_Item_2 = ptr_Item_2->next;
			continue;
		}
		tList = InitPolyList();
		tList = CopyPolyList(tList,p1);
		ptr_Item_1 = tList->next;
		while (ptr_Item_1)
		{
			ptr_Item_1->a *= ptr_Item_2->a;//系数相乘
			ptr_Atom_2 = ptr_Item_2->Atoms->next;
			while (ptr_Atom_2)
			{
				ptr_Atom_1 = ptr_Item_1->Atoms;
				InsertAtomList(ptr_Atom_1,ptr_Atom_2->x,ptr_Atom_2->b);//执行一次Atom相乘
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
{//两多项式相除，返回结果链表
	PNode *ptr_Item_1 = NULL,*ptr_Item_2 = NULL;//两个多项式指针
	Atom *ptr_Atom_1 = NULL,*ptr_Atom_2 = NULL;//两个未知数部分指针
	PList pRet = InitPolyList();//结果链表
	PList pBegin = pRet,tptr = NULL,tList = NULL,tList2 = NULL;//临时链表
	int n = 0;//被除式项数

	ptr_Item_2 = p2->next;
	while (ptr_Item_2)//除式某项系数为0，向后找系数不为0的项，若没有则报错
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
	while (tptr)//计算被除式项数
	{
		n++;
		tptr = tptr->next;
	}
	tList = InitPolyList();
	tList = CopyPolyList(tList,p1);//被除式的复制
	while (n--)//进行n次相除
	{
		ptr_Item_1 = tList;
		while (ptr_Item_1->next && !ptr_Item_1->next->a)//跳过被除式中系数为0的项
			ptr_Item_1 = ptr_Item_1->next;
		if (!ptr_Item_1)//被除式为0，返回0结果链
		{
			InsertPolyList(pRet,0,"");
			return pRet;
		}
		ptr_Atom_1 = ptr_Item_1->next->Atoms;//跳转到多项式一的第一个有效Atom
		ptr_Atom_2 = ptr_Item_2->Atoms->next;
		while (ptr_Atom_2)
		{
			InsertAtomList(ptr_Atom_1,ptr_Atom_2->x,-(ptr_Atom_2->b));//执行一次Atom相除
			ptr_Atom_2 = ptr_Atom_2->next;
		}
		ptr_Item_1->next->a /= ptr_Item_2->a;//系数相除
		tptr = ptr_Item_1->next->next;
		ptr_Item_1->next->next = NULL;
		CopyPolyList(pRet,ptr_Item_1);
		ptr_Item_1->next->next = tptr;

		tList2 = MulTwoPolyList(pRet,p2);
		AllFree(tList);
		tList = SubTwoPolyList(p1,tList2);//tList最终为余式
		pRet = pRet->next;//暂只存商
		AllFree(tList2);
	}
	AllFree(tList);
	tList = SubTwoPolyList(p1,MulTwoPolyList(pBegin,p2));

	*last = tList;
	return pBegin;
}
PList GetPolyList(char *PolyList_str)
{//根据输入的字符串获得多项式链表，成功返回链表，失败返回空
	double a;//临时保存某项系数
	char *AtomBegin = NULL;//指向一个多项式未知数指数部分的开始
	char tch;//保存分隔符
	PList tList = InitPolyList();//初始化多项式链表
	if (!tList)
		return NULL;
	while (*PolyList_str)
	{
		//取系数部分
		if (isalpha(*PolyList_str) || (isalpha(*(PolyList_str+1)) && *PolyList_str == '+'))
			a = 1;//某项开头为字母或开头只有+号，则系数为1
		else if (isalpha(*(PolyList_str+1)) && *PolyList_str == '-')
			a = -1;//某项开头只有-号，系数为-1
		else if (*PolyList_str == '+' || *PolyList_str == '-' || isdigit(*PolyList_str))
			a = atof(PolyList_str);//开头为数字，则录入数字为系数
		else
			return NULL;//字符串不合法
		while (isdigit(*PolyList_str) || *PolyList_str == '+' || *PolyList_str == '-')
			PolyList_str++;//向前步进，直到不是数字

		//取未知数部分
		AtomBegin = PolyList_str;//指向多项式的未知数部分开头
		while (*PolyList_str && *PolyList_str != '+' && *PolyList_str!= '-')
		{
			if (!isalnum(*PolyList_str))
				return NULL;//有非法输入
			PolyList_str++;//向后检查
		}
		tch = *PolyList_str;
		*PolyList_str = 0;//以0分割
		if (InsertPolyList(tList,a,AtomBegin))//尝试插入多项式
			return NULL;//插入失败返回空
		*PolyList_str = tch;//恢复分隔符
	}
	
	return tList;
}
PList GetResult(char ch,PList Poly_1,PList Poly_2,PList *last)
{//根据输入的运算方式执行运算并返回结果，返回失败返回空，成功返回链表
	PList Poly_Result = NULL;//结果多项式
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
		else if (ptr->a == 0)//某项为0，跳过
		{
			ptr = ptr->next;
			continue;
		}
		outFlag = 1;
		if (ptr->a - (int)(ptr->a) < 1e-10)
			printf("%d%s",(int)ptr->a,GetAtomstr(ptr->Atoms));//系数为整数
		else
			printf("%.2lf%s",ptr->a,GetAtomstr(ptr->Atoms));
		ptr = ptr->next;
	}
	if (!outFlag)
		printf("0");//输入该项为0
	return 0;
}
int AllFree(PList pList)
{//摧毁整个链表
	PNode *ptr = NULL;
	Atom *ptr_atom1 = NULL,*ptr_atom2 = NULL;
	if (!pList)
		return 0;
	ptr = pList->next;
	while (ptr)//摧毁PNode
	{
		if (pList->Atoms)//摧毁Atoms
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

	ptr_atom1 = pList->Atoms;//摧毁Atoms
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
	PList Poly_1 = NULL,Poly_2 = NULL;//两个多项式
	PList Poly_Result = NULL;//结果多项式
	PList Last = NULL;//用于保存除法的余式
	char ch;//输入的符号
	char *inputPoly = NULL;//输入的多项式字符串缓冲区
	
	printf("Poly Format: x1y+2x2y5+3z\n\n");//提示输入格式

	//输入第一个多项式
	inputPoly = (char*)malloc(65535*sizeof(char));
	printf("input first poly:\n");
	gets(inputPoly);
	inputPoly = (char*)realloc(inputPoly,(strlen(inputPoly)+1)*sizeof(char));
	Poly_1 = GetPolyList(inputPoly);
	free(inputPoly);

	//输入第二个多项式
	inputPoly = (char*)malloc(65535*sizeof(char));
	printf("input second poly:\n");
	gets(inputPoly);
	inputPoly = (char*)realloc(inputPoly,(strlen(inputPoly)+1)*sizeof(char));
	Poly_2 = GetPolyList(inputPoly);
	free(inputPoly);

	//输入运算方式
	printf("Type + - * /:\n");
	ch = getch();
	putch(ch);//回显
	printf("\nResult:\n");

	Poly_Result = GetResult(ch,Poly_1,Poly_2,&Last);//尝试获取结果
	if (!Poly_Result)//计算失败
	{
		AllFree(Poly_1);
		AllFree(Poly_2);
		AllFree(Poly_Result);
		return 1;
	}
	OutputResult(Poly_Result);
	if (Last)//有余数
	{
		printf("……");
		OutputResult(Last);
	}
	printf("\n");
	AllFree(Poly_1);
	AllFree(Poly_2);
	AllFree(Poly_Result);
	system("pause");
	return 0;
}
