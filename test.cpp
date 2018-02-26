/* exp6_09_3.cpp ѭ�����ճ̰�������-���÷��η� */
#include<stdlib.h>
#include<stdio.h>
int **A;              //int *ָ�����飬
int *schedule;        //int���飬һά���鱣���ά���������
int N = 1;             //����Ĺ�ģ����ʼ��ʱ���趨
					   //isodd:�ж�x�Ƿ����������򷵻�1������0
int isodd(int x)
{
	return x & 1;
}
//print:��ӡ����
void print()
{
	int i, j, row, col;
	if (isodd(N))
	{
		row = N;
		col = N + 1;
	}
	else
	{
		row = N;
		col = N;
	}
	printf("��1����ѡ�ֱ��\n");
	for (i = 0; i<row; i++)
	{
		for (j = 0; j<col; j++)
		{
			printf("%4d", A[i][j]);
		}
		printf("\n");
	}
}
/*init����ʼ�������������ģNֵ�������ڴ棬��scheduleָ��
��A�����һ����ά����
*/
void init()
{
	int i, n;
	char line[100] = { '\0' };
	printf("������ѡ��������");
	fgets(line, sizeof(line), stdin);
	N = atoi(line);
	if (N <= 0) {
		exit(-1);
	}
	if (isodd(N))
		n = N + 1;
	else
		n = N;

	//schedule���л��Ķ�ά����
	schedule = (int *)calloc(n*n, sizeof(int));
	A = (int **)calloc(n, sizeof(int *));
	if (!schedule || A == NULL) exit(-2);

	for (i = 0; i<n; i++) //��A�ȼ�Ϊ��ά����
	{
		A[i] = schedule + i*n;
		A[i][0] = i + 1;//��ʼ���������ĵ�һ��
	}
	return;

}
/*replaceVirtual:�ѵ�m�����ѡ��ȥ��������0��*/
void replaceVirtual(int m)
{
	int i, j;
	for (i = 0; i<m - 1; i++)   //�У���Ӧѡ�ֺ�1��m-1
	{
		for (j = 0; j <= m; j++) //��: ����Ҫ��1
			A[i][j] = (A[i][j] == m) ? 0 : A[i][j];
	}
	return;
}
/*copyeven:mΪż��ʱ��,��ǰ1���mλѡ�ֵİ��ţ������ɵ�2��mλѡ��
�����̰��ţ��Լ�����֮��ı�������  */
void copyeven(int m)
{
	if (isodd(m)) return;
	int i, j;
	for (j = 0; j<m; j++)    //1. ���2��İ��ţ�+m��
	{
		for (i = 0; i<m; i++)
		{
			A[i + m][j] = A[i][j] + m;
		}
	}
	for (j = m; j<2 * m; j++)//���������İ���
	{
		for (i = 0; i<m; i++)    //2. ��1��͵�2��
		{
			A[i][j] = A[i + m][j - m]; //�����½ǿ��������Ͻ�
		}
		for (i = m; i<2 * m; i++) //3. ��Ӧ�ģ���2��͵�1��
		{
			A[i][j] = A[i - m][j - m]; //�����Ͻǿ��������½�
		}
	}
	return;
}
/*copyodd:mΪ����ʱ��,��ǰ1���mλѡ�ֵİ��ţ������ɵ�2��mλѡ��
�����̰��ţ��Լ�����֮��ı������š���ʱ��mΪż��ʱ��
����������
*/
void copyodd(int m)
{
	int i, j;
	for (j = 0; j <= m; j++)    //1. ���2��İ���(ǰm��)
	{
		for (i = 0; i<m; i++)//��
		{
			if (A[i][j] != 0)
			{
				A[i + m][j] = A[i][j] + m;
			}
			else  //���⴦�������Ӹ���һ��ѡ���пգ��������Ǳ���
			{
				A[i + m][j] = i + 1;
				A[i][j] = i + m + 1;
			}
		}
	}
	///////////��������ѡ��֮��ı���(��m-1��)////////////////////////
	for (i = 0, j = m + 1; j<2 * m; j++)
	{
		A[i][j] = j + 1;      //2. 1��ѡ�ֵĺ�m-1�����
		A[(A[i][j] - 1)][j] = i + 1;  //3. ���Ķ��ֺ�m-1��İ���
	}
	//���µ�ȡֵҪ������1��ѡ�ֵİ��ţ�����֮ǰ�Ȱ���1�ŵ�����
	for (i = 1; i<m; i++)        //��1�������ѡ�ֵĺ�m-1��İ���
	{
		for (j = m + 1; j<2 * m; j++)
		{//2. �۲�õ��Ĺ���һ������m+1~2*mѭ������
			A[i][j] = ((A[i - 1][j] + 1) % m == 0) ? A[i - 1][j] + 1 : m + (A[i - 1][j] + 1) % m;
			//3. ��Ӧ��2��Ķ���ҲҪ����Ӧ�İ���
			A[(A[i][j] - 1)][j] = i + 1;
		}
	}
	return;
}
/*makecopy:��ǰ��mλ��ż����ѡ�֣��ֳ����飬ÿ����m/2λѡ�ֹ���
�ɵ�һ���m/2λѡ�ֵİ��������ɵڶ���ı������ţ���һ
����ڶ���ı������š�Ҫ����m/2Ϊ������ż���������*/
void makecopy(int m)
{
	if (isodd(m / 2))       //m/2Ϊ����
		copyodd(m / 2);
	else                  //m/2Ϊż��
		copyeven(m / 2);
}
void tournament(int m)
{
	if (m == 1)
	{
		A[0][0] = 1;
		return;
	}
	else if (isodd(m))        //���mΪ��������m+1��ż��
	{
		tournament(m + 1);     //����ż����ѡ�������
		replaceVirtual(m + 1); //Ȼ��ѵ�m+1����ѡ���ó�0
		return;
	}
	else                     //m��ż����
	{
		tournament(m / 2);     //���Ȱ��ŵ�1���m/2�˱���
		makecopy(m);         //Ȼ������㷨���������¡����¡����ϡ����µľ���
	}
	return;
}
/*endprogram:���շ�����ڴ�*/
void endprogram()
{
	free(schedule);
	free(A);
}
int main()
{
	init();       //��ʼ�� 
	tournament(N);//���
	print();      //��ӡ���
	endprogram(); //�����ڴ�
	return 0;
}
