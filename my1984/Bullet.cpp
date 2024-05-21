#include <iostream>

#include "Bullet.h"
#include "enemy.h"
#include "player.h"

bullet_t g_enemyBullet[MAX_BULLET_COUNT];
bullet_t g_playerBullet[MAX_BULLET_COUNT];

void EnemyAttack()
{
	static unsigned int enemyShootSpeed = 0;
	pos_t BulletPos[ENEMY_MAX_COUNT] = { 0 , };
	int cursor = 0;

	if (enemyShootSpeed > 50) {
		for (int i = 0; i < ENEMY_MAX_COUNT; i++) {
			if (g_enemy[i].isAlive == true)
			{
				BulletPos[cursor].x = g_enemy[i].x;
				BulletPos[cursor].y = g_enemy[i].y + 1;
				cursor++;
			}
		}

		for (int i = 0; i < MAX_BULLET_COUNT; i++) {
			if (cursor <= 0)
			{
				break;
			}
			if (g_enemyBullet[i].isAlive == false)
			{
				g_enemyBullet[i].x = BulletPos[cursor - 1].x;
				g_enemyBullet[i].y = BulletPos[cursor - 1].y;
				g_enemyBullet[i].isAlive = true;
				--cursor;
			}
		}
		enemyShootSpeed = 0;
	}
	enemyShootSpeed++;


	
}

void playerAttack()
{
	static unsigned int playerShootSpeed = 0;
	if (playerShootSpeed > 3)
	{
		for (int i = 0; i < MAX_BULLET_COUNT; i++)
		{
			if (g_playerBullet[i].isAlive == false)
			{
				g_playerBullet[i].x = PLAYER.x;
				g_playerBullet[i].y = PLAYER.y - 1;
				g_playerBullet[i].isAlive = true;
				break;
			}
		}
		playerShootSpeed = 0;
	}
	playerShootSpeed++;
}

void hitDetection()
{
	//Player 공격판정 먼저
	for (int i = 0; i < MAX_BULLET_COUNT; i++) 
	{
		if (g_playerBullet[i].isAlive)
		{
			for (int j = 0; j < ENEMY_MAX_COUNT; j++)
			{
				if (g_enemy[j].isAlive && g_playerBullet[i].x == g_enemy[j].x && g_playerBullet[i].y == g_enemy[j].y)
				{
					g_enemy[j].isAlive = false;
					g_playerBullet[i].isAlive = false;
					break;
				}
			}
		}
	}


	//적 공격판정
	for (int i = 0; i < MAX_BULLET_COUNT; i++) 
	{
		if (g_enemyBullet[i].isAlive)
		{
			if (PLAYER.Alive == true && (g_enemyBullet[i].x == PLAYER.x) && (g_enemyBullet[i].y == PLAYER.y))
			{
				g_enemyBullet[i].isAlive = false;
				PLAYER.hp -= 1;
			}
		}
	}

}

void InitBullet()
{
	memset(g_playerBullet, 0, sizeof(bullet_t) * MAX_BULLET_COUNT);
	memset(g_enemyBullet, 0, sizeof(bullet_t) * MAX_BULLET_COUNT);
}

