#pragma comment (lib, "winmm")
#include <stdio.h>
#include <memory.h>
#include <Windows.h>
#include <mmsystem.h>


#include "Console.h"
#include "stage.h"
#include "enemy.h"
#include "Bullet.h"
#include "player.h"

const int START_LINE = 15;
const int END_LINE = 16;
const int FRAME_PER_MS = 20;

typedef enum gameState {
	TITLE,
	SCENCE,
	CLEAR,
	DEAD
} eGameState;

typedef struct menuPos {
	int x;
	int y;
} menuPos_t;

eGameState GAMESTATE;
menuPos_t menuCursorPos;



unsigned int gameStageState;
bool bFirstTimeOfStage;
char szScreenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];

void Buffer_Flip(void);
void Buffer_Clear(void);
void Sprite_Draw(int iX, int iY, char chSprite);
void LoadMainScreen();

void InitGame();

void DrawEnemy();
void DrawEnemyBullet();
void DrawPlayerBullet();
void DrawClearScene();
void DrawHp();


void moveEnemy();
void moveEnemyBullet();
void movePlayerBullet();

int main(void)
{
	
	DWORD nextTickTime;
	int sleepTime;
	int numberOfStage;
	int g_test = 0;
	cs_Initial();
	timeBeginPeriod(1);

	numberOfStage = ReadStageInfo();
	LoadEnemyMovingPatternInfo();
	LoadEnemyMovingPattern();
	
	InitGame();
	nextTickTime = timeGetTime();
	//총알 장전
	while (1)
	{
		switch (GAMESTATE)
		{
		case TITLE:
			Buffer_Clear();
			//Moving Menu Cursor
			if (GetAsyncKeyState(VK_UP)) 
			{
				if (menuCursorPos.y > START_LINE)
				{
					menuCursorPos.y--;
				}
				
			}
			if (GetAsyncKeyState(VK_DOWN)) 
			{
				if (menuCursorPos.y <= START_LINE)
				{
					menuCursorPos.y++;
				}
			}
			LoadMainScreen();
			if (GetAsyncKeyState(VK_RETURN)) 
			{
				if (menuCursorPos.y == START_LINE) 
				{
					GAMESTATE = SCENCE;
					gameStageState = 0;
				}
				// GAME 종료
				if (menuCursorPos.y == END_LINE) 
				{
					goto RETURNS;
				}
			}
			break;
		case SCENCE:
			Buffer_Clear();
			
			if (gameStageState >= numberOfStage)
			{
				GAMESTATE = CLEAR;
				continue;
			}
			// 스테이지 처음 진입 시, 적의 정보를 넣어줘야한다.
			// 그 뒤로는 갱신되면 안됨. 시작 위치에 적이 계속 로딩되는건 말이 안됨. 
			if (bFirstTimeOfStage == true) {
				InitPlayer();
				InitEnemy();
				InitBullet();
				InitStage(gameStageState);
				bFirstTimeOfStage = false;
			}
			// 입력부
			if (GetAsyncKeyState(VK_LEFT)) 
			{	
				if (PLAYER.x > 0) {
					PLAYER.x--;
				}
			}
				
			if (GetAsyncKeyState(VK_RIGHT))
			{
				if (PLAYER.x < dfSCREEN_WIDTH - 1) {
					PLAYER.x++;
				}
			}
				
			if (GetAsyncKeyState(VK_UP))
			{
				if (PLAYER.y > 0) {
					PLAYER.y--;
				}
			}
				
			if (GetAsyncKeyState(VK_DOWN))
			{
				if (PLAYER.y < dfSCREEN_HEIGHT) {
					PLAYER.y++;
				}
			}
			//Player Attack
			if (GetAsyncKeyState('Z'))
			{
				playerAttack();
			}
		
			Sprite_Draw(PLAYER.x, PLAYER.y, 'P');
			EnemyAttack();
			//공격판정
			hitDetection();
			//플레이어 생존여부 확인
			if (PLAYER.hp < 0)
			{
				GAMESTATE = DEAD;
				continue;
			}

			if (CheckStageClear())
			{
				bFirstTimeOfStage = true;
				gameStageState++;
				continue;
			}

			moveEnemy();
			movePlayerBullet();
			moveEnemyBullet();

			//Render
			DrawEnemy();
			DrawHp();
			DrawEnemyBullet();
			DrawPlayerBullet();
			break;
		case CLEAR:
			Buffer_Clear();
			DrawClearScene();
			Buffer_Flip();
			goto RETURNS;
			break;
		case DEAD:
			InitGame();
			InitPlayer();
			break;
		}
		Buffer_Flip();
		nextTickTime += FRAME_PER_MS;
		sleepTime = nextTickTime - timeGetTime();
		if (sleepTime >= 0) {
			Sleep(sleepTime);
		}
		// 프레임 맞추기용 대기 Sleep(X)
	}
RETURNS:
	CleanUpStageMem();
	CleanUpEnemyMem();
	return 0;
}

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

