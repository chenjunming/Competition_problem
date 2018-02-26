/*ʹ�÷��η������������*/
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include<conio.h>
int** schedule;// ��ά��������ָ�룬������̰��Ž��
int N = 1; // ��¼���ٸ��˱������������ģ
/*�����Ƿ�Ϊ����*/
int isodd(int i) {
	return i & 1;
}
/*�����ά����*/
void printArray() {
	int n=N;
	// �������������ô����0ռλ������������Ҫ�����һ��
	if (isodd(N))n++;
	printf("��һ��Ϊѡ�ֱ��\n");
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < n; j++) {
			printf("%d\t", schedule[i][j]);
		}
		printf("\n");
	}
}
/*
 * ��ʼ��schedule��N
 */
void init() {
	printf("�������������:");
	char str[100];
	int n;
	while (1) {
		scanf("%s", str);
		N =  atoi(str);
		// �ж�����ĺϷ���
		if (N <= 1) {
			printf("����Ĳ��������������������롣");
			continue;
		}
		if (isodd(N)) {
			printf("NΪ����ʱ���޷�����������ĿҪ��Ľ⣬���ǿ����������N������ɱ����Ľ�\n");
			n = N + 1;
		}
		else {
			n = N;
		}
		break;
	}
	
	// ��ʼ��schedule
	schedule = (int **)malloc(n * sizeof(int *));
	if (schedule == NULL) 
	{ 
		printf("�ڴ治��");
		exit(-1); 
	}
	for (int i = 0; i < n; i++) {
		schedule[i] = (int *)malloc(n * sizeof(int));
		if (schedule[i] == NULL) {
			printf("�ڴ治��");
			exit(-1);
		}
		// ��һ��Ϊѡ�ֱ��
		schedule[i][0] = i + 1;
	}

	for (int i = 0; i < n; i++) {
		for (int j = 1; j < n; j++) {
			schedule[i][j] = 0;
		}
	}

}
/*
	��m������ʱ��������
	m�ǵ�ǰ���̵�һ��
*/
void completeOdd(int m) {
	// ���½Ǿ������ϽǼ���m��������ѡ�ֱ�ź�0��
	for (int i = 0; i < m; i++) {
		for (int j = 1; j <= m; j++) {
			if (schedule[i][j] != 0) {
				schedule[i + m][j] = schedule[i][j] + m;
			}
			else {
				// ���е������˽��б���
				schedule[i][j] = i + 1 + m;
				schedule[i + m][j] = i + 1;
			}
		}
	}
	// ���ź�m-1��ı���
	// ���Ͻ�
	// ���ŵ�һλ���У�ѡ�ֺ����Ķ��ֵ�����
	for (int j = m + 1; j < 2 * m; j++) {
		// ���ŵ�һλ���У�ѡ�ֵ�����
		schedule[0][j] = j + 1;
		// ���Ķ��ֵ�����
		schedule[j][j] = 1;
	}
	for (int i = 1; i < m; i++) {
		for (int j = m+1; j < 2 * m; j++) {
			schedule[i][j] = (schedule[i - 1][j] + 1) % m == 0 ? schedule[i - 1][j] + 1 : m + (schedule[i - 1][j] + 1) % m;
			// ��Ӧ��ѡ��ҲҪ�������̣����½ǣ�
			schedule[schedule[i][j] -1][j] = i + 1;
		}
	}

}
/*��m��ż��ʱ��������*/
void completeEven(int m) {
	// ���½Ǿ������ϽǼ���m
	int i, j;
	for (i = 0; i < m; i++) {
		for (int j = 1; j <= m; j++) {
			schedule[i + m][j] = schedule[i][j] + m;
			
		}
	}
	for (j = m; j<2 * m; j++)//���������İ���
	{
		for (i = 0; i<m; i++)    //���Ͻ� = ���½�
		{
			schedule[i][j] = schedule[i + m][j - m]; //�����½ǿ��������Ͻ�
		}
		for (i = m; i<2 * m; i++) //���½� = ���Ͻ�
		{
			schedule[i][j] = schedule[i - m][j - m]; //�����Ͻǿ��������½�
		}
	}
}
/**
* ���ʣ�µ����½ǣ����Ͻǣ����½�
* @param n �ܹ��ж���ѡ��(������ѡ�֣�
*/
void complete(int n) {
	// �� n/2 ����������ż����������
	if (isodd(n/2)) {// ���n/2������
		completeOdd(n/2);
	}
	else {// ���n/2��ż��
		completeEven(n / 2);
	}
}
// ����ѡ�ֱ��t�滻Ϊ0
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
* ִ���㷨
* @param n �㷨Ҫִ�е�����
*/
void execute(int n) 
{
	if (n == 1) {
		return;
	}
	else if (isodd(n)) {//���������
		execute(n + 1);// ����n+1�����̾���
		replace(n + 1);// �滻n+1Ϊ0
		return;
	}
	else {// ���n��ż��
		execute(n/2);// �Ȱ���n/2�������̣��������������Ͻǣ�
		complete(n);  // �����㷨���ʣ�µ����½ǣ����Ͻǣ����½�

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
	init();// ��ʼ��
	execute(N);//ִ���㷨
	printArray();// ������
	freeSource();// �ͷ���Դ
	printf("�����������......");
	getch();
}
