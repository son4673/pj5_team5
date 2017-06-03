#include <stdio.h>
#include <windows.h>
#include <conio.h>

#include "Console.h"

#define clrscr() system("cls")
#define ESC 27
#define D 68
#define d 100


struct data {

	char name[10];
	int realscore;

};	// 이름과 점수 저장

struct data rank[17];	// data구조체 17개로 구성된 배열

						/* 파일에 기록 저장 */
void FileSave(void)
{
	FILE *rankfile;
	rankfile = fopen("rank.txt", "wt");

	int i;

	/* 파일에 1등부터 16등 점수 기록 */
	for (i = 0; i<16; i++) {

		if (rank[i].realscore == 0)
			fprintf(rankfile, "0 ....\n");

		else
			fprintf(rankfile, "%d %s\n", rank[i].realscore, rank[i].name);
	}
	fclose(rankfile);
}

/* 기존 랭킹 기록과 새로운 게임 점수를 비교하여 정렬*/
void RankSort(void)
{
	int i, j;
	struct data temp;

	for (i = 0; i<16; i++)
	{
		for (j = 0; j<17; j++)
		{
			if (rank[j].realscore < rank[j + 1].realscore)
			{
				temp = rank[j];
				rank[j] = rank[j + 1];
				rank[j + 1] = temp;
			}
		}
	}
	FileSave();
}

/* 랭킹에 등록할 이름과 점수를 입력받고 저장 */
void WriteRank(int realscore)
{
	gotoxy(33, 20);
	printf("NAME : ");
	gotoxy(41, 20);
	fgets(rank[16].name, 10, stdin);

	rank[16].realscore = realscore;
	RankSort();
	FileSave();
}

/* 랭킹 파일 초기화 */
void RankAllDel(void)
{
	int i;
	for (i = 0; i<17; i++)
		rank[i].realscore = 0; // 모든 순위의 점수를 0으로 만듬

	FileSave();
}

/* 파일 로드 */
void RankLoad(void)
{
	int i;
	FILE *rankfile;

	rankfile = fopen("rank.txt", "rt");

	if (rankfile == NULL) {		// 파일이 없는 경우, 파일을 생성해줌
		rankfile = fopen("rank.txt", "a");
		fclose(rankfile);
		return;
	}

	for (i = 0; i < 16; i++) {
		fscanf(rankfile, "%d %s\n", &rank[i].realscore, &rank[i].name);
	}

	fclose(rankfile);

}

/* 랭킹 그리기 - 출력*/
void RankDetailDraw()
{

	for (int i = 0; i < 8; i++)
	{
		gotoxy(5, (2 + i) * 3);
		printf("%d", i + 1);
		gotoxy(48, (2 + i) * 3);
		printf("%d", i + 9);
		gotoxy(18, (2 + i) * 3);
		printf("%s", rank[i].name);
		gotoxy(62, (2 + i) * 3);
		printf("%s", rank[i + 8].name);
		gotoxy(35, (2 + i) * 3);
		printf("%d", rank[i].realscore);
		gotoxy(80, (2 + i) * 3);
		printf("%d", rank[i + 8].realscore);

	}

	for (int i = 0; i < 24; i++)
	{
		gotoxy(42, 5 + i);
		printf("|");
	}

}

/* 랭킹 그리기 */
void ViewRanking(void)
{
	clrscr();
	RankLoad();
	RankSort();

	RankDetailDraw();

	// 키 입력
	while (1)
	{
		int key;
		key = getch();

		if (key == D || key == d)
		{
			RankAllDel();
			break;
		}
		if (key == ESC)
		{
			break;
		}
	}
	system("cls");
}
