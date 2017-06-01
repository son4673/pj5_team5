/*
game_play.c: game playing functions
*/

// header files
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#include "Console.h"

// define constant value
#define GS_WIDTH 60 // Game Screen Width
#define GS_HEIGHT 24 // Game Screen Height
#define ST_WIDTH 20 // Status Screen Width
#define ST_HEIGHT 24 // Status Screen Height
#define SCORE_X 70 // Score Location X
#define SCORE_Y 3 // Score Location Y

//���ΰ� ��ü �¿� ���
#define LeftBound 1
#define RightBound 60

#define FighterWidth 5 //���ΰ� ����
#define NoMissile 23 //�̻����� Y�� ����
#define NoEnemy 8//��ȭ�� ����ü ��

int coin = 0; //�̾ �ϱ� �Ǻ�

			  // function declaration
int GamePlay();
void DrawGameScreen();
void DrawStatus();
void PrintStage();
void Enemy_Fighter(int enemy_life[], int enemy_x[]);
void CountnDraw_Score(int realscore);
void EnemyFighter_Move(int enemy_x[], int enemy_y[], int enemy_life[]);
int check_shot(int enemy_x[], int enemy_y[], int missiley, int missilex, int enemy_life[]);
void CheckStage();
int CheckDie(int enemy_x[], int enemy_y[], int enemy_life[]);
void clrscr_center();
void WriteRanking(realscore);
int Bomb(int enemy_life[], int enemy_x[], int enemy_y[], int missilex[], int missiley[]);

// global variable
extern char first_name[3];
extern char second_name[3];
extern char third_name[3]; // ��ŷ�� 1, 2, 3���� �̸�

extern int first_score;
extern int second_score;
extern int third_score;

int score = 0; // �ʱ� Score
int life = 3, bomb = 3; // �ʱ� life
int stage = 1; // �ʱ� stage
int kx = 0, ky = 0;
int speed = 7;//�ʱ� speed�� �۾������� ������
int enemy_draw = 0; //����ü ����

					// function definitions
