#pragma once

typedef struct player {
	int x;
	int y;
	int hp;
	bool Alive;
}player_t;

extern player_t PLAYER;

void InitPlayer();
