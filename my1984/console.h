#ifndef __CONSOLE__
#define __CONSOLE__

#define dfSCREEN_WIDTH		81		// 콘솔 가로 80칸 + NULL
#define dfSCREEN_HEIGHT		24		// 콘솔 세로 24칸
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

//각종 Draw
void Sprite_Draw(int iX, int iY, char chSprite);

void RenderAllElements();
void DrawMainScreen();
void DrawClearScene();
//Render
void Buffer_Flip(void);
void Buffer_Clear(void);
#endif
