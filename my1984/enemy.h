#pragma once
#define ENEMY_MAX_COUNT (30)
#define MAX_MOVING_PATTERN_COUNT (64)

typedef struct enemy {
	bool isAlive;
	int x;
	int y;
	unsigned int movingPattern;
}enemy_t;

typedef struct pos {
	int x;
	int y;
}pos_t;

typedef struct Pattern {
	int movementNumber;
	pos_t* movingPos;
}movingPattern_t;

extern enemy_t g_enemy[ENEMY_MAX_COUNT];
extern movingPattern_t g_movingPatterns[MAX_MOVING_PATTERN_COUNT];
extern char** g_enemyMovingFileName;

void LoadEnemy(const char* fileName);
void LoadEnemyMovingPatternInfo();
void LoadEnemyMovingPattern();
void CleanUpEnemyMem();
void InitEnemy();