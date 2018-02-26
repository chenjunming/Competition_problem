/* exp6_09_3.cpp 循环赛日程安排问题-采用分治法 */
#include<stdlib.h>
#include<stdio.h>
int **A;              //int *指针数组，
int *schedule;        //int数组，一维数组保存二维数组的数据
int N = 1;             //问题的规模。初始化时会设定
					   //isodd:判断x是否奇数，是则返回1，否则0
int isodd(int x)
{
	return x & 1;
}
//print:打印赛程
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
	printf("第1列是选手编号\n");
	for (i = 0; i<row; i++)
	{
		for (j = 0; j<col; j++)
		{
			printf("%4d", A[i][j]);
		}
		printf("\n");
	}
}
/*init：初始化，设置问题规模N值，分配内存，用schedule指向；
把A构造成一个二维数组
*/
void init()
{
	int i, n;
	char line[100] = { '\0' };
	printf("请输入选手人数：");
	fgets(line, sizeof(line), stdin);
	N = atoi(line);
	if (N <= 0) {
		exit(-1);
	}
	if (isodd(N))
		n = N + 1;
	else
		n = N;

	//schedule是行化的二维数组
	schedule = (int *)calloc(n*n, sizeof(int));
	A = (int **)calloc(n, sizeof(int *));
	if (!schedule || A == NULL) exit(-2);

	for (i = 0; i<n; i++) //把A等价为二维数组
	{
		A[i] = schedule + i*n;
		A[i][0] = i + 1;//初始化这个数组的第一列
	}
	return;

}
/*replaceVirtual:把第m号虚的选手去掉（换做0）*/
void replaceVirtual(int m)
{
	int i, j;
	for (i = 0; i<m - 1; i++)   //行：对应选手号1～m-1
	{
		for (j = 0; j <= m; j++) //列: 比行要多1
			A[i][j] = (A[i][j] == m) ? 0 : A[i][j];
	}
	return;
}
/*copyeven:m为偶数时用,由前1组的m位选手的安排，来构成第2组m位选手
的赛程安排，以及两组之间的比赛安排  */
void copyeven(int m)
{
	if (isodd(m)) return;
	int i, j;
	for (j = 0; j<m; j++)    //1. 求第2组的安排（+m）
	{
		for (i = 0; i<m; i++)
		{
			A[i + m][j] = A[i][j] + m;
		}
	}
	for (j = m; j<2 * m; j++)//两组间比赛的安排
	{
		for (i = 0; i<m; i++)    //2. 第1组和第2组
		{
			A[i][j] = A[i + m][j - m]; //把左下角拷贝到右上角
		}
		for (i = m; i<2 * m; i++) //3. 对应的，第2组和第1组
		{
			A[i][j] = A[i - m][j - m]; //把左上角拷贝到右下角
		}
	}
	return;
}
/*copyodd:m为奇数时用,由前1组的m位选手的安排，来构成第2组m位选手
的赛程安排，以及两组之间的比赛安排。这时和m为偶数时的
处理有区别。
*/
void copyodd(int m)
{
	int i, j;
	for (j = 0; j <= m; j++)    //1. 求第2组的安排(前m天)
	{
		for (i = 0; i<m; i++)//行
		{
			if (A[i][j] != 0)
			{
				A[i + m][j] = A[i][j] + m;
			}
			else  //特殊处理：两个队各有一名选手有空，安排他们比赛
			{
				A[i + m][j] = i + 1;
				A[i][j] = i + m + 1;
			}
		}
	}
	///////////安排两组选手之间的比赛(后m-1天)////////////////////////
	for (i = 0, j = m + 1; j<2 * m; j++)
	{
		A[i][j] = j + 1;      //2. 1号选手的后m-1天比赛
		A[(A[i][j] - 1)][j] = i + 1;  //3. 他的对手后m-1天的安排
	}
	//以下的取值要依赖于1号选手的安排，所以之前先安排1号的赛程
	for (i = 1; i<m; i++)        //第1组的其他选手的后m-1天的安排
	{
		for (j = m + 1; j<2 * m; j++)
		{//2. 观察得到的规则一：向下m+1~2*m循环递增
			A[i][j] = ((A[i - 1][j] + 1) % m == 0) ? A[i - 1][j] + 1 : m + (A[i - 1][j] + 1) % m;
			//3. 对应第2组的对手也要做相应的安排
			A[(A[i][j] - 1)][j] = i + 1;
		}
	}
	return;
}
/*makecopy:当前有m位（偶数）选手，分成两组，每组由m/2位选手构成
由第一组的m/2位选手的安排来构成第二组的比赛安排，第一
组与第二组的比赛安排。要区分m/2为奇数和偶数两种情况*/
void makecopy(int m)
{
	if (isodd(m / 2))       //m/2为奇数
		copyodd(m / 2);
	else                  //m/2为偶数
		copyeven(m / 2);
}
void tournament(int m)
{
	if (m == 1)
	{
		A[0][0] = 1;
		return;
	}
	else if (isodd(m))        //如果m为奇数，则m+1是偶数
	{
		tournament(m + 1);     //按照偶数个选手来求解
		replaceVirtual(m + 1); //然后把第m+1号虚选手置成0
		return;
	}
	else                     //m是偶数，
	{
		tournament(m / 2);     //则先安排第1组的m/2人比赛
		makecopy(m);         //然后根据算法，构造左下、右下、右上、右下的矩阵
	}
	return;
}
/*endprogram:回收分配的内存*/
void endprogram()
{
	free(schedule);
	free(A);
}
int main()
{
	init();       //初始化 
	tournament(N);//求解
	print();      //打印结果
	endprogram(); //回收内存
	return 0;
}
