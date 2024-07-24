#ifndef TILES_H
#define TILES_H

#include "alchemist/elements.h"


enum  game_tiles
{
    TILE_AIR,
    TILE_STONE,
    TILE_DIRT,
    TILE_SAND,
    TILE_SANDSTONE,
    TILE_SWEET_TREE,
    TILE_SWEET_BUSH,
    TILE_SWEET_FLOWER,
    TILE_GRASS,
    TILE_SWEET_GRASS,
    TILE_WATER,
    TILE_MAX_NUM
};

enum  biomes
{
    BIOME_DESERT, // yellow
    BIOME_FOREST, // green with trees
    BIOME_PLAINS, // green
};


#define CHUNK_SIZE 16
#define WORLD_SIZE 256
#define WORLD_CENTER WORLD_SIZE/2
#define WORLD_SCALE 100.0

struct tile {
    enum game_tiles tile;
};
    
typedef struct tile chunk_table[CHUNK_SIZE][CHUNK_SIZE];

typedef struct {
    enum biomes biome;
    chunk_table table;
    InventoryElement * items[CHUNK_SIZE*CHUNK_SIZE];
    Being * beings[CHUNK_SIZE*CHUNK_SIZE];
} chunk;



#endif
