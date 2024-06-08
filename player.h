#ifndef PLAYER_H
#define PLAYER_H
#include "alchemist/el_list.h"

enum location
{
    LOC_WORLD,
    LOC_DUNGEON,
    LOC_CAVE
};


enum class direction
{
    up,
    right,
    down,
    left,
};

class Player
{
	void check_and_move(int new_map_x, int new_map_y, int new_x, int new_z);

public:
    int map_y;
    int map_x;
    int y;
    int x;
	int back_y;
	int back_x;
    char running;
    char sneaking;
    char going_right;
    enum direction direction;
    enum location in;
    int thirst;
    int hunger;
    InvList * inventory; 
    InventoryElement * hotbar[10];
    int craftbar[10];
	void move(int x, int z);
	Player();

};



#endif
