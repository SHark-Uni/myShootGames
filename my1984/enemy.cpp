#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "enemy.h"

enemy_t g_enemy[ENEMY_MAX_COUNT];
char** g_enemyMovingFileName;
movingPattern_t g_movingPatterns[MAX_MOVING_PATTERN_COUNT];

static int movingPatternFILECnt;
static const int MAX_BUFFER_SIZE = 512;

typedef struct enemyData {
	int x;
	int y;
	unsigned int movingPattern;
}enemyData_t;

void LoadEnemy(const char* fileName)
{
	FILE* fp;
	enemyData_t* enemyData;
	int enemyDataCnt = 0;
	int enemyCnt = 0;

	char buffer[MAX_BUFFER_SIZE] = { 0,};

	fopen_s(&fp, fileName, "r");
	
	//extract pos info from file , strtok를 안써보는 방향으로도 구현해볼까
	//Reader Header
	fgets(buffer, MAX_BUFFER_SIZE, fp);
	
	fgets(buffer, MAX_BUFFER_SIZE, fp);
	sscanf_s(buffer, "%d", &enemyCnt);
	//Reader Header
	fgets(buffer, MAX_BUFFER_SIZE, fp);

	//Read Enemy Data
	enemyData = (enemyData_t*)malloc(sizeof(enemyData_t) * enemyCnt);
	for (int i = 0; i < enemyCnt; i++) {
		int enemyX = 0;
		int enemyY = 0;
		int movingPattern = 0;
		
		if (fgets(buffer, MAX_BUFFER_SIZE, fp) != NULL && sscanf_s(buffer, "%*c%*c%d%*c%d%*c%d%*c", &enemyY, &enemyX,&movingPattern) == 3) {
			enemyData[enemyDataCnt].x = enemyX;
			enemyData[enemyDataCnt].y = enemyY;
			enemyData[enemyDataCnt].movingPattern = movingPattern;
			++enemyDataCnt;
		};
	}


	// Spawn Enemy
	for (int i = 0; i < ENEMY_MAX_COUNT; i++) 
	{
		if (!g_enemy[i].isAlive && enemyDataCnt > 0)
		{
			g_enemy[i].x = enemyData[enemyDataCnt - 1].x;
			g_enemy[i].y = enemyData[enemyDataCnt - 1].y;
			g_enemy[i].movingPattern = enemyData[enemyDataCnt - 1].movingPattern;
			g_enemy[i].isAlive = true;
			--enemyDataCnt;
		}
	}

	free(enemyData);
	fclose(fp);
}

void LoadEnemyMovingPatternInfo()
{
	FILE* fp;
	const char* _PATH_MOVING_PATTERN = "movingPattern/";
	const char* delims = ":\n\t";
	const int _PATH_LENGTH = strlen(_PATH_MOVING_PATTERN);

	size_t size = 0;

	char* buffer = NULL;
	char* token = NULL;
	char* context = NULL;


	fopen_s(&fp, "movingPattern/movingInfo.txt", "r");
	if (fp == NULL) {
		return;
	}
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	rewind(fp);

	buffer = (char*)malloc(sizeof(char) * (size + 1));
	memset(buffer, 0, size + 1);

	fread(buffer, size, 1, fp);
	buffer[size] = '\0';

	//read Header
	token = strtok_s(buffer, delims, &context);
	token = strtok_s(NULL, delims, &context);

	sscanf_s(token, "%d", &movingPatternFILECnt);
	g_enemyMovingFileName = (char**)malloc(sizeof(char*) * movingPatternFILECnt);

	//read Header
	token = strtok_s(NULL, delims, &context);
	//Read FileNames 
	for (int i = 0; i < movingPatternFILECnt; i++)
	{
		size_t fileNameLength = 0;
		token = strtok_s(NULL, delims, &context);
		fileNameLength = strlen(token) + _PATH_LENGTH;

		char* tmp = (char*)malloc(sizeof(char) * (fileNameLength + 1));

		strcpy_s(tmp, fileNameLength + 1, _PATH_MOVING_PATTERN);
		strcat_s(tmp, fileNameLength + 1, token);
		tmp[fileNameLength] = '\0';

		g_enemyMovingFileName[i] = tmp;
	}

	free(buffer);
	fclose(fp);
}

void LoadEnemyMovingPattern()
{
	for (int i = 0; i < movingPatternFILECnt; i++) {
		FILE* fp;
		char buffer[MAX_BUFFER_SIZE] = { 0 , };
		int movementCount = 0;
		

		fopen_s(&fp, g_enemyMovingFileName[i], "r");

		//Read Header
		fgets(buffer, MAX_BUFFER_SIZE, fp);

		//Read Count
		fgets(buffer, MAX_BUFFER_SIZE, fp);
		sscanf_s(buffer, "%d", &movementCount);

		g_movingPatterns[i].movementNumber = movementCount;
		g_movingPatterns[i].movingPos = (pos_t*)malloc(sizeof(pos_t) * movementCount);

		//Read Header
		fgets(buffer, MAX_BUFFER_SIZE, fp);

		for (int j = 0; j < movementCount; j++) {
			int moveX = 0;
			int moveY = 0;
			//Read \t(x,y)\n
			fgets(buffer, MAX_BUFFER_SIZE, fp);
			sscanf_s(buffer, "%*c%*c%d%*c%d%*c%*c", &moveX, &moveY);
			g_movingPatterns[i].movingPos[j].x = moveX;
			g_movingPatterns[i].movingPos[j].y = moveY;
		}
		fclose(fp);
	}

}

void CleanUpEnemyMem()
{
	for (int i = 0; i < movingPatternFILECnt; i++) {
		free(g_movingPatterns[i].movingPos);
	}

	for (int i = 0; i < movingPatternFILECnt; i++) {
		free(g_enemyMovingFileName[i]);

	}
	free(g_enemyMovingFileName);
	g_enemyMovingFileName = NULL;

}

void InitEnemy()
{
	memset(g_enemy, 0, sizeof(enemy_t) * ENEMY_MAX_COUNT);
}