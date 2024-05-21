#pragma once

extern char** g_fileNames;

typedef enum ParsingErrorCode {
	WRONG_FILE_NAME = -1,
	FAIL_TO_GET_STAGE_NUMBER = -2,
}ParsingErrorCode_t;

//Stage ������ �����մϴ�.
int ReadStageInfo();
void InitStage(const int stageNumber);
void CleanUpStageMem();
bool CheckStageClear();
