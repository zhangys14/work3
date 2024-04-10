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
{//��ʼ����������
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
{//�ú������������ַ��Ƿ����Ѵ���,������ڣ����ض�Ӧ���±�ֵ
//��������ڷ���0
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
{//��ht[k](1<=k<=i-1)��ѡ��������˫����Ϊ0,
//��Ȩֵ��С�Ľ�㣬���������ǵ���ht�����е����s1��s2

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
	}//�ҵ������޸�ĸ���
	s1 = s2 = max;
	for (i = 1; i <= n; i++)
	{
		if ((ht[i].weight < ht[s1].weight) && (ht[i].parent == 0))
		{
			s1 = i;
		}
	}//����s1��Ϊ��С�Ľ��
	for (i = 1; i <= n; i++)
	{
		if ((ht[i].weight < ht[s2].weight) && (ht[i].parent == 0) && (i != s1))
		{
			s2 = i;
		}
	}//s2�ǳ���s1����ڶ�С�Ľ��

}

void creathuffmantree(huffmanTree& ht)
{//���ı��ļ�����ȡ�����ַ�����ʹ��Ƶ�ʣ�������������
	FILE* fp;
	char ch;
	int n = 0;//��¼�����С
	inithuffmantree(ht);
	fp = fopen("article.txt", "r");
	if (fp == NULL)
	{
		printf("�ļ�������");
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
		}//û�ҵ���Ӧ���ַ�
		else
		{
			ht[t].weight++;
		}//�ҵ�����Ӧ���ַ�
		ch = fgetc(fp);
	}
	fclose(fp);
	ht[0].weight = n;//��Ԫ�ظ������浽�±�Ϊ0��λ����
	int s1, s2;
	for (int i = n + 1; i <= (2 * n - 1); i++)
	{
		select(ht, (i - 1), s1, s2);//��ht[k](1<=k<=i-1)��ѡ��������˫����Ϊ0,
									//��Ȩֵ��С�Ľ�㣬���������ǵ���ht�����е����s1��s2
		ht[s1].parent = ht[s2].parent = i;//�����Ѿ�ѡ���s1��s2����Ҷ�ӽ��
		ht[i].lchild = s2;//s2��Ϊi������
		ht[i].rchild = s1;//s1��Ϊi���Һ���
		ht[i].weight = ht[s1].weight + ht[s2].weight;//i��ȨֵΪ���Һ��ӵ�Ȩֵ֮��
	}
}

void creatcode(huffmanTree ht, huffmancode& hc, int n)
{//�����Ѿ������õĹ������������й���������
//��Ҷ�ӵ���������ÿ���ַ��Ĺ��������룬�洢�ڱ����hc��
	char* cd;
	int start, c, f;
	hc = (huffmancode)malloc(sizeof(char*) * (n + 1));//����������������洢�ռ�
	cd = (char*)malloc(sizeof(char) * n);//����ʱ��ű�����������洢�ռ�
	cd[n - 1] = '\0';//���������
	for (int i = 1; i <= n; i++)
	{

		start = n - 1;
		c = i;
		f = ht[i].parent;
		while (f != 0)//��Ҷ�ӽ�㿪ʼ���ϻ��ݣ�ֱ�����ڵ�
		{
			--start;//����һ�Σ�strat��ǰָһ��λ��
			if (ht[f].lchild == c)
			{
				cd[start] = '0';//���c��f�����ӣ������ɴ���0
			}
			else if (ht[f].rchild == c)
			{
				cd[start] = '1';//���c��f���Һ��ӣ������ɴ���1
			}
			c = f;
			f = ht[f].parent;//�������ϻ���
		}
		hc[i] = (char*)malloc(sizeof(char) * (n - start));//Ϊ��i���ַ�������洢�ռ�
		strcpy(hc[i], &cd[start]);//����õı������ʱ�ռ�cd���Ƶ�hc��ǰ����
	}
	free(cd);//�ͷ���ʱ�ռ�
}

void translatehuffmancode(huffmanTree ht, huffmancode hc, int n)
{//���ݹ���Ĺ������������û������һ������ת���ɹ���������
	char* a;
	bool s=0;//s�ж��Ƿ����ɹ�
	a = (char*)malloc(sizeof(char) * maxsize);
	printf("��������Ҫ���������:");
	getchar();//����һ������ʱ�������Ļس������յ�
	gets_s(a, maxsize - 1);
	printf("��������");
	for (int i = 0; a[i] != '\0'; i++)
	{
		int t;
		t = seekkey(ht, n, a[i]);//����a[i]�ڱ����������Ӧ�����
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
		printf("\r���벻�ɹ���δ���ҵ���Ӧ�ַ�,����������.�������\n");
	}
	free(a);//�ͷŴ洢�ռ�
}

void translatchar(huffmanTree ht, int n)
{//���ݹ���Ĺ������������û�����Ĺ���������ת��������
	printf("��������Ҫ����Ĺ���������:");
	char* a2;
	char* b2;
	a2 = (char*)malloc(sizeof(char) * nmax);
	b2 = (char*)malloc(sizeof(char) * maxsize);
	getchar();//����һ������ʱ�������Ļس������յ�
	gets_s(a2, nmax - 1);//������Ҫ����Ĺ���������
	int s = 2 * n - 1;//���ڵ���±�
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
		if (ht[s].lchild == 0 && ht[s].rchild == 0)//�Ѿ��ҵ���Ҷ�ӽڵ�
		{
			b2[k] = ht[s].key;//������������ַ���ŵ�����b��
			k++;
			s = 2 * n - 1;//��ʼ��sΪ���������ĸ��ڵ�
		}//����������һ���ַ����ӹ��������ĸ��ڵ㿪ʼ
	}
	b2[k] = '\0';
	printf("��������%s\n", b2);
	free(a2);//�ͷŴ洢�ռ�
	free(b2);//�ͷŴ洢�ռ�
}

void menu()
{
	printf("------------------------������������------------------------\n");
	printf("		1.�����ַ�������ת���ɹ���������			\n");
	printf("		2.������������룬����ת�����ַ�			\n");
	printf("		3.�˳�										\n");
	printf("------------------------------------------------------------\n");
	printf("����������ѡ�������:");
}
void quit()
{
	printf("------------------------------------------------------------\n");
	printf("--------------------------�˳��ɹ�--------------------------\n");
	printf("-------------------��л��ʹ�ù�����������-------------------\n");
	printf("------------------------------------------------------------\n");
	printf("------------------------------------------------------------\n");
}
int main(void)
{
	int n;//Ԫ�ظ���
	huffmanTree ht;//��������
	huffmancode hc;//�����������
	creathuffmantree(ht);//������������
	n = ht[0].weight;//��Ԫ�ظ�����ֵ��n
	creatcode(ht, hc, n);//���й���������
	while (1)
	{
		menu();//��ӡ�˵�����
		int t;
		scanf("%d", &t);
		switch (t)
		{
		case 1:
			translatehuffmancode(ht, hc, n);//���û����������ת���ɹ���������
			break;
		case 2:
			translatchar(ht, n);//���û�����Ĺ���������ת��������
			break;
		case 3:
			quit();//��ӡ�˳�����
			return 0;
			break;
		default:
			printf("���������Ų���ȷ������������\n");
			break;
		}
	}
}