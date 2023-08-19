#ifndef DUNGEON_H
#define DUNGEON_H
#define DUNGEON_SIZE 16
#include <stdio.h>
#include <stdlib.h>

enum class game_tiles
{
    STONE,
    DIRT,
    TREE,
    DUNG_WALL,
    DUNG_FLOOR,
    DUNG_EXIT,
    DUNG_ENTRANCE
};

class Dungeon
{
    public:
        game_tiles dungeon_terrain_list[DUNGEON_SIZE][DUNGEON_SIZE];
        void generator();
};

#endif
