#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define maxsize 1000
#define nmax 100000
typedef char** huffmancode;
typedef struct
{
	char key;
	int weight;
	int parent, lchild, rchild;
}htnode, * huffmanTree;

void inithuffmantree(huffmanTree& ht)
{//初始化哈夫曼树
	ht = (htnode*)malloc(sizeof(htnode) * maxsize);
	for (int i = 1; i < maxsize; i++)
	{
		ht[i].weight = 0;
		ht[i].parent = 0;
		ht[i].lchild = 0;
		ht[i].rchild = 0;
	}
}

int seekkey(huffmanTree ht, int n, char ch)
{//该函数用来查找字符是否在已存在,如果存在，返回对应的下标值
//如果不存在返回0
	int i;
	for (i = 1; i <= n; i++)
	{
		if (ch == ht[i].key)
		{
			return i;
		}
	}
	if (i > n)
	{
		return 0;
	}
}

void select(huffmanTree ht, int n, int& s1, int& s2)
{//在ht[k](1<=k<=i-1)中选择两个其双亲域为0,
//且权值最小的结点，并返回他们的在ht数组中的序号s1和s2

	int i;
	int max = 1;
	for (i = 1; i <= n; i++)
	{
		if (ht[i].parent == 0)
		{
			if (ht[i].weight > ht[max].weight)
			{
				max = i;
			}
		}
	}//找到最大的无父母结点
	s1 = s2 = max;
	for (i = 1; i <= n; i++)
	{
		if ((ht[i].weight < ht[s1].weight) && (ht[i].parent == 0))
		{
			s1 = i;
		}
	}//先让s1成为最小的结点
	for (i = 1; i <= n; i++)
	{
		if ((ht[i].weight < ht[s2].weight) && (ht[i].parent == 0) && (i != s1))
		{
			s2 = i;
		}
	}//s2是除了s1以外第二小的结点

}

void creathuffmantree(huffmanTree& ht)
{//从文本文件中提取各个字符及其使用频率，构建哈夫曼树
	FILE* fp;
	char ch;
	int n = 0;//记录数组大小
	inithuffmantree(ht);
	fp = fopen("article.txt", "r");
	if (fp == NULL)
	{
		printf("文件不存在");
		return;
	}
	ch = fgetc(fp);
	while (ch != EOF)
	{
		int t;
		t = seekkey(ht, n, ch);
		if (t == 0)
		{
			n = n + 1;
			ht[n].key = ch;
			ht[n].weight++;
		}//没找到相应的字符
		else
		{
			ht[t].weight++;
		}//找到了相应的字符
		ch = fgetc(fp);
	}
	fclose(fp);
	ht[0].weight = n;//将元素个数保存到下标为0的位置中
	int s1, s2;
	for (int i = n + 1; i <= (2 * n - 1); i++)
	{
		select(ht, (i - 1), s1, s2);//在ht[k](1<=k<=i-1)中选择两个其双亲域为0,
									//且权值最小的结点，并返回他们的在ht数组中的序号s1和s2
		ht[s1].parent = ht[s2].parent = i;//表明已经选择过s1和s2两个叶子结点
		ht[i].lchild = s2;//s2作为i的左孩子
		ht[i].rchild = s1;//s1作为i的右孩子
		ht[i].weight = ht[s1].weight + ht[s2].weight;//i的权值为左右孩子的权值之和
	}
}

void creatcode(huffmanTree ht, huffmancode& hc, int n)
{//根据已经构建好的哈夫曼树，进行哈夫曼编码
//从叶子到根逆向求每个字符的哈夫曼编码，存储在编码表hc中
	char* cd;
	int start, c, f;
	hc = (huffmancode)malloc(sizeof(char*) * (n + 1));//给哈夫曼编码表分配存储空间
	cd = (char*)malloc(sizeof(char) * n);//给临时存放编码的数组分配存储空间
	cd[n - 1] = '\0';//编码结束符
	for (int i = 1; i <= n; i++)
	{

		start = n - 1;
		c = i;
		f = ht[i].parent;
		while (f != 0)//从叶子结点开始向上回溯，直到根节点
		{
			--start;//回溯一次，strat向前指一个位置
			if (ht[f].lchild == c)
			{
				cd[start] = '0';//结点c是f的左孩子，则生成代码0
			}
			else if (ht[f].rchild == c)
			{
				cd[start] = '1';//结点c是f的右孩子，则生成代码1
			}
			c = f;
			f = ht[f].parent;//继续向上回溯
		}
		hc[i] = (char*)malloc(sizeof(char) * (n - start));//为第i个字符串分配存储空间
		strcpy(hc[i], &cd[start]);//将求得的编码从临时空间cd复制到hc当前行中
	}
	free(cd);//释放临时空间
}

