#pragma once
#define MAX_BULLET_COUNT (64)

typedef struct Bullet {
	bool isAlive;

	int x;
	int y;
}bullet_t;

extern bullet_t g_enemyBullet[MAX_BULLET_COUNT];
extern bullet_t g_playerBullet[MAX_BULLET_COUNT];

void EnemyAttack();
void playerAttack();
void hitDetection();
void InitBullet();