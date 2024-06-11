#include "move.h"
#include "player.h"
#include "Bullet.h"
#include "enemy.h"
#include "console.h"


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

void moveAllElements()
{
	moveEnemy();
	movePlayerBullet();
	moveEnemyBullet();
}