#ifndef PLAYER_H
#define PLAYER_H

#include "dungeon.h"

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
    int in_dungeon;
    int in_cave;
    int energy;
};

extern struct Player player;
void player_interact(int key );

#endif
