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

#define LeftBound 1 // Left boundary
#define RightBound 60 // Right boundary

#define FighterWidth 5 // Fighters length

#define NoMissile 23 // �̻����� �߻�� �� �� �� �ִ� �ִ� y�������� ���� 
#define NoEnemy 9 // ��ȭ�鿡 �����ϴ� �ִ� �� ��ü ��

// function declaration
int GamePlay();
void DrawGameScreen();
void DrawStatus();
void PrintStage();
void Enemy_Fighter(int enemy_life[], int enemy_x[]);
void Enemy_Boss_Fighter(int enemy_boss_life);
void CountnDraw_Score(int realscore);
void EnemyFighter_Move(int enemy_x[], int enemy_y[], int enemy_life[]);
void Enemy_Boss_Fighter_Move(int enemy_boss_x, int enemy_boss_y, int enemy_boss_life);
int check_shot(int enemy_x[], int enemy_y[], int missiley, int missilex, int enemy_life[]);
int boss_check_shot(int enemy_boss_x, int enemy_boss_y, int missiley, int missilex, int enemy_boss_life);
void CheckStage();
int CheckDie(int enemy_x[], int enemy_y[], int enemy_life[]);
void clrscr_center();
//void WriteRanking(realscore);
int Bomb(int enemy_life[], int enemy_x[], int enemy_y[], int missilex[], int missiley[], int enemy_boss_life);

// global variable
int score = 0; // �ʱ� Score
int life = 3, bomb = 3; // �ʱ� life ��, �ʱ� bomb ��
int stage = 1; // �ʱ� stage
int kx = 0, ky = 0; // missile x,y ��ǥ�� �ֱ� ���� ����
int jx = 0, jy = 0; 
int speed = 7; // �ʱ� �� ��ü ���ǵ�. ���ǵ� ���� ���� ������.
int enemy_draw = 0; // �ʱ� �� ��ü ����
int enemy_boss_draw = 0; // �ʱ� ���� ��ü ����

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
	int enemy_boss_x = 25;
	int enemy_boss_y = 2;
	int enemy_boss_life = 50;
	int realscore = 0; // score * 10
	int height = 1; // for enemy ���� �ӵ� control

	clrscr();
	DrawGameScreen();
	gotoxy(20, 12);
	printf("* G  A  M  E  S  T  A  R  T *");
	Sleep(1000);
	clrscr();

	// do ���� �ȿ����� �ǽð����� ������ ��� �����ؾ��ϴ� �Լ����� ����.
	do {
		realscore = score * 10;
		DrawGameScreen();
		DrawStatus(life);
		PrintStage(stage);
		CountnDraw_Score(realscore);
		if (height % (3 * (speed * 2)) == 0) // Enemy_Fighter �ʱ�ȭ
			Enemy_Fighter(enemy_life, enemy_x); 
		if (height % (3 * (speed * 2)) == 0 && score >= 150 && score <= 200) // Enemy_Boss_Fighter �ʱ�ȭ
			Enemy_Boss_Fighter(enemy_boss_life);
		if (height % (speed * 2) == 0) //  EnemyFighter_Move �ʱ�ȭ
			EnemyFighter_Move(enemy_x, enemy_y, enemy_life);
		if (height % (speed * 2) == 0 && score >= 150 && score <= 200) // Enemy_Boss_Fighter_Move �ʱ�ȭ
			Enemy_Boss_Fighter_Move(enemy_boss_x, enemy_boss_y, enemy_boss_life);

		if ((height % 2000) == 0) {// Enemy ���� �ӵ��� 2000�� �ɶ�
			if (bomb == 3) //Bomb�� 3�� �Ͻ� �״��  
				bomb = 3;
			else //Bomb�� 3���� �ƴҶ� ��ź �߰�
				bomb++;
		} // if
		if (x != oldx) {
			// erase old bar
			gotoxy(oldx, y);
			for (i = 0; i<FighterWidth; i++)
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
			if (boss_check_shot(enemy_boss_x, enemy_boss_y, missiley[i], missilex[i], enemy_boss_life)) {
				score += 50;
				gotoxy(jx, jy + 1);
				putchar(' ');
				gotoxy(jx, jy);
				putchar(' ');
				// missilex, missiley �ʱ�ȭ
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
				Bomb(enemy_life, enemy_x, enemy_y, missilex, missiley, enemy_boss_life);
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
					score = 0; // Score ����
					life = 3, bomb = 3; //life ����
					stage = 1; //stage ����
					kx = 0, ky = 0;
					speed = 7;//speed�� ����
					enemy_draw = 0; //����ü ���� ����
					break;
			} // else if
		} // if
	} 
	while (c != 27);
		//WriteRanking(realscore);
	clrscr_center();
	gotoxy(1, 1);
	printf("Thank you for playing!!\n");
	system("PAUSE");
}