int GamePlay()
{

	int i = 0;
	char c = 0; // for keyboard hit
	int dx = 0; // for my fighter's movement

				// for missile x, y
	int missilex[NoMissile] = { 0 };
	int missiley[NoMissile] = { 0 };
	int counts_shot = 0; // for shot count

	int x = (LeftBound + RightBound - FighterWidth) / 2, y = 24; // x, y location initialization
	int oldx = x - 1;

	// for enemy status
	int enemy_x[NoEnemy] = { 0 };
	int enemy_y[NoEnemy] = { 2 };
	int enemy_life[NoEnemy] = { 0 };
	int realscore = 0; // score * 10
	int height = 1; // for enemy ���� �ӵ� control

	clrscr();
	DrawGameScreen();
	gotoxy(20, 12);
	printf("* G  A  M  E  S  T  A  R  T *");
	Sleep(1000);
	clrscr();

	// do ���� �ȿ����� �ǽð����� ������ ��� �����ؾ��ϴ� �Լ����� ����.
	do
	{
		realscore = score * 10;
		DrawGameScreen();
		DrawStatus(life);
		PrintStage(stage);
		CountnDraw_Score(realscore);
		if (height % (3 * (speed * 2)) == 0)
			Enemy_Fighter(enemy_life, enemy_x);
		if (height % (speed * 2) == 0)
			EnemyFighter_Move(enemy_x, enemy_y, enemy_life);
		if ((height % 2000) == 0) {
			if (bomb == 3)
				bomb = 3;
			else
				bomb++;
		} // if
		if (x != oldx) {
			// erase old bar
			gotoxy(oldx, y);
			for (i = 0; i<FighterWidth;i++)
				printf("  ");
			// draw new bar;
			gotoxy(x, y);
			printf("<-A->");
			oldx = x;
		} // if
		Sleep(50);
		for (i = 0; i<NoMissile; i++) {
			// check shot�� 1�� ��ȯ���� ���: enemy_x,y�� missilex,y�� ��ǥ�� ��ġ�� ��.
			if (check_shot(enemy_x, enemy_y, missiley[i], missilex[i], enemy_life)) {
				score++;
				gotoxy(kx, ky + 1); // kx, ky�� enemy_x,y�� ��ġ�ϴ� missilex, missiley�� ���̴�.
				putchar(' ');
				gotoxy(kx, ky);
				putchar(' ');
				// missilex, y �ʱ�ȭ
				missiley[i] = 0;
				missilex[i] = 0;
			} // if
			else {
				if (missiley[i]>2) { // missiley ��½�Ű�� if��
					gotoxy(missilex[i], missiley[i]);
					putchar(' ');
					missiley[i]--;
					gotoxy(missilex[i], missiley[i]);
					putchar('*');
				} // if
				else if (missiley[i] == 2) { // missiley�� y��ǥ 2�� �����ϸ� ����
					gotoxy(missilex[i], missiley[i]);
					putchar(' ');
					missilex[i] = 0;
					missiley[i] = 0;
				} // else if
			} // else
		} // for
		  // ����� key�� �޾Ƶ��̴� if��
		if (kbhit()) {
			c = getch();
			switch (c) {
			case 'j': // �������� �̵�
				if (dx >= 0)
					dx = -1;
				else if (dx>-3)
					dx--;
				break;
			case 'k': // ���������� �̵�
				if (dx <= 0)
					dx = 1;
				else if (dx<3)
					dx++;
				break;
			case 'a': // �̻��� �߻�
				missiley[counts_shot] = 23; // �̻��� �ʱ� y��
				missilex[counts_shot] = x + 2; // �̻��� �ʱ� x��
				counts_shot = (counts_shot + 1) % NoMissile; // �� ��° �̻����� �߻�Ǿ����� ����ϴ� ����
				break;
			case 's': // ��ź ����
				Bomb(enemy_life, enemy_x, enemy_y, missilex, missiley);
			default: // j, k, a�� �ƴ� Ű�� ���� ��� fighter ����
				dx = 0;
				break;
			} // switch
		} // if
		if (dx != 0) {
			x += dx; // fighter�� �����̰� �Ѵ�.
			if (x + FighterWidth >= RightBound) { // fighter�� ������ ��
				x = RightBound - FighterWidth;
				dx = 0;
			}
			if (x <= LeftBound) { // fighter�� ���� ��
				x = LeftBound + 1;
				dx = 0;
			}
		}
		height++;
		CheckStage(); // �ǽð����� Stage Check
		if (CheckDie(enemy_x, enemy_y, enemy_life)) { // enemy�� ��迡 �������� ��.
			if (life == 3)
				life--;
			else if (life == 2)
				life--;
			else if (life == 1)
			{
				life--;
				clrscr_center();
				gotoxy(GS_WIDTH / 2 - 8, GS_HEIGHT / 2);
				printf(" G A M E O V E R ");
				gotoxy(GS_WIDTH / 2 - 8, GS_HEIGHT / 2 + 1);
				system("PAUSE");
				coin = getch();
				score = 0; // Score ����
				life = 3, bomb = 3; //life ����
				stage = 1; //stage ����
				kx = 0, ky = 0;
				speed = 7;//speed�� ����
				enemy_draw = 0; //����ü ���� ����
				break;
			} // else if
		} // if
	} while (c != 27);
	{
		WriteRanking(realscore);
	}
	clrscr_center();
	gotoxy(1, 1);
	printf("Thank you for playing!!\n");
	system("PAUSE");

}

void DrawGameScreen()
{
	int i = 0; // for�� ����

	for (i = 1; i <= GS_HEIGHT - 1; i++) {
		gotoxy(1, i);
		putchar('|');

		gotoxy(GS_WIDTH, i);
		putchar('|');
	} // for
}

// DrawStatus - Status�� �׸��� �Լ�
void DrawStatus(int life)
{
	gotoxy(GS_WIDTH + 3, 3);
	printf("SCORE");

	// print ranking
	gotoxy(GS_WIDTH + 3, 7);
	printf("Ranking");
	gotoxy(GS_WIDTH + 4, 8);
	printf("1. %s %d", first_name, first_score);
	gotoxy(GS_WIDTH + 4, 9);
	printf("2. %s %d", second_name, second_score);
	gotoxy(GS_WIDTH + 4, 10);
	printf("3. %s %d", third_name, third_score);

	// print bomb
	gotoxy(GS_WIDTH + 3, 12);
	printf("Bomb:");
	gotoxy(GS_WIDTH + 2, 13);
	if (bomb == 3) {
		printf("                  ");
		gotoxy(GS_WIDTH + 2, 13);
		printf("<TNT> <TNT> <TNT>");
	} // if
	else if (bomb == 2) {
		printf("                 ");
		gotoxy(GS_WIDTH + 2, 13);
		printf("<TNT> <TNT>");
	} // else if

	else if (bomb == 1) {
		printf("              ");
		gotoxy(GS_WIDTH + 2, 13);
		printf("<TNT>");
	} // else if
	else if (bomb == 0)
		printf("                  ");

	// print life
	gotoxy(GS_WIDTH + 3, 17);
	printf("Life:");
	gotoxy(GS_WIDTH + 2, 18);

	if (life == 3)
		printf("<-A-> <-A->");
	else if (life == 2) {
		printf("           ");
		gotoxy(GS_WIDTH + 2, 18);
		printf("<-A->");
	} // else if
	else if (life == 1)
		printf("           ");

	// print my name
	gotoxy(GS_WIDTH + 5, 22);
	printf("Made by");
	gotoxy(GS_WIDTH + 2, 23);
	printf("Sung-Hyun, Lee");
}

