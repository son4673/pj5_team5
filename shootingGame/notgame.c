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
// global variable
extern char first_name[3];
extern char second_name[3];
extern char third_name[3]; // 랭킹의 1, 2, 3등의 이름
extern int first_score;
extern int second_score;
extern int third_score;
extern int highest; // 랭킹의 1, 2, 3등, 최고 점수

// function declaration
void DrawLogo();
void DrawBoundary();
void HelpScreen(); // 도움말
void ExitGame(); // 게임 종료
void ViewRanking(); // 랭킹 보기

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
void ViewRanking() // 랭킹 보기
{	
	clrscr();
	DrawBoundary();
	DrawLogo();
	gotoxy(5, 8);
	printf("\tRANKING:");
	gotoxy(5, 10);
	printf("  1. %s  %d", first_name, first_score);
	gotoxy(5, 11);
	printf("  2. %s  %d", second_name, second_score);
	gotoxy(5, 12);
	printf("  3. %s  %d", third_name, third_score);
	gotoxy(5, 15);
	system("PAUSE");
}