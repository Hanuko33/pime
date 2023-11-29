#ifndef DUNGEON_H
#define DUNGEON_H
#include <stdio.h>
#include <stdlib.h>

enum  game_tiles
{
    TILE_STONE,
    TILE_DIRT,
    TILE_TREE,
    TILE_DUNG_WALL,
    TILE_DUNG_FLOOR,
    TILE_DUNG_EXIT,
    TILE_DUNG_ENTRANCE,
    TILE_SAND,
    TILE_SANDSTONE,
    TILE_SWEET_TREE,
    TILE_SWEET_BUSH,
    TILE_SWEET_FLOWER,
    TILE_GRASS,
    TILE_SWEET_GRASS,
    TILE_WATER
};

#define DUNGEON_SIZE 16

typedef enum game_tiles tile_table[DUNGEON_SIZE][DUNGEON_SIZE];
extern tile_table dungeon_terrain_list;

void dungeon_generator(int start_x, int start_y);

#endif
