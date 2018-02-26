/*使用分治法解决赛程问题*/
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include<conio.h>
int** schedule;// 二维整型数组指针，存放赛程安排结果
int N = 1; // 记录多少个人比赛，即问题规模
/*检验是否为奇数*/
int isodd(int i) {
	return i & 1;
}
/*输出二维数组*/
void printArray() {
	int n=N;
	// 如果是奇数，那么会有0占位，所以我们需要多输出一列
	if (isodd(N))n++;
	printf("第一列为选手编号\n");
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < n; j++) {
			printf("%d\t", schedule[i][j]);
		}
		printf("\n");
	}
}
/*
 * 初始化schedule和N
 */
void init() {
	printf("请输入参赛人数:");
	char str[100];
	int n;
	while (1) {
		scanf("%s", str);
		N =  atoi(str);
		// 判断输入的合法性
		if (N <= 1) {
			printf("输入的参赛人数错误！请重新输入。");
			continue;
		}
		if (isodd(N)) {
			printf("N为奇数时，无法求解出符合题目要求的解，但是可以求出满足N天内完成比赛的解\n");
			n = N + 1;
		}
		else {
			n = N;
		}
		break;
	}
	
	// 初始化schedule
	schedule = (int **)malloc(n * sizeof(int *));
	if (schedule == NULL) 
	{ 
		printf("内存不足");
		exit(-1); 
	}
	for (int i = 0; i < n; i++) {
		schedule[i] = (int *)malloc(n * sizeof(int));
		if (schedule[i] == NULL) {
			printf("内存不足");
			exit(-1);
		}
		// 第一列为选手编号
		schedule[i][0] = i + 1;
	}

	for (int i = 0; i < n; i++) {
		for (int j = 1; j < n; j++) {
			schedule[i][j] = 0;
		}
	}

}
/*
	当m是奇数时，填充矩阵
	m是当前赛程的一半
*/
void completeOdd(int m) {
	// 左下角就是左上角加上m（不包含选手编号和0）
	for (int i = 0; i < m; i++) {
		for (int j = 1; j <= m; j++) {
			if (schedule[i][j] != 0) {
				schedule[i + m][j] = schedule[i][j] + m;
			}
			else {
				// 空闲的两个人进行比赛
				schedule[i][j] = i + 1 + m;
				schedule[i + m][j] = i + 1;
			}
		}
	}
	// 安排后m-1天的比赛
	// 右上角
	// 安排第一位（行）选手和他的对手的赛程
	for (int j = m + 1; j < 2 * m; j++) {
		// 安排第一位（行）选手的赛程
		schedule[0][j] = j + 1;
		// 他的对手的赛程
		schedule[j][j] = 1;
	}
	for (int i = 1; i < m; i++) {
		for (int j = m+1; j < 2 * m; j++) {
			schedule[i][j] = (schedule[i - 1][j] + 1) % m == 0 ? schedule[i - 1][j] + 1 : m + (schedule[i - 1][j] + 1) % m;
			// 对应的选手也要填充好赛程（右下角）
			schedule[schedule[i][j] -1][j] = i + 1;
		}
	}

}
/*当m是偶数时，填充矩阵*/
void completeEven(int m) {
	// 左下角就是左上角加上m
	int i, j;
	for (i = 0; i < m; i++) {
		for (int j = 1; j <= m; j++) {
			schedule[i + m][j] = schedule[i][j] + m;
			
		}
	}
	for (j = m; j<2 * m; j++)//两组间比赛的安排
	{
		for (i = 0; i<m; i++)    //右上角 = 左下角
		{
			schedule[i][j] = schedule[i + m][j - m]; //把左下角拷贝到右上角
		}
		for (i = m; i<2 * m; i++) //右下角 = 左上角
		{
			schedule[i][j] = schedule[i - m][j - m]; //把左上角拷贝到右下角
		}
	}
}
/**
* 填充剩下的左下角，右上角，右下角
* @param n 总共有多少选手(包含虚选手）
*/
void complete(int n) {
	// 分 n/2 是奇数还是偶数进行讨论
	if (isodd(n/2)) {// 如果n/2是奇数
		completeOdd(n/2);
	}
	else {// 如果n/2是偶数
		completeEven(n / 2);
	}
}
// 把虚选手编号t替换为0
void replace(int t) {
	for (int i = 0; i < t-1; i++) {
		for (int j = 1; j < t+1; j++) {
			if (schedule[i][j] == t) {
				schedule[i][j] = 0;
			}
		}
	}
}
/**
* 执行算法
* @param n 算法要执行的人数
*/
void execute(int n) 
{
	if (n == 1) {
		return;
	}
	else if (isodd(n)) {//如果是奇数
		execute(n + 1);// 计算n+1的赛程矩阵
		replace(n + 1);// 替换n+1为0
		return;
	}
	else {// 如果n是偶数
		execute(n/2);// 先安排n/2的人赛程（即结果矩阵的左上角）
		complete(n);  // 按照算法填充剩下的左下角，右上角，右下角

	}
}

void freeSource() {
	if (schedule == NULL)return;
	for (int i = 0; i < N; i++) {
		if (schedule[i] != NULL)
		{
			free(schedule[i]);
		}
	}
	free(schedule);
}
void main()
{
	init();// 初始化
	execute(N);//执行算法
	printArray();// 输出结果
	freeSource();// 释放资源
	printf("按任意键结束......");
	getch();
}