void translatehuffmancode(huffmanTree ht, huffmancode hc, int n)
{//根据构造的哈夫曼树，把用户输入的一段文字转换成哈夫曼编码
	char* a;
	bool s=0;//s判断是否编码成功
	a = (char*)malloc(sizeof(char) * maxsize);
	printf("请输入需要编码的文字:");
	getchar();//将上一次输入时缓冲区的回车符吸收掉
	gets_s(a, maxsize - 1);
	printf("编码结果：");
	for (int i = 0; a[i] != '\0'; i++)
	{
		int t;
		t = seekkey(ht, n, a[i]);//查找a[i]在编码表中所对应的序号
		if (t != 0)
		{
			s = 1;
			printf("%s", hc[t]);
		}
		if (t == 0)
		{
			s = 0;
		}
	}
	if (s==1)
	{
		printf("\n");
	}
	if (s == 0)
	{
		printf("\r编码不成功，未查找到对应字符,请重新输入.错误代码\n");
	}
	free(a);//释放存储空间
}

void translatchar(huffmanTree ht, int n)
{//根据构造的哈夫曼树，把用户输入的哈夫曼编码转换成文字
	printf("请输入需要译码的哈夫曼编码:");
	char* a2;
	char* b2;
	a2 = (char*)malloc(sizeof(char) * nmax);
	b2 = (char*)malloc(sizeof(char) * maxsize);
	getchar();//将上一次输入时缓冲区的回车符吸收掉
	gets_s(a2, nmax - 1);//输入需要译码的哈夫曼编码
	int s = 2 * n - 1;//根节点的下标
	int k = 0;
	for (int i = 0; a2[i] != '\0'; i++)
	{
		if (a2[i] == '0')
		{
			s = ht[s].lchild;
		}
		if (a2[i] == '1')
		{
			s = ht[s].rchild;
		}
		if (ht[s].lchild == 0 && ht[s].rchild == 0)//已经找到了叶子节点
		{
			b2[k] = ht[s].key;//将翻译出来的字符存放到数组b中
			k++;
			s = 2 * n - 1;//初始化s为哈夫曼树的根节点
		}//继续翻译下一个字符，从哈夫曼树的根节点开始
	}
	b2[k] = '\0';
	printf("译码结果：%s\n", b2);
	free(a2);//释放存储空间
	free(b2);//释放存储空间
}

void menu()
{
	printf("------------------------哈夫曼编码器------------------------\n");
	printf("		1.输入字符，将其转换成哈夫曼编码			\n");
	printf("		2.输入哈夫曼编码，将其转换成字符			\n");
	printf("		3.退出										\n");
	printf("------------------------------------------------------------\n");
	printf("请输入你所选择的数字:");
}
void quit()
{
	printf("------------------------------------------------------------\n");
	printf("--------------------------退出成功--------------------------\n");
	printf("-------------------感谢您使用哈夫曼编码器-------------------\n");
	printf("------------------------------------------------------------\n");
	printf("------------------------------------------------------------\n");
}
int main(void)
{
	int n;//元素个数
	huffmanTree ht;//哈夫曼树
	huffmancode hc;//哈夫曼编码表
	creathuffmantree(ht);//建立哈夫曼树
	n = ht[0].weight;//将元素个数赋值给n
	creatcode(ht, hc, n);//进行哈夫曼编码
	while (1)
	{
		menu();//打印菜单界面
		int t;
		scanf("%d", &t);
		switch (t)
		{
		case 1:
			translatehuffmancode(ht, hc, n);//将用户输入的文字转换成哈夫曼编码
			break;
		case 2:
			translatchar(ht, n);//将用户输入的哈夫曼编码转换成文字
			break;
		case 3:
			quit();//打印退出界面
			return 0;
			break;
		default:
			printf("您输入的序号不正确，请重新输入\n");
			break;
		}
	}
}