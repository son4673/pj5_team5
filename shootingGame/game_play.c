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

#define NoMissile 23 // 미사일이 발사된 후 갈 수 있는 최대 y축으로의 범위 
#define NoEnemy 9 // 한화면에 등장하는 최대 적 객체 수

#define LEFT 75
#define RIGHT 77

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
int Bomb(int enemy_life[], int enemy_x[], int enemy_y[], int missilex[], int missiley[]);

// global variable
int score = 0; // 초기 Score
int life = 3, bomb = 3; // 초기 life 수, 초기 bomb 수
int stage = 1; // 초기 stage
int kx = 0, ky = 0; // missile x,y 좌표를 넣기 위한 변수
int speed = 7; // 초기 적 객체 스피드. 스피드 작을 수록 빨라짐.
int enemy_draw = 0; // 초기 적 객체 갯수

					// function definitions
int GamePlay()
{
	srand((unsigned)time(NULL));

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
	int height = 1; // for enemy 생성 속도 control

	clrscr();
	DrawGameScreen();
	gotoxy(20, 12);
	printf("* G  A  M  E  S  T  A  R  T *");
	Sleep(1000);
	clrscr();

	// do 루프 안에서는 실시간으로 연산을 계속 수행해야하는 함수들이 들어간다.
	do {
		realscore = score * 10;
		DrawGameScreen();
		DrawStatus(life);
		PrintStage(stage);
		CountnDraw_Score(realscore);
		if (height % (3 * (speed * 2)) == 0) // Enemy_Fighter 초기화
			Enemy_Fighter(enemy_life, enemy_x); 
		if (height % (speed * 2) == 0) //  EnemyFighter_Move 초기화
			EnemyFighter_Move(enemy_x, enemy_y, enemy_life);

		if ((height % 2000) == 0) {// Enemy 생성 속도가 2000이 될때
			if (bomb == 3) //Bomb이 3개 일시 그대로  
				bomb = 3;
			else //Bomb이 3개가 아닐때 폭탄 추가
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
			// check shot가 1을 반환했을 경우: enemy_x,y와 missilex,y와 좌표가 일치할 때.
			if (check_shot(enemy_x, enemy_y, missiley[i], missilex[i], enemy_life)) {
				score++;
				gotoxy(kx, ky + 1); // kx, ky는 enemy_x,y와 일치하는 missilex, missiley의 값이다.
				putchar(' ');
				gotoxy(kx, ky);
				putchar(' ');
				// missilex, y 초기화
				missiley[i] = 0;
				missilex[i] = 0;
			} // if
			else {
				if (missiley[i]>2) { // missiley 상승시키는 if문
					gotoxy(missilex[i], missiley[i]);
					putchar(' ');
					missiley[i]--;
					gotoxy(missilex[i], missiley[i]);
					putchar('*');
				} // if
				else if (missiley[i] == 2) { // missiley가 y좌표 2에 도달하면 지움
					gotoxy(missilex[i], missiley[i]);
					putchar(' ');
					missilex[i] = 0;
					missiley[i] = 0;
				} // else if
			} // else
		} // for
		  // 사용자 key를 받아들이는 if문
		if (kbhit()) {
			c = getch();
			switch (c) {
			case LEFT: // 왼쪽으로 이동
				if (dx >= 0)
					dx = -1;
				else if (dx>-3)
					dx--;
				break;
			case RIGHT: // 오른쪽으로 이동
				if (dx <= 0)
					dx = 1;
				else if (dx<3)
					dx++;
				break;
			case 'a': // 미사일 발사
				missiley[counts_shot] = 23; // 미사일 초기 y값
				missilex[counts_shot] = x + 2; // 미사일 초기 x값
				counts_shot = (counts_shot + 1) % NoMissile; // 몇 번째 미사일이 발사되었는지 계산하는 변수
				break;
			case 's': // 폭탄 투하
				Bomb(enemy_life, enemy_x, enemy_y, missilex, missiley);
			default: // j, k, a가 아닌 키가 눌린 경우 fighter 정지
				dx = 0;
				break;
			} // switch
		} // if
		if (dx != 0) {
			x += dx; // fighter를 움직이게 한다.
			if (x + FighterWidth >= RightBound) { // fighter가 오른쪽 끝
				x = RightBound - FighterWidth;
				dx = 0;
			}
			if (x <= LeftBound) { // fighter가 왼쪽 끝
				x = LeftBound + 1;
				dx = 0;
			}
		}
		height++;
		CheckStage(); // 실시간으로 Stage Check
		if (CheckDie(enemy_x, enemy_y, enemy_life)) { // enemy가 경계에 도달했을 때.
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
					score = 0; // Score 리셋
					life = 3, bomb = 3; //life 리셋
					stage = 1; //stage 리셋
					kx = 0, ky = 0;
					speed = 7;//speed값 리셋
					enemy_draw = 0; //적객체 갯수 리셋
					break;
			} // else if
		} // if
	} while (c != 27);

	WriteRank(realscore);

	clrscr_center();
	gotoxy(1, 1);
	printf("Thank you for playing!!\n");
	system("PAUSE");
}

// DrawGameScreen function definition - 게임 스크린을 그리는 함수
void DrawGameScreen()
{
	int i = 0; 

	
	//문자열 'ㅣ'로 게임 메인틀 만들기
	for (i = 1; i <= GS_HEIGHT - 1; i++) {
		gotoxy(1, i); 
		putchar('|');

		gotoxy(GS_WIDTH, i); 
		putchar('|');
	} // for
}

// DrawStatus function definition - Status를 그리는 함수
void DrawStatus(int life)
{
	
	//Score 출력 
	gotoxy(GS_WIDTH + 3, 3);
	printf("SCORE");

	//Bomb 출력
	gotoxy(GS_WIDTH + 3, 12);
	printf("Bomb:");
	gotoxy(GS_WIDTH + 2, 13);

	
	//Bomb이 3개 있을때 메인창에 출력
	if (bomb == 3) {
		printf("                  ");
		gotoxy(GS_WIDTH + 2, 13);
		printf("BOMB BOMB BOMB");
	}

	
	//Bomb이 2개 있을 때 메인창에 출력
	else if (bomb == 2) {
		printf("                 ");
		gotoxy(GS_WIDTH + 2, 13);
		printf("BOMB BOMB");
	}

	
	//Bomb이 1개 있을 때 메인창에 출력
	else if (bomb == 1) {
		printf("              ");
		gotoxy(GS_WIDTH + 2, 13);
		printf("BOMB");
	}

	//Bomb이 0개 있을때 메인창에 출력
	else if (bomb == 0)
		printf("                  ");

	//Life 출력
	gotoxy(GS_WIDTH + 3, 17);
	printf("Life:");
	gotoxy(GS_WIDTH + 2, 18);

	//Life 3개 일때 남은 생명수 표시
	if (life == 3)
		printf("<-A-> <-A->");

	//Life 2개 일때 남은 생명수 표시 						   
	else if (life == 2) {
		printf("           ");
		gotoxy(GS_WIDTH + 2, 18);
		printf("<-A->");
	}

	else if (life == 1)
		printf("           ");

	//게임 저작권자 표시
	gotoxy(GS_WIDTH + 5, 22);
	printf("Made by");
	gotoxy(GS_WIDTH + 2, 23);
	printf("Sung-Hyun, Lee");
}

// PrintStage function definition - stage를 찍는 함수
void PrintStage(int stage)
{
	//게임메인창에 STAGE 단계 표시
	gotoxy(GS_WIDTH + 6, 19);
	printf("STAGE %d", stage);
}

// CountnDraw_Score function definition - score를 찍는 함수
void CountnDraw_Score(realscore)
{
	//게임메인창에 Fighter의 Score 표시
	gotoxy(SCORE_X, SCORE_Y);
	printf("%8d", realscore);
}

// Enemy_Fighter function definition - 적 fighter에게 life, x, y좌표를 주는 함수
void Enemy_Fighter(int enemy_life[], int enemy_x[])
{
	int i;

	for (i = 0; i<NoEnemy; i++) { // 조건 - enemy의 life가 있으면
		if (!enemy_life[i]) {
			enemy_life[i] = 1;
			enemy_x[i] = rand() % 50 + 2; // 적군 비행기 x 좌표 위치 랜덤
			enemy_draw++; // 적 비행기 생성
			break;
		} // if
	} // for
}

// EnemyFighter_Move function definition - 적 fighter 이동하는 함수
void EnemyFighter_Move(int enemy_x[], int enemy_y[], int enemy_life[])
{
	int i;

	
	for (i = 0; i<NoEnemy; i++) {
		if (enemy_life[i]) { //조건- enemy의 life가 있으면
			gotoxy(enemy_x[i], (enemy_y[i])++); //enemy 겹치지 않게 주변 공백처리 
			printf("     ");
			gotoxy(enemy_x[i], enemy_y[i]); //enemy 표시
			printf("<-O->");
		} // if
	} // for

}

// check_shot function definition - 적이 미사일에 맞았으면 1을 리턴하는 함수
int check_shot(int enemy_x[], int enemy_y[], int missiley, int missilex, int enemy_life[])
{
	int j;
	for (j = 0; j<NoEnemy; j++)
		// 조건 - enemy의 life가 1이고, enemy의 y좌표와 missile의 y좌표가 같고, enemy의 x, x+1, x+2, x+3, x+4d의 좌표와 missile의 y좌표가 같을 때
		if (missiley == enemy_y[j] && enemy_life[j] == 1 && (missilex == enemy_x[j] || missilex == enemy_x[j] + 1 || missilex == enemy_x[j] + 2 || missilex == enemy_x[j] + 3 || missilex == enemy_x[j] + 4)) {
			gotoxy(enemy_x[j], enemy_y[j]);
			printf("     ");
			enemy_life[j] = 0;
			enemy_x[j] = 0;
			enemy_y[j] = 0;
			enemy_draw--; // 적 비행기 사라짐
			kx = missilex; // kx 라는 변수에 현재 missilex 값을 대입한다.
			ky = missiley; // ky 라는 변수에 현재 missiley 값을 대입한다.
			return 1;
		} // if
	return 0;
}

/* 단계(stage) 및 속도 조정 */
void CheckStage()
{
	//score는 제거한 enemy의 수. (게임 실행 화면의 score(== realscore)가 아님)
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

	//speed는 enemy가 내려오는 속도에 영향을 줌.
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

// CheckDie function definition - 적이 y좌표 24에 도달하면 적을 없애고 1을 리턴
int CheckDie(int enemy_x[], int enemy_y[], int enemy_life[])
{
	int i;
	for (i = 0; i<NoEnemy; i++) 
		if (enemy_y[i] >= 24) { //조건- enemy가 fighter부분을 지나치게 되면
			gotoxy(enemy_x[i], enemy_y[i]);
			printf("     "); //enemy 공백 표시 
			enemy_life[i] = 0; //enemy life 초기화
			enemy_x[i] = 0; //enemy 좌표 초기화 
			enemy_y[i] = 0;
			return 1; //1리턴 후 종료
		} // if
	return 0;
}

// clrscr_center function definition - 화면이 가운데부터 사라지는 함수
void clrscr_center()
{
	int x = 0, y = 0; // 좌표를 나타낼 x, y 값 초기화
	int i = 1; // x좌표의 변화값을 나타내는 변수 초기화
	int ScreenWidth = 80, ScreenHeight = 24; // 스크린의 폭과 넓이 지정
	for (x = ScreenWidth / 2; x <= ScreenWidth; x++) { // x축의 위치를 지정하는 for문.
		for (y = 1; y <= ScreenHeight; y++) { // y=1에서 시작, y가 ScreenHeight보다 커지면 loop 탈출.
			gotoxy(x, y); // (x, y)로 이동
			Sleep(1); // Delay를 준다
			printf(" "); // 문자열을 지움
		}
		x -= i; // x좌표를 왼쪽으로 한 칸 이동.
		for (y = 1; y <= ScreenHeight; y++) {
			gotoxy(x, y);
			Sleep(1);
			printf(" ");
		}
		x += i; // x좌표를 원상태로 복귀.
		i += 2; // i 변수에 2를 더함. 다음 loop에서 x좌표 사이의 간격을 벌어지게 함.
	}
}

// Bomb function definition - 폭탄 갯수 계산과 폭탄 사용시 적 비행기 없어지는 함수
int Bomb(int enemy_life[], int enemy_x[], int enemy_y[], int missilex[], int missiley[])
{
	int i, j;

	//폭탄이 0개일때 화면에 출력 후 일시정지
	if (bomb == 0) {

		//Status Screen에 출력 
		gotoxy(GS_WIDTH+2, 13);
		printf("NO BOMB AVAILABLE!");
		
		//Game Main에 출력 
		gotoxy(GS_WIDTH/2-7, 13);
		printf("NO BOMB AVAILABLE!");
		Sleep(800);
		//Game Main에 초기화
		gotoxy(GS_WIDTH/2-7, 13);
		printf("                  ");
	}  

	//폭탄이 있을 경우 게임 모든 창에 미사일 출력 후 일시정지 
	else {

		//게임 세로창 
		for (i = 1; i<GS_HEIGHT; i++) {
			gotoxy(GS_WIDTH - 59, i);
			putchar('l');
		
			//게임 가로창
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
		score += enemy_draw; // 폭탄 발사시 존재하는 enemy 수 만큼 점수 증가  
		
		// 폭탄발사 후 존재하는 enemy값 초기화 
		for (i = 0; i<NoEnemy; i++) {
			enemy_life[i] = 0; // 
			enemy_x[i] = 0;
			enemy_y[i] = 0;
		} 

		// 폭탄 발사 후 존재하는 미사일 값 초기화 
		for (i = 0; i<NoMissile; i++) {
			missilex[i] = 0;
			missiley[i] = 0;
		} 

		// 폭탄 발사 후 메인창 초기화
		for (i = 1; i<GS_HEIGHT; i++) {
			gotoxy(GS_WIDTH - 58, i);
			putchar(' ');
			for (j = 2; j<GS_WIDTH; j++) {
				gotoxy(j, i);
				putchar(' ');
			} // for
			Sleep(10);
		} // for

		// enemy 수 초기화
		enemy_draw = 0;

		//폭탄 개수 줄이기
		bomb--;
	} // else
	return 0;
}
