#ifndef TILES_H
#define TILES_H

#include "alchemist/elements.h"
#include "object.h"

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
    BaseElement* tile;
    int weight;
};
    
typedef struct tile chunk_table[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

typedef struct {
    enum biomes biome;
    chunk_table table;
    // TODO: change objects and items arrays to lists
    struct object* objects[128];
    InventoryElement * items[128];
} chunk;



#endif
