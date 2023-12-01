#ifndef PLAYER_H
#define PLAYER_H

#include "dungeon.h"

enum location
{
    LOC_WORLD,
    LOC_DUNGEON,
    LOC_CAVE
};

struct Player
{
    int map_y;
    int map_x;
    int y;
    int x;
	int back_y;
	int back_x;
    char running;
    char going_right;
    enum location in;
    int energy;
};

extern struct Player player;
void player_interact(int key );

#endif
