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
#include "move.h"

const int FRAME_PER_MS = 20;

typedef enum gameState {
	TITLE,
	SCENCE,
	CLEAR,
	DEAD,
	LOADING
} eGameState;

eGameState GAMESTATE;

unsigned int gameStageState;
int numberOfStage;

void InitGame();
bool checkGameClear();
void checkPlayerDead();

int main(void)
{
	
	DWORD nextTickTime;
	int sleepTime;

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
			DrawMainScreen();
			// KeyInput
			if (GetAsyncKeyState(VK_UP)) 
			{
				if (menuCursorPos.y > MENU_START_LINE)
				{
					menuCursorPos.y--;
				}
				
			}
			if (GetAsyncKeyState(VK_DOWN)) 
			{
				if (menuCursorPos.y <= MENU_END_LINE)
				{
					menuCursorPos.y++;
				}
			}
			if (GetAsyncKeyState(VK_RETURN)) 
			{
				if (menuCursorPos.y == MENU_START_LINE)
				{
					GAMESTATE = LOADING;
					gameStageState = 0;
				}
				// GAME 종료
				if (menuCursorPos.y == MENU_END_LINE)
				{
					goto RETURNS;
				}
			}
			break;
		case LOADING:
			if (checkGameClear())
			{
				continue;
			}
			InitPlayer();
			InitEnemy();
			InitBullet();
			InitStage(gameStageState);
			GAMESTATE = SCENCE;
			break;
		case SCENCE:
			Buffer_Clear();
			// 플레이어 키 입력부
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
			if (GetAsyncKeyState('Z'))
			{
				playerAttack();
			}
		
			EnemyAttack();
			hitDetection();
			checkPlayerDead();

			if (CheckStageClear())
			{
				GAMESTATE = LOADING;
				++gameStageState;
				continue;
			}

			moveAllElements();
			RenderAllElements();
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

void InitGame()
{
	GAMESTATE = TITLE;
	menuCursorPos.y = MENU_START_LINE;
	menuCursorPos.x = 23;

	gameStageState = -1;

	InitEnemy();
	InitBullet();
}


bool checkGameClear()
{
	if (gameStageState >= numberOfStage)
	{
		GAMESTATE = CLEAR;
		return true;
	}
	return false;
}

void checkPlayerDead()
{
	if (PLAYER.hp < 0)
	{
		GAMESTATE = DEAD;
		return;
	}
}
