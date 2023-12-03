#ifndef PLAYER_H
#define PLAYER_H


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
    int z;
    int y;
    int x;
	int back_y;
	int back_x;
    char running;
    char going_right;
    enum location in;
    int energy;
};

void player_move(struct Player* player, int x, int y);



#endif