void PrintStage(int stage) // Stage�� ��� �Լ�
{
	gotoxy(GS_WIDTH + 6, 19);
	printf("STAGE %d", stage);
}
void CountnDraw_Score(realscore) // score�� ��� �Լ�
{
	gotoxy(SCORE_X, SCORE_Y);
	printf("%8d", realscore);
}

// Enemy_Fighter function definition - �� fighter���� life, x, y��ǥ�� �ִ� �Լ�
void Enemy_Fighter(int enemy_life[], int enemy_x[])
{
	int i;

	for (i = 0; i<NoEnemy; i++) {
		if (!enemy_life[i])
		{
			enemy_life[i] = 1;
			enemy_x[i] = rand() % 50 + 2; // ���� ����� x ��ǥ ��ġ ����
			enemy_draw++;
			break;
		} // if
	} // for
}
// EnemyFighter_Move function definition - �� fighter �̵��ϴ� �Լ�
void EnemyFighter_Move(int enemy_x[], int enemy_y[], int enemy_life[])
{
	int i;

	for (i = 0; i<NoEnemy; i++)
	{
		if (enemy_life[i]) { // ���� - enemy�� life�� ������
			gotoxy(enemy_x[i], (enemy_y[i])++);
			printf("     ");
			gotoxy(enemy_x[i], enemy_y[i]);
			printf("<-O->");
		} // if
	} // for

}

// check_shot function definition - ���� �̻��Ͽ� �¾����� 1�� �����ϴ� �Լ�
int check_shot(int enemy_x[], int enemy_y[], int missiley, int missilex, int enemy_life[])
{
	int j;
	for (j = 0; j<NoEnemy; j++)
		// ���� - enemy�� life�� 1�̰�, enemy�� y��ǥ�� missile�� y��ǥ�� ����, enemy�� x, x+1, x+2, x+3, x+4d�� ��ǥ�� missile�� y��ǥ�� ���� ��
		if (missiley == enemy_y[j] && enemy_life[j] == 1 && (missilex == enemy_x[j] || missilex == enemy_x[j] + 1 || missilex == enemy_x[j] + 2 || missilex == enemy_x[j] + 3 || missilex == enemy_x[j] + 4)) {
			gotoxy(enemy_x[j], enemy_y[j]);
			printf("     ");
			enemy_life[j] = 0;
			enemy_x[j] = 0;
			enemy_y[j] = 0;
			enemy_draw--;
			kx = missilex; // kx ��� ������ ���� missilex ���� �����Ѵ�.
			ky = missiley; // ky ��� ������ ���� missiley ���� �����Ѵ�.
			return 1;
		} // if
	return 0;
}

void CheckStage()
{
	if (score>10)
		stage = 2;
	if (score>30)
		stage = 3;
	if (score>60)
		stage = 4;
	if (score>100)
		stage = 5;
	if (score>150)
		stage = 6;
	if (stage == 2)
		speed = 5;
	if (stage == 3)
		speed = 4;
	if (stage == 4)
		speed = 3;
	if (stage == 5)
		speed = 2;
	if (stage == 6)
		speed = 1;
}
// CheckDie function definition - ���� y��ǥ 24�� �����ϸ� ���� ���ְ� 1�� ����
int CheckDie(int enemy_x[], int enemy_y[], int enemy_life[])
{
	int i;
	for (i = 0; i<NoEnemy; i++)
		if (enemy_y[i] >= 24) {
			gotoxy(enemy_x[i], enemy_y[i]);
			printf("     ");
			enemy_life[i] = 0;
			enemy_x[i] = 0;
			enemy_y[i] = 0;
			return 1;
		} // if
	return 0;
}
// clrscr_center function definition - ȭ���� ������� ������� �Լ� (from HW 4_3)
void clrscr_center()
{
	int x = 0, y = 0; // ��ǥ�� ��Ÿ�� x, y �� �ʱ�ȭ
	int i = 1; // x��ǥ�� ��ȭ���� ��Ÿ���� ���� �ʱ�ȭ
	int ScreenWidth = 80, ScreenHeight = 24; // ��ũ���� ���� ���� ����
	for (x = ScreenWidth / 2; x <= ScreenWidth; x++) { // x���� ��ġ�� �����ϴ� for��.
		for (y = 1; y <= ScreenHeight; y++) { // y=1���� ����, y�� ScreenHeight���� Ŀ���� loop Ż��.
			gotoxy(x, y); // (x, y)�� �̵�
			Sleep(1); // Delay�� �ش�
			printf(" "); // ���ڿ��� ����
		}
		x -= i; // x��ǥ�� �������� �� ĭ �̵�.
		for (y = 1; y <= ScreenHeight; y++) { // y=1���� ����, y�� ScreenHeight���� Ŀ���� loop Ż��.
			gotoxy(x, y); // (x, y)�� �̵�
			Sleep(1); // Delay�� �ش�
			printf(" "); // ���ڿ��� ����
		}
		x += i; // x��ǥ�� �����·� ����.
		i += 2; // i ������ 2�� ����. ���� loop���� x��ǥ ������ ������ �������� ��.
	}
}
// WriteRanking function definition - ��ŷ�� ����� ���Ͽ� ���� �Լ�
void WriteRanking(realscore)
{
	FILE *rank = NULL; // rank ����
	int i = 0;

	if (realscore>third_score && realscore<second_score) { // 3��
		clrscr();
		gotoxy(1, 1);
		third_score = realscore;
		printf("3���� ����ϼ̽��ϴ�! ���ϵ帳�ϴ�.\n");
		printf("�̴ϼ��� �Է����ּ��� (���ĺ� �� �ڸ�): ");
		scanf("%s", third_name);
		i = 1;
	} // if
	else if (realscore>second_score && realscore<first_score) { // 2��
		clrscr();
		gotoxy(1, 1);
		// 2���� 3���� �ȴ�.
		for (i = 0; i<3; i++)
			third_name[3] = second_name[3];
		third_score = second_score;
		second_score = realscore;
		printf("2���� ����ϼ̽��ϴ�! ���ϵ帳�ϴ�.\n");
		printf("�̴ϼ��� �Է����ּ��� (���ĺ� �� �ڸ�): ");
		scanf("%s", second_name);
		i = 1;
	} // else if
	else if (realscore>first_score) { // 3��
		clrscr();
		gotoxy(1, 1);
		// 2���� 3���� �ǰ�, 1���� 2���� �ȴ�.
		for (i = 0; i<3; i++) {
			third_name[i] = second_name[i];
			second_name[i] = first_name[i];
		} // for
		third_score = second_score;
		second_score = first_score;
		first_score = realscore;
		printf("1���� ����ϼ̽��ϴ�! ���ϵ帳�ϴ�.\n");
		printf("�̴ϼ��� �Է����ּ��� (���ĺ� �� �ڸ�): ");
		scanf("%s", first_name);
		i = 1;
	} // else if

	if (i == 1) { // ��ŷ ���Ͽ� ���
		if ((rank = fopen("1945rk.dat", "w")) == NULL)
			printf("Error to open ranking file!");
		fprintf(rank, "%s %d ", first_name, first_score);
		fprintf(rank, "%s %d ", second_name, second_score);
		fprintf(rank, "%s %d ", third_name, third_score);
		printf("��ŷ ���Ͽ� ��ϵǾ����ϴ�. ���� ȭ������ �̵��մϴ�.\n");
	} // if
}

int Bomb(int enemy_life[], int enemy_x[], int enemy_y[], int missilex[], int missiley[])
{
	int i, j;
	if (bomb == 0) {
		gotoxy(GS_WIDTH + 2, 13);
		printf("NO BOMB AVAILABLE!");
		Sleep(100);
	} // if

	else {
		for (i = 1; i<GS_HEIGHT; i++) {
			gotoxy(GS_WIDTH - 58, i);
			putchar('*');
			for (j = 2; j<GS_WIDTH; j++) {
				gotoxy(j, i);
				putchar('*');
			} // for
			Sleep(10);
		} // for
		Sleep(100);
		score += enemy_draw;
		for (i = 0; i<NoEnemy; i++) {
			enemy_life[i] = 0;
			enemy_x[i] = 0;
			enemy_y[i] = 0;
		} // for
		for (i = 0; i<NoMissile; i++) {
			missilex[i] = 0;
			missiley[i] = 0;
		} // for
		for (i = 1; i<GS_HEIGHT; i++) {
			gotoxy(GS_WIDTH - 58, i);
			putchar(' ');
			for (j = 2; j<GS_WIDTH; j++) {
				gotoxy(j, i);
				putchar(' ');
			} // for
			Sleep(10);
		} // for
		enemy_draw = 0;
		bomb--;
	} // else
	return 0;
}