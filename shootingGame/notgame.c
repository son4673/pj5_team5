/*
	notgame.c: functions that are not related with game itself.
*/

// header file
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include "Console.h"
// define constant value
#define ScreenWidth 80
#define ScreenHeight 24 

// function declaration
void DrawLogo();
void DrawBoundary();
void HelpScreen(); // 도움말
void ExitGame(); // 게임 종료

// function definition
void DrawLogo()
{
	gotoxy(17,1);
	printf("  *** STRIKERS 1945 - C Programming Version ***  ");
}
void DrawBoundary()
{
	int i=0;
	clrscr();
	gotoxy(1,1);
	for(i=0; i<ScreenWidth; i++)
		putchar('#');
	for(i=1; i<=ScreenHeight; i++){
		gotoxy(1,i);
		putchar('#');
		gotoxy(ScreenWidth, i);
		putchar('#');
	} // for
	gotoxy(1, ScreenHeight);
	for(i=0; i<ScreenWidth; i++)
		putchar('#');
}
void HelpScreen() // 도움말
{
	clrscr();
	DrawBoundary();
	DrawLogo();
	gotoxy(10,5);
	printf("\t키 설명:");
	gotoxy(10,7);
	printf("좌로 이동: j");
	gotoxy(10,8);
	printf("우로 이동: k");
	gotoxy(10,10);
	printf("미사일 발사: a");
	gotoxy(10,13);
	printf("폭탄 투하: s");
	gotoxy(10,14);
	printf("폭탄은 일정한 시간이 지날수록 하나씩 회복됩니다.");
	gotoxy(10,20);
	printf("Made by Sung-Hyun, Lee");
	gotoxy(10,23);
	system("PAUSE");
}
void ExitGame() // 게임 종료
{
	clrscr();
	printf("STRIKERS 1945 C-Programming Version.\n");
	printf("Made by Sung-Hyun, Lee\n\n");
	printf("e-mail: caleb-@hanmail.net\n");
}