#include <windows.h>
#include <stdio.h>

#include "Console.h"
#include "player.h"
#include "Bullet.h"
#include "enemy.h"

HANDLE  hConsole;
char szScreenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];
menuPos_t menuCursorPos;

void DrawEnemy();
void DrawHp();
void DrawEnemyBullet();
void DrawPlayerBullet();
void DrawPlayer();

void cs_Initial(void)
{
	CONSOLE_CURSOR_INFO stConsoleCursor;

	stConsoleCursor.bVisible = FALSE;
	stConsoleCursor.dwSize = 1;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(hConsole, &stConsoleCursor);
}

void cs_MoveCursor(int iPosX, int iPosY)
{
	COORD stCoord;
	stCoord.X = iPosX;
	stCoord.Y = iPosY;
	SetConsoleCursorPosition(hConsole, stCoord);
}

void cs_ClearScreen(void)
{
	//int iCountX, iCountY;
	DWORD dw;

	FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', 100 * 100, { 0, 0 }, &dw);

}

void Sprite_Draw(int iX, int iY, char chSprite)
{
	if (iX < 0 || iY < 0 || iX >= dfSCREEN_WIDTH - 1 || iY >= dfSCREEN_HEIGHT)
		return;

	szScreenBuffer[iY][iX] = chSprite;
}

void DrawEnemy()
{
	for (int i = 0; i < ENEMY_MAX_COUNT; i++)
	{
		if (g_enemy[i].isAlive == true)
		{
			Sprite_Draw(g_enemy[i].x, g_enemy[i].y, '@');
		}
	}
}
void DrawHp()
{
	Sprite_Draw(0, 23, 'H');
	Sprite_Draw(1, 23, 'P');
	Sprite_Draw(2, 23, '0' + PLAYER.hp);
}
void DrawEnemyBullet()
{
	for (int i = 0; i < MAX_BULLET_COUNT; i++)
	{
		// 적을 먼저 표시한다. 총알은 그대로 진행.
		if (szScreenBuffer[g_enemyBullet[i].y][g_enemyBullet[i].x] != ' ')
		{
			continue;
		}
		if (g_enemyBullet[i].isAlive == true) {
			Sprite_Draw(g_enemyBullet[i].x, g_enemyBullet[i].y, '*');
		}
	}
}

void DrawPlayerBullet()
{
	for (int i = 0; i < MAX_BULLET_COUNT; i++)
	{
		if (g_playerBullet[i].isAlive == true) {
			Sprite_Draw(g_playerBullet[i].x, g_playerBullet[i].y, '*');
		}
	}
}

void DrawClearScene()
{
	strncpy_s(szScreenBuffer[5], "       !! Congratulation !!     ", 32);
	strncpy_s(szScreenBuffer[6], "      !!!! GAME CLEAR !!!!!     ", 32);
}

void DrawMainScreen()
{
	strncpy_s(szScreenBuffer[5], "                         G A M E                         ", 57);
	strncpy_s(szScreenBuffer[15], "                        START                        ", 53);
	strncpy_s(szScreenBuffer[16], "                        END                        ", 52);

	//커서 그리기
	Sprite_Draw(menuCursorPos.x, menuCursorPos.y, '>');
}

void DrawPlayer()
{
	Sprite_Draw(PLAYER.x, PLAYER.y, 'P');
}

//Render
void Buffer_Flip(void)
{
	int iCnt;
	for (iCnt = 0; iCnt < dfSCREEN_HEIGHT; iCnt++)
	{
		cs_MoveCursor(0, iCnt);
		printf(szScreenBuffer[iCnt]);
	}
}

void Buffer_Clear(void)
{
	int iCnt;
	memset(szScreenBuffer, ' ', dfSCREEN_WIDTH * dfSCREEN_HEIGHT);

	for (iCnt = 0; iCnt < dfSCREEN_HEIGHT; iCnt++)
	{
		szScreenBuffer[iCnt][dfSCREEN_WIDTH - 1] = '\0';
	}

}


void RenderAllElements()
{
	DrawPlayer();
	DrawEnemy();
	DrawHp();
	DrawEnemyBullet();
	DrawPlayerBullet();
}
