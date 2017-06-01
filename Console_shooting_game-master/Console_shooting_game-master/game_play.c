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
#define LeftBound 1
#define RightBound 60
#define FighterWidth 5
#define NoMissile 23
#define NoEnemy 9

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
extern char third_name[3]; // 랭킹의 1, 2, 3등의 이름
extern int first_score;
extern int second_score;
extern int third_score;
int score=0; // Score
int life=3, bomb=3; // life
int stage=1; // stage
int kx=0, ky=0;
int speed=7;
int enemy_draw=0;

// function definitions
int GamePlay()
{

	int i=0; 
	char c=0; // for keyboard hit
	int dx=0; // for my fighter's movement

	// for missile x, y
	int missilex[NoMissile]={0};
	int missiley[NoMissile]={0};
	int counts_shot=0; // for shot count

	int x=(LeftBound+RightBound-FighterWidth)/2, y=24; // x, y location initialization
	int oldx=x-1;

	// for enemy status
	int enemy_x[NoEnemy]={0};
	int enemy_y[NoEnemy]={2};
	int enemy_life[NoEnemy]={0};
	int realscore=0; // score * 10
	int height=1; // for enemy 생성 속도 control

	clrscr();
	DrawGameScreen();
	gotoxy(20, 12);
	printf("* G  A  M  E  S  T  A  R  T *");
	Sleep(1000);
	clrscr();

	// do 루프 안에서는 실시간으로 연산을 계속 수행해야하는 함수들이 들어간다.
	do{
		realscore=score*10;
		DrawGameScreen();
		DrawStatus(life);
		PrintStage(stage);
		CountnDraw_Score(realscore);
		if(height % (3*(speed*2)) == 0)
			Enemy_Fighter(enemy_life, enemy_x);
		if(height % (speed*2) == 0)
			EnemyFighter_Move(enemy_x, enemy_y, enemy_life);
		if((height % 2000) == 0){
			if(bomb==3)
				bomb=3;
			else
				bomb++;
		} // if
		if(x!=oldx){
			// erase old bar
			gotoxy(oldx, y);
			for(i=0; i<FighterWidth;i++)
				printf("  ");
			// draw new bar;
			gotoxy(x, y);	
			printf("<-A->");
			oldx=x;
		} // if
		Sleep(50);
		for(i=0; i<NoMissile; i++){
			// check shot가 1을 반환했을 경우: enemy_x,y와 missilex,y와 좌표가 일치할 때.
			if(check_shot(enemy_x, enemy_y, missiley[i], missilex[i], enemy_life)){
				score++;
				gotoxy(kx, ky+1); // kx, ky는 enemy_x,y와 일치하는 missilex, missiley의 값이다.
				putchar(' ');
				gotoxy(kx, ky);
				putchar(' ');
				// missilex, y 초기화
				missiley[i]=0; 
				missilex[i]=0;
			} // if
			else{
				if(missiley[i]>2){ // missiley 상승시키는 if문
					gotoxy(missilex[i], missiley[i]);
					putchar(' ');
					missiley[i]--;
					gotoxy(missilex[i], missiley[i]);
					putchar('*');
				} // if
				else if(missiley[i]==2){ // missiley가 y좌표 2에 도달하면 지움
				gotoxy(missilex[i], missiley[i]);
				putchar(' ');
				missilex[i]=0;
				missiley[i]=0;
				} // else if
			} // else
		} // for
		// 사용자 key를 받아들이는 if문
		if(kbhit()){
			c=getch();
			switch(c){
			case 'j': // 왼쪽으로 이동
				if(dx>=0)
					dx=-1;
				else if(dx>-3)
					dx--;
				break;
			case 'k': // 오른쪽으로 이동
				if(dx<=0)
					dx=1;
				else if(dx<3) 
					dx++;
				break;
			case 'a': // 미사일 발사
				missiley[counts_shot] = 23; // 미사일 초기 y값
				missilex[counts_shot] = x+2; // 미사일 초기 x값
				counts_shot = (counts_shot + 1)%NoMissile; // 몇 번째 미사일이 발사되었는지 계산하는 변수
				break;
			case 's': // 폭탄 투하
				Bomb(enemy_life, enemy_x, enemy_y, missilex, missiley);
			default: // j, k, a가 아닌 키가 눌린 경우 fighter 정지
				dx=0;
				break;
			} // switch
		} // if
		if(dx!=0){
			x+=dx; // fighter를 움직이게 한다.
			if(x+FighterWidth>=RightBound){ // fighter가 오른쪽 끝
				x=RightBound-FighterWidth;
				dx=0;
			}
			if(x<=LeftBound){ // fighter가 왼쪽 끝
				x=LeftBound+1;
				dx=0;
			}
		}
		height++;
		CheckStage(); // 실시간으로 Stage Check
		if(CheckDie(enemy_x, enemy_y, enemy_life)){ // enemy가 경계에 도달했을 때.
			if(life==3)
				life--;
			else if(life==2)
				life--;
			else if(life==1){
				life--;
				clrscr_center();
				gotoxy(GS_WIDTH/2-8, GS_HEIGHT/2);
				printf(" G A M E O V E R ");
				gotoxy(GS_WIDTH/2-8, GS_HEIGHT/2+1);
				system("PAUSE");
				break;
			} // else if
		} // if
	}while(c!=27);
	WriteRanking(realscore);
	clrscr_center();
	gotoxy(1,1);
	printf("Thank you for playing!!\n");
	system("PAUSE");
	return 0;
}

void DrawGameScreen()
{
	int i=0; // for문 변수

	for(i=1; i<=GS_HEIGHT-1; i++){
		gotoxy(1, i);
		putchar('|');

		gotoxy(GS_WIDTH, i);
		putchar('|');
	} // for
}

// DrawStatus - Status를 그리는 함수
void DrawStatus(int life)
{
	gotoxy(GS_WIDTH+3, 3);
	printf("SCORE");

	// print ranking
	gotoxy(GS_WIDTH+3, 7);
	printf("Ranking");
	gotoxy(GS_WIDTH+4, 8);
	printf("1. %s %d", first_name, first_score);
	gotoxy(GS_WIDTH+4, 9);
	printf("2. %s %d", second_name, second_score);
	gotoxy(GS_WIDTH+4, 10);
	printf("3. %s %d", third_name, third_score);
	
	// print bomb
	gotoxy(GS_WIDTH+3, 12);
	printf("Bomb:");
	gotoxy(GS_WIDTH+2, 13);
	if(bomb==3){
		printf("                  ");
		gotoxy(GS_WIDTH+2, 13);
		printf("<TNT> <TNT> <TNT>");
	} // if
	else if(bomb==2){
		printf("                 ");
		gotoxy(GS_WIDTH+2, 13);
		printf("<TNT> <TNT>");
	} // else if

	else if(bomb==1){
		printf("              ");
		gotoxy(GS_WIDTH+2, 13);
		printf("<TNT>");
	} // else if
	else if(bomb==0)
		printf("                  ");

	// print life
	gotoxy(GS_WIDTH+3, 17);
	printf("Life:");
	gotoxy(GS_WIDTH+2, 18);

	if(life==3)
		printf("<-A-> <-A->");
	else if(life==2){
		printf("           ");
		gotoxy(GS_WIDTH+2, 18);
		printf("<-A->");
	} // else if
	else if(life==1)
		printf("           ");

	// print my name
	gotoxy(GS_WIDTH+5, 22);
	printf("Made by");
	gotoxy(GS_WIDTH+2, 23);
	printf("Sung-Hyun, Lee");
}

void PrintStage(int stage) // Stage를 찍는 함수
{
	gotoxy(GS_WIDTH+6, 19);
	printf("STAGE %d", stage);
}
void CountnDraw_Score(realscore) // score를 찍는 함수
{
	gotoxy(SCORE_X, SCORE_Y);
	printf("%8d", realscore);
}
// Enemy_Fighter function definition - 적 fighter에게 life, x, y좌표를 주는 함수
void Enemy_Fighter(int enemy_life[], int enemy_x[])
{
	int i;

	for(i=0; i<NoEnemy; i++){
		if(!enemy_life[i]){
			enemy_life[i]=1;
			enemy_x[i]=rand()%50+2; // 적군 비행기 x 좌표 위치 랜덤
			enemy_draw++;
			break;
		} // if
	} // for
}
// EnemyFighter_Move function definition - 적 fighter 이동하는 함수
void EnemyFighter_Move(int enemy_x[], int enemy_y[], int enemy_life[])
{
	int i;

	for(i=0; i<NoEnemy; i++){
		if(enemy_life[i]){ // 조건 - enemy의 life가 있으면
			gotoxy(enemy_x[i], (enemy_y[i])++);
			printf("     ");
			gotoxy(enemy_x[i], enemy_y[i]);
			printf("<-O->");
		} // if
	} // for
	
}

// check_shot function definition - 적이 미사일에 맞았으면 1을 리턴하는 함수
int check_shot(int enemy_x[], int enemy_y[], int missiley, int missilex, int enemy_life[])
{
	int j;
	for(j=0; j<NoEnemy; j++)
		// 조건 - enemy의 life가 1이고, enemy의 y좌표와 missile의 y좌표가 같고, enemy의 x, x+1, x+2, x+3, x+4d의 좌표와 missile의 y좌표가 같을 때
		if(missiley == enemy_y[j] && enemy_life[j]==1 && (missilex == enemy_x[j] || missilex == enemy_x[j]+1 || missilex == enemy_x[j]+2 || missilex == enemy_x[j]+3 || missilex == enemy_x[j]+4)){
			gotoxy(enemy_x[j], enemy_y[j]);
			printf("     ");
			enemy_life[j]=0;
			enemy_x[j]=0;
			enemy_y[j]=0;
			enemy_draw--;
			kx=missilex; // kx 라는 변수에 현재 missilex 값을 대입한다.
			ky=missiley; // ky 라는 변수에 현재 missiley 값을 대입한다.
			return 1;
		} // if
	return 0;
}

void CheckStage()
{
	if(score>10)
		stage=2;
	if(score>30)
		stage=3;
	if(score>60)
		stage=4;
	if(score>100)
		stage=5;
	if(score>150)
		stage=6;
	if(stage==2)
		speed=5;
	if(stage==3)
		speed=4;
	if(stage==4)
		speed=3;
	if(stage==5)
		speed=2;
	if(stage==6)
		speed=1;
}
// CheckDie function definition - 적이 y좌표 24에 도달하면 적을 없애고 1을 리턴
int CheckDie(int enemy_x[], int enemy_y[], int enemy_life[])
{
	int i;
	for(i=0; i<NoEnemy; i++)
		if(enemy_y[i]>=24){
			gotoxy(enemy_x[i], enemy_y[i]);
			printf("     ");
			enemy_life[i]=0;
			enemy_x[i]=0;
			enemy_y[i]=0;
			return 1;
		} // if
	return 0;
}
// clrscr_center function definition - 화면이 가운데부터 사라지는 함수 (from HW 4_3)
void clrscr_center() 
{
	int x=0, y=0; // 좌표를 나타낼 x, y 값 초기화
	int i=1; // x좌표의 변화값을 나타내는 변수 초기화
	int ScreenWidth=80, ScreenHeight=24; // 스크린의 폭과 넓이 지정
	for(x=ScreenWidth/2; x<=ScreenWidth; x++){ // x축의 위치를 지정하는 for문.
		for(y=1; y<=ScreenHeight; y++){ // y=1에서 시작, y가 ScreenHeight보다 커지면 loop 탈출.
			gotoxy(x, y); // (x, y)로 이동
			Sleep(1); // Delay를 준다
			printf(" "); // 문자열을 지움
		}
		x-=i; // x좌표를 왼쪽으로 한 칸 이동.
		for(y=1; y<=ScreenHeight; y++){ // y=1에서 시작, y가 ScreenHeight보다 커지면 loop 탈출.
			gotoxy(x, y); // (x, y)로 이동
			Sleep(1); // Delay를 준다
			printf(" "); // 문자열을 지움
		}
		x+=i; // x좌표를 원상태로 복귀.
		i+=2; // i 변수에 2를 더함. 다음 loop에서 x좌표 사이의 간격을 벌어지게 함.
	}
}
// WriteRanking function definition - 랭킹을 계산해 파일에 쓰는 함수
void WriteRanking(realscore)
{	
	FILE *rank=NULL; // rank 선언
	int i=0;

	if(realscore>third_score && realscore<second_score){ // 3등
		clrscr();
		gotoxy(1,1);
		third_score=realscore;
		printf("3등을 기록하셨습니다! 축하드립니다.\n");
		printf("이니셜을 입력해주세요 (알파벳 세 자리): ");
		scanf("%s", third_name);
		i=1;
	} // if
	else if(realscore>second_score && realscore<first_score){ // 2등
		clrscr();
		gotoxy(1,1);
		// 2등이 3등이 된다.
		for(i=0; i<3; i++)
			third_name[3]=second_name[3];
		third_score=second_score;
		second_score=realscore;
		printf("2등을 기록하셨습니다! 축하드립니다.\n");
		printf("이니셜을 입력해주세요 (알파벳 세 자리): ");
		scanf("%s", second_name);
		i=1;
	} // else if
	else if(realscore>first_score){ // 3등
		clrscr();
		gotoxy(1,1);
		// 2등이 3등이 되고, 1등이 2등이 된다.
		for(i=0; i<3; i++){
			third_name[i]=second_name[i];
			second_name[i]=first_name[i];
		} // for
		third_score=second_score;
		second_score=first_score;
		first_score=realscore;
		printf("1등을 기록하셨습니다! 축하드립니다.\n");
		printf("이니셜을 입력해주세요 (알파벳 세 자리): ");
		scanf("%s", first_name);
		i=1;
	} // else if

	if(i==1){ // 랭킹 파일에 기록
		if((rank=fopen("1945rk.dat", "w"))==NULL)
			printf("Error to open ranking file!");
		fprintf(rank, "%s %d ", first_name, first_score);
		fprintf(rank, "%s %d ", second_name, second_score);
		fprintf(rank, "%s %d ", third_name, third_score);
		printf("랭킹 파일에 기록되었습니다. 메인 화면으로 이동합니다.\n");
	} // if
}
int Bomb(int enemy_life[], int enemy_x[], int enemy_y[], int missilex[], int missiley[])
{
	int i, j;
	if(bomb==0){
		gotoxy(GS_WIDTH+2, 13);
		printf("NO BOMB AVAILABLE!");
		Sleep(100);
	} // if

	else{
		for(i=1; i<GS_HEIGHT; i++){
			gotoxy(GS_WIDTH-58, i);
			putchar('*');
			for(j=2; j<GS_WIDTH; j++){
				gotoxy(j, i);
				putchar('*');
			} // for
			Sleep(10);
		} // for
		Sleep(100);
		score+=enemy_draw;
		for(i=0; i<NoEnemy; i++){
			enemy_life[i]=0;
			enemy_x[i]=0;
			enemy_y[i]=0;
		} // for
		for(i=0; i<NoMissile; i++){
			missilex[i]=0;
			missiley[i]=0;
		} // for
		for(i=1; i<GS_HEIGHT; i++){
			gotoxy(GS_WIDTH-58, i);
			putchar(' ');
			for(j=2; j<GS_WIDTH; j++){
				gotoxy(j, i);
				putchar(' ');
			} // for
			Sleep(10);
		} // for
		enemy_draw=0;
		bomb--;
	} // else
	return 0;
}