#ifndef PLAYER_H
#define PLAYER_H

#include "dungeon.h"

struct Player
{
    int map_y;
    int map_x;
    int y;
    int x;
    bool running;
    void interact(int key, Dungeon& dungeon);
    bool going_right;
    bool in_dungeon;
    int energy = 250;
};

extern Player player;

#endif