// DrawGameScreen function definition - ���� ��ũ���� �׸��� �Լ�
void DrawGameScreen()
{
	int i = 0; 

	
	//���ڿ� '��'�� ���� ����Ʋ �����
	for (i = 1; i <= GS_HEIGHT - 1; i++) {
		gotoxy(1, i); 
		putchar('|');

		gotoxy(GS_WIDTH, i); 
		putchar('|');
	} // for
}

// DrawStatus function definition - Status�� �׸��� �Լ�
void DrawStatus(int life)
{
	
	//Score ��� 
	gotoxy(GS_WIDTH + 3, 3);
	printf("SCORE");

	//Bomb ���
	gotoxy(GS_WIDTH + 3, 12);
	printf("Bomb:");
	gotoxy(GS_WIDTH + 2, 13);

	
	//Bomb�� 3�� ������ ����â�� ���
	if (bomb == 3) {
		printf("                  ");
		gotoxy(GS_WIDTH + 2, 13);
		printf("BOMB BOMB BOMB");
	}

	
	//Bomb�� 2�� ���� �� ����â�� ���
	else if (bomb == 2) {
		printf("                 ");
		gotoxy(GS_WIDTH + 2, 13);
		printf("BOMB BOMB");
	}

	
	//Bomb�� 1�� ���� �� ����â�� ���
	else if (bomb == 1) {
		printf("              ");
		gotoxy(GS_WIDTH + 2, 13);
		printf("BOMB");
	}

	//Bomb�� 0�� ������ ����â�� ���
	else if (bomb == 0)
		printf("                  ");

	//Life ���
	gotoxy(GS_WIDTH + 3, 17);
	printf("Life:");
	gotoxy(GS_WIDTH + 2, 18);

	//Life 3�� �϶� ���� ����� ǥ��
	if (life == 3)
		printf("<-A-> <-A->");

	//Life 2�� �϶� ���� ����� ǥ�� 						   
	else if (life == 2) {
		printf("           ");
		gotoxy(GS_WIDTH + 2, 18);
		printf("<-A->");
	}

	else if (life == 1)
		printf("           ");

	//���� ���۱��� ǥ��
	gotoxy(GS_WIDTH + 5, 22);
	printf("Made by");
	gotoxy(GS_WIDTH + 2, 23);
	printf("Sung-Hyun, Lee");
}

// PrintStage function definition - stage�� ��� �Լ�
void PrintStage(int stage)
{
	//���Ӹ���â�� STAGE �ܰ� ǥ��
	gotoxy(GS_WIDTH + 6, 19);
	printf("STAGE %d", stage);
}

// CountnDraw_Score function definition - score�� ��� �Լ�
void CountnDraw_Score(realscore)
{
	//���Ӹ���â�� Fighter�� Score ǥ��
	gotoxy(SCORE_X, SCORE_Y);
	printf("%8d", realscore);
}

// Enemy_Fighter function definition - �� fighter���� life, x, y��ǥ�� �ִ� �Լ�
void Enemy_Fighter(int enemy_life[], int enemy_x[])
{
	int i;

	for (i = 0; i<NoEnemy; i++) { // ���� - enemy�� life�� ������
		if (!enemy_life[i]) {
			enemy_life[i] = 1;
			enemy_x[i] = rand() % 50 + 2; // ���� ����� x ��ǥ ��ġ ����
			enemy_draw++; // �� ����� ����
			break;
		} // if
	} // for
}

// Enemy_Boss_Fighter function definition - ������ �ݿ��� boss fighter�� ���� �������ִ� �Լ� 
void Enemy_Boss_Fighter(int enemy_boss_life)
{
	if (enemy_boss_life != 0 && stage == 6) {
		enemy_boss_draw = 1;
	}
}

