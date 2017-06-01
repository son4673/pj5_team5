/* 20700486 이성현
   Term-Project: STRIKERS 1945
   Visual C++ */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h> // for getch

#include "Console.h"

// Global Variables Declaration
char selection=0; // 메뉴 선택 변수
char first_name[3];
char second_name[3];
char third_name[3]; // 랭킹의 1, 2, 3등의 이름
int first_score;
int second_score;
int third_score; // 랭킹의 1, 2, 3등의 점수

// Functions Declaration
int StartMenu(char selection); // 메인 메뉴
void ViewRanking();
int GamePlay();
void HelpScreen();
void ExitGame();
void DrawBoundary();
void DrawLogo();
void CountRanking(); // 랭킹을 파일로부터 불러오는 역할 수행

// main function
int main()
{
	CountRanking();
	while(1){
		selection=StartMenu(selection);
		switch(selection){
		case '1': // 게임 시작
			GamePlay();
			return 0;
		case '2': // Ranking 확인
			ViewRanking();
			break;
		case '3': // 도움말
			HelpScreen();
			break;
		case '4': // 게임 종료
			ExitGame();
			system("PAUSE");
			return 0;
			break;
		} // switch
	} // while
	return 0;
}
void CountRanking() // 랭킹을 파일로부터 불러오는 함수
{
	int i=0;
	FILE *rank=NULL;
	while(i==0){
		rank=fopen("1945rk.dat", "r");
		if(rank!=NULL) // 파일이 존재한다
			i=1;
		else if(rank==NULL){ // 파일이 존재하지 않거나 에러가 있다
			
			clrscr();

			DrawBoundary();
			DrawLogo();

			gotoxy(5,5);
			printf("Error to open file!\n");
			gotoxy(5,6);
			printf("File open error! 랭킹파일을 새로 작성합니다.");
			gotoxy(5,7);
			system("PAUSE");
		
			// 랭킹 파일 작성
			rank=fopen("1945rk.dat", "w");
			fprintf(rank, "COa 500 ");
			fprintf(rank, "COb 200 ");
			fprintf(rank, "COc 100 ");
		
			fclose(rank);
		} // else if
	} // while
	// 랭킹 파일에 있는 데이타들을 불러오는 함수
	fscanf(rank, "%s", first_name);
	fscanf(rank, "%d", &first_score);
	fscanf(rank, "%s", second_name);
	fscanf(rank, "%d", &second_score);
	fscanf(rank, "%s", third_name);
	fscanf(rank, "%d", &third_score);
	fclose(rank);
}
int StartMenu(char selection) // Menu
{
	do{
		clrscr();
		DrawBoundary();
		DrawLogo();
		gotoxy(10,5);
		printf("1. 게임 시작");
		gotoxy(10,8);
		printf("2. Ranking 확인");
		gotoxy(10,11);
		printf("3. 도움말");
		gotoxy(10,14);
		printf("4. 게임 종료");
		gotoxy(10,20);
		printf(" 번호 입력: ");
		selection=getch();
		if(selection!='1'&&selection!='2'&&selection!='3'&&selection!='4'){
			printf("#    잘못 입력하셨습니다. 다시 입력하세요.\n");
			printf("#    ");
			system("PAUSE");
		} // if
	} while(selection!='1'&&selection!='2'&&selection!='3'&&selection!='4'); // while
	return selection;
}