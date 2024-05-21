#include <stdio.h>
#include "player.h"

player_t PLAYER;

void InitPlayer()
{
	PLAYER.x = 40;
	PLAYER.y = 20;
	PLAYER.hp = 5;
	PLAYER.Alive = true;
}