// EnemyFighter_Move function definition - �� fighter �̵��ϴ� �Լ�
void EnemyFighter_Move(int enemy_x[], int enemy_y[], int enemy_life[])
{
	int i;

	
	for (i = 0; i<NoEnemy; i++) {
		if (enemy_life[i]) { //����- enemy�� life�� ������
			gotoxy(enemy_x[i], (enemy_y[i])++); //enemy ��ġ�� �ʰ� �ֺ� ����ó�� 
			printf("     ");
			gotoxy(enemy_x[i], enemy_y[i]); //enemy ǥ��
			printf("<-O->");
		} // if
	} // for

}

// Enemy_Boss_Fighter_Move function definition - �� boss fighter �̵��ϴ� �Լ�
void Enemy_Boss_Fighter_Move(int enemy_boss_x, int enemy_boss_y, int enemy_boss_life)
{
	if (enemy_boss_life != 0 && score >= 150 && score <= 200) { // ���� - enemy boss�� life�� �ְ�, ���������� 1500������ 2000�� ������ ��
		gotoxy(enemy_boss_x, enemy_boss_y);
		printf("#^^^#");
	}
}

// check_shot function definition - ���� �̻��Ͽ� �¾����� 1�� �����ϴ� �Լ�
int check_shot(int enemy_x[], int enemy_y[], int missiley, int missilex, int enemy_life[])
{
	int j;
	for (j = 0; j<NoEnemy; j++)
		// ���� - enemy�� life�� 1�̰�, enemy�� y��ǥ�� missile�� y��ǥ�� ����, enemy�� x, x+1, x+2, x+3, x+4�� ��ǥ�� missile�� y��ǥ�� ���� ��
		if (missiley == enemy_y[j] && enemy_life[j] == 1 && (missilex == enemy_x[j] || missilex == enemy_x[j] + 1 || missilex == enemy_x[j] + 2 || missilex == enemy_x[j] + 3 || missilex == enemy_x[j] + 4)) {
			gotoxy(enemy_x[j], enemy_y[j]);
			printf("     ");
			enemy_life[j] = 0;
			enemy_x[j] = 0;
			enemy_y[j] = 0;
			enemy_draw--; // �� ����� ���� ����
			kx = missilex; // kx ��� ������ ���� missilex ���� �����Ѵ�.
			ky = missiley; // ky ��� ������ ���� missiley ���� �����Ѵ�.
			return 1;
		} // if
	return 0;
}

// boss_check_shot function definition - boss�� �̻��Ͽ� �¾����� 1�� �����ϴ� �Լ�
int boss_check_shot(int enemy_boss_x, int enemy_boss_y, int missiley, int missilex, int enemy_boss_life)
{
	// ���� boss enemy�� life�� 0�� �ƴϰ�, boss ��ü�� y��ǥ�� �̻����� y��ǥ�� ����, boss ��ü�� x, x+1, x+2, x+3, x+4�� ��ǥ�� �̻����� y��ǥ�� ���� ��
	if (missiley == enemy_boss_y && enemy_boss_life != 0 && (missilex == enemy_boss_x || missilex == enemy_boss_x + 1 || missilex == enemy_boss_x + 2 || missilex == enemy_boss_x + 3 || missilex == enemy_boss_x + 4)) {
		enemy_boss_life--; // boss ��ü life�� ���δ�.
		// ���� boss enemy�� life�� 1�̰�, boss ��ü�� y��ǥ�� �̻����� y��ǥ�� ����, boss ��ü�� x, x+1, x+2, x+3, x+4�� ��ǥ�� �̻����� y��ǥ�� ���� ��
		if (missiley == enemy_boss_y && enemy_boss_life == 1 && (missilex == enemy_boss_x || missilex == enemy_boss_x + 1 || missilex == enemy_boss_x + 2 || missilex == enemy_boss_x + 3 || missilex == enemy_boss_x + 4))
		{
			gotoxy(enemy_boss_x, enemy_boss_y);
			printf("     ");
			enemy_boss_life = 0;
			enemy_boss_x = 0;
			enemy_boss_y = 0;
			enemy_boss_draw--; // ���� ��ü ���� ����
			jx = missilex; // jx ��� ������ ���� missilex ���� �����Ѵ�.
			jy = missiley; // jy ��� ������ ���� missiley ���� �����Ѵ�.
			return 1;
		}
	}
	return 0;
}

/* �ܰ�(stage) �� �ӵ� ���� */
void CheckStage()
{
	//score�� ������ enemy�� ��. (���� ���� ȭ���� score(== realscore)�� �ƴ�)
	if (score>=10)
		stage = 2;
	if (score>=30)
		stage = 3;
	if (score>=60)
		stage = 4;
	if (score>=100)
		stage = 5;
	if (score>=150)
		stage = 6;

	//speed�� enemy�� �������� �ӵ��� ������ ��.
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
		if (enemy_y[i] >= 24) { //����- enemy�� fighter�κ��� ����ġ�� �Ǹ�
			gotoxy(enemy_x[i], enemy_y[i]);
			printf("     "); //enemy ���� ǥ�� 
			enemy_life[i] = 0; //enemy life �ʱ�ȭ
			enemy_x[i] = 0; //enemy ��ǥ �ʱ�ȭ 
			enemy_y[i] = 0;
			return 1; //1���� �� ����
		} // if
	return 0;
}

// clrscr_center function definition - ȭ���� ������� ������� �Լ�
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
		for (y = 1; y <= ScreenHeight; y++) {
			gotoxy(x, y);
			Sleep(1);
			printf(" ");
		}
		x += i; // x��ǥ�� �����·� ����.
		i += 2; // i ������ 2�� ����. ���� loop���� x��ǥ ������ ������ �������� ��.
	}
}

// Bomb function definition - ��ź ���� ���� ��ź ���� �� ����� �������� �Լ�
int Bomb(int enemy_life[], int enemy_x[], int enemy_y[], int missilex[], int missiley[] , int enemy_boss_life)
{
	int i, j;

	//��ź�� 0���϶� ȭ�鿡 ��� �� �Ͻ�����
	if (bomb == 0) {

		//Status Screen�� ��� 
		gotoxy(GS_WIDTH+2, 13);
		printf("NO BOMB AVAILABLE!");
		
		//Game Main�� ��� 
		gotoxy(GS_WIDTH/2-7, 13);
		printf("NO BOMB AVAILABLE!");
		Sleep(800);
		//Game Main�� �ʱ�ȭ
		gotoxy(GS_WIDTH/2-7, 13);
		printf("                  ");
	}  

	//��ź�� ���� ��� ���� ��� â�� �̻��� ��� �� �Ͻ����� 
	else {

		//���� ����â 
		for (i = 1; i<GS_HEIGHT; i++) {
			gotoxy(GS_WIDTH - 59, i);
			putchar('l');
		
			//���� ����â
			for(j=2; j<=GS_WIDTH-50; j++){

				gotoxy(j-1,0);
				putchar(' ');
				gotoxy(j,0);
				putchar(' ');
				gotoxy(j+1,0);
				putchar('B');
				gotoxy(j+2,0);
				putchar('O');
				gotoxy(j+3,0);
				putchar('M');
				gotoxy(j+4,0);
				putchar('B');
				
			
			}
			Sleep(10); 
		} // for
		Sleep(50); 
		score += enemy_draw; // ��ź �߻�� �����ϴ� enemy �� ��ŭ ���� ����  
		score += enemy_boss_draw * 50; // ��ź �߻�� �����ϴ� boss enemy ��*50 ��ŭ ���� ����

		// ��ź�߻� �� �����ϴ� enemy�� �ʱ�ȭ 
		for (i = 0; i<NoEnemy; i++) {
			enemy_life[i] = 0; // 
			enemy_x[i] = 0;
			enemy_y[i] = 0;
		} 

		enemy_boss_life = 0;
		if (enemy_boss_life == 0)
		{
			enemy_boss_draw = 0;
		}

		// ��ź �߻� �� �����ϴ� �̻��� �� �ʱ�ȭ 
		for (i = 0; i<NoMissile; i++) {
			missilex[i] = 0;
			missiley[i] = 0;
		} 

		// ��ź �߻� �� ����â �ʱ�ȭ
		for (i = 1; i<GS_HEIGHT; i++) {
			gotoxy(GS_WIDTH - 58, i);
			putchar(' ');
			for (j = 2; j<GS_WIDTH; j++) {
				gotoxy(j, i);
				putchar(' ');
			} // for
			Sleep(10);
		} // for

		// enemy �� �ʱ�ȭ
		enemy_draw = 0;

		//��ź ���� ���̱�
		bomb--;
	} // else
	return 0;
}
