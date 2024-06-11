#ifndef __CONSOLE__
#define __CONSOLE__

#define dfSCREEN_WIDTH		81		// �ܼ� ���� 80ĭ + NULL
#define dfSCREEN_HEIGHT		24		// �ܼ� ���� 24ĭ
#define MENU_START_LINE (15)
#define MENU_END_LINE (16)

extern char szScreenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];

typedef struct menuPos {
	int x;
	int y;
} menuPos_t;

extern menuPos_t menuCursorPos;

void cs_Initial(void);
void cs_MoveCursor(int iPosX, int iPosY);
void cs_ClearScreen(void);

//���� Draw
void Sprite_Draw(int iX, int iY, char chSprite);

void RenderAllElements();
void DrawMainScreen();
void DrawClearScene();
//Render
void Buffer_Flip(void);
void Buffer_Clear(void);
#endif