void Sprite_Draw(int iX, int iY, char chSprite)
{
	if (iX < 0 || iY < 0 || iX >= dfSCREEN_WIDTH - 1 || iY >= dfSCREEN_HEIGHT)
		return;

	szScreenBuffer[iY][iX] = chSprite;
}


void LoadMainScreen()
{
	strncpy_s(szScreenBuffer[5], "                         G A M E                         ", 57);
	strncpy_s(szScreenBuffer[15], "                        START                        ", 53);
	strncpy_s(szScreenBuffer[16], "                        END                        ", 52);

	//커서 그리기
	Sprite_Draw(menuCursorPos.x, menuCursorPos.y, '>');
}

void DrawClearScene()
{
	strncpy_s(szScreenBuffer[5], "       !! Congratulation !!     ", 32);
	strncpy_s(szScreenBuffer[6], "      !!!! GAME CLEAR !!!!!     ", 32);
}


void InitGame()
{
	GAMESTATE = TITLE;
	menuCursorPos.y = START_LINE;
	menuCursorPos.x = 23;

	gameStageState = -1;
	bFirstTimeOfStage = true;

	InitEnemy();
	InitBullet();
}


void DrawHp()
{

	Sprite_Draw(0, 23, 'H');
	Sprite_Draw(1, 23, 'P');
	Sprite_Draw(2, 23, '0' + PLAYER.hp);
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

void moveEnemy()
{
	static unsigned int enemyMoveSpeed = 0;
	static unsigned int movePattern_cursor = 0;
	if (enemyMoveSpeed > 30)
	{
		for (int i = 0; i < ENEMY_MAX_COUNT; i++)
		{
			if (g_enemy[i].isAlive == true)
			{
				int pattern = g_enemy[i].movingPattern - 1;
				int cur = movePattern_cursor % (g_movingPatterns[pattern].movementNumber);

				g_enemy[i].x += g_movingPatterns[pattern].movingPos[cur].x;
				g_enemy[i].y += g_movingPatterns[pattern].movingPos[cur].y;
			}
		}
		movePattern_cursor++;
		enemyMoveSpeed = 0;
	}
	enemyMoveSpeed++;
}

void moveEnemyBullet()
{
	static unsigned int enemyBulletSpeed = 0;

	if (enemyBulletSpeed > 30)
	{
		for (int i = 0; i < MAX_BULLET_COUNT; i++)
		{
			if (g_enemyBullet[i].isAlive == true)
			{
				if (g_enemyBullet[i].y > dfSCREEN_HEIGHT)
				{
					g_enemyBullet[i].isAlive = false;
					continue;
				}
				g_enemyBullet[i].y++;
			}
		}
		enemyBulletSpeed = 0;
	}
	enemyBulletSpeed++;
}

void movePlayerBullet()
{
	static unsigned int playerBulletSpeed = 0;

	if (playerBulletSpeed > 3)
	{
		for (int i = 0; i < MAX_BULLET_COUNT; i++)
		{
			if (g_playerBullet[i].isAlive == true) {

				if (g_playerBullet[i].y < 0)
				{
					g_playerBullet[i].isAlive = false;
					continue;
				}
				g_playerBullet[i].y--;
			}
		}
		playerBulletSpeed = 0;
	}
	playerBulletSpeed++;
}


