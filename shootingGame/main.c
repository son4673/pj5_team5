/*
1차 개발

20700486 이성현

Term-Project: STRIKERS 1945

Visual C++
*/
/*
2차 개발

main.c/Console.c/game_play.c/notgame.c

Team-Project : 오픈소스SW개론

Created by OSS_TEAM5 on 2017. 6.

Copyright @ 2017. 5. OSS_TEAM5. All rights reserved.

Visual C++

*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h> // for getch
#include "Console.h"

// Global Variables Declaration
char selection = 0; // 메뉴 선택 변수

				 // Functions Declaration
int StartMenu(char selection); // 메인 메뉴
int GamePlay();
void HelpScreen();
void ExitGame(); // 게임 종료 역할 수행

				 //경계와 로고 그리기
void DrawBoundary();
void DrawLogo();

					 // main function
int main()
{
	RankLoad();

	while (1)
	{

		selection = StartMenu(selection);
		switch (selection) {
		case '1': // 게임 시작
			GamePlay();
			break;
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

int StartMenu(char selection) // Menu
{
	do {
		clrscr();
		DrawBoundary();
		DrawLogo();
		gotoxy(10, 5);
		printf("1. 게임 시작");
		gotoxy(10, 8);
		printf("2. Ranking 확인");
		gotoxy(10, 11);
		printf("3. 도움말");
		gotoxy(10, 14);
		printf("4. 게임 종료");
		gotoxy(10, 20);
		printf(" 번호 입력: ");
		selection = getch();
		if (selection != '1'&&selection != '2'&&selection != '3'&&selection != '4') {
			printf("#    잘못 입력하셨습니다. 다시 입력하세요. \n");
			printf("#    ");
			system("PAUSE");
		} // if
	} while (selection != '1'&&selection != '2'&&selection != '3'&&selection != '4'); // while
	return selection;
}