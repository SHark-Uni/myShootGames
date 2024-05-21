#pragma warning(disable : 6387)
#pragma warning(disable : 6011)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stage.h"
#include "enemy.h"

const int BUFFER_SIZE = 512;
const int STAGE_DIC_PATH = 6;
char** g_fileNames = NULL;

static int stageCnt = 0;

int ReadStageInfo()
{
	FILE* fp;
	char* buffer = NULL;
	size_t size = 0;
	const char* delims = ":\n\t";
	char* token = NULL;
	char* context = NULL;

	fopen_s(&fp, "stage/stageInfo.txt", "r");
	
	if (fp == NULL) {
		stageCnt = WRONG_FILE_NAME;
		goto END_OF_READ_INFO;
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	rewind(fp);

	buffer = (char*)malloc(sizeof(char)* (size + 1));
	memset(buffer, 0, size + 1);

	fread(buffer, size, 1 ,fp);
	buffer[size] = '\0';

	//read Header
	token = strtok_s(buffer, delims, &context);

	token = strtok_s(NULL, delims, &context);
	if (sscanf_s(token, "%d", &stageCnt) != 1) {
		stageCnt = FAIL_TO_GET_STAGE_NUMBER;
		goto END_OF_READ_INFO;
	}
	g_fileNames = (char**)malloc(sizeof(char*) * stageCnt);
	
	//read Header
	token = strtok_s(NULL, delims, &context);
	//Read FileNames 
	for (int i = 0; i < stageCnt; i++) 
	{
		size_t fileNameLength = 0;
		token = strtok_s(NULL, delims, &context);
		fileNameLength = strlen(token) + STAGE_DIC_PATH;

		char* tmp = (char*)malloc(sizeof(char) * (fileNameLength + 1));
		
		strcpy_s(tmp, fileNameLength + 1, "stage/");
		strcat_s(tmp, fileNameLength + 1, token);
		tmp[fileNameLength] = '\0';

		g_fileNames[i] = tmp;
	}

END_OF_READ_INFO:
	free(buffer);
	fclose(fp);

	return stageCnt;
}

void InitStage(const int stageNumber)
{
	LoadEnemy(g_fileNames[stageNumber]);
}

void CleanUpStageMem()
{
	for (int i = 0; i < stageCnt; i++) {
		free(g_fileNames[i]);
	}
	free(g_fileNames);
	g_fileNames = NULL;
}

bool CheckStageClear()
{
	for (int i = 0; i < ENEMY_MAX_COUNT; i++)
	{
		if (g_enemy[i].isAlive)
		{
			return false;
		}
	}
	return true;
}