#ifndef TILES_H
#define TILES_H

#include "alchemist/elements.h"
#include "object.h"

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
    BIOME_FOREST, //  green
    BIOME_SWEET_TREE,//brown
    BIOME_LAKE //blue
};


#define CHUNK_SIZE 16
#define WORLD_SIZE 256
#define WORLD_CENTER WORLD_SIZE/2
#define WORLD_SCALE 100.0

struct tile {
    enum game_tiles tile;
    int weight;
};
    
typedef struct tile chunk_table[CHUNK_SIZE][CHUNK_SIZE];

typedef struct {
    enum biomes biome;
    chunk_table table;
    // TODO: change objects and items arrays to lists
    struct object objects[CHUNK_SIZE][CHUNK_SIZE];
    InventoryElement * items[128];
} chunk;



#endif
