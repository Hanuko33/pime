#include "alchemist/elements.h"
#include "object.h"
#include "player.h"
#include "world.h"
//#include "cave.h"
//#include "dungeon.h"
#include "tiles.h"
#include "time.h"
#include "notifier.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "OpenSimplex/OpenSimplex2F.h"

struct OpenSimplex2F_context *simplex_context;

chunk * world_table[WORLD_SIZE][WORLD_SIZE];

void generator()
{
    OpenSimplex2F(time(NULL), &simplex_context);

    load_chunk(WORLD_CENTER, WORLD_CENTER);
}

int height_at(int chunk_x, int chunk_y, int x, int y)
{
    return 1 + 
        (OpenSimplex2F_noise2(simplex_context, 
                (chunk_x * CHUNK_SIZE + x) / WORLD_SCALE, 
                (chunk_y * CHUNK_SIZE + y) / WORLD_SCALE) + 1) *
        (CHUNK_SIZE - 2) / 2.0;
}

void create_biome_forest(chunk * chunk)
{
    /*int type_int = 0;
    int random = 0;
    char chunk_contains_dung_entrance = 0;
    char chunk_contains_cave_entrance = 0;
    int it_randnum;
    for (int i=0; i<CHUNK_SIZE; i++)
    {
        for (int j=0; j<CHUNK_SIZE; j++)
        {
            it_randnum = rand() % 10;
            type_int = rand() % 4;
            switch (type_int)
            {
                case 0:
                    random = rand() % 75;
                    if (random < 10 && !(chunk_contains_cave_entrance))
                    {
                        chunk->table[0][i][j].tile = TILE_CAVE_DOOR;
                        chunk_contains_cave_entrance = 1;
                        generate_cave(chunk, j, i);
                    }
                    else
                    {
                        chunk->table[0][i][j].tile = TILE_STONE;
                        chunk->table[0][i][j].item.count = (it_randnum > 5) ? it_randnum/4 : 0;
                        if (chunk->table[0][i][j].item.count)
                            chunk->table[0][i][j].item.id=IT_stone;

                    }
                    break;
                case 1:
                    chunk->table[0][i][j].tile = TILE_DIRT;
                    chunk->table[0][i][j].item.count = (it_randnum > 5) ? it_randnum/4 : 0;
                    if (chunk->table[0][i][j].item.count) {
                        if (rand() % 10 < 7) chunk->table[0][i][j].item.id=IT_stone;
                        else 
                        {
                            chunk->table[0][i][j].item.id = IT_log;
                            chunk->table[0][i][j].item.count = 1;
                        }
                    }
                    break;
                case 2:
                    random = rand() % 100;
                    //if (random < 10 && !(chunk_contains_dung_entrance))
                    //{
                        chunk->table[0][i][j].tile = TILE_DUNG_DOOR;
                        chunk_contains_dung_entrance = 1;
                        generate_dungeon(chunk, j, i);
                    //}
                    //else 
                    //    chunk->table[0][i][j].tile = TILE_TREE;
                    break;
                case 3:
                    chunk->table[0][i][j].tile = TILE_GRASS;
                    chunk->table[0][i][j].item.count = (it_randnum > 5) ? it_randnum/4 : 0;
                    if (chunk->table[0][i][j].item.count) {
                        if (rand() % 10 < 8) chunk->table[0][i][j].item.id=IT_stone;
                        else
                        { 
                            chunk->table[0][i][j].item.id=IT_log;
                            chunk->table[0][i][j].item.count=0;
                        }
                    }
                    break;
            }
        }
    }*/
}

void create_biome_desert(chunk * chunk)
{
    /*int it_randnum;
    int type_int = 0;
    for (int i=0; i<CHUNK_SIZE; i++)
    {
        for (int j=0; j<CHUNK_SIZE; j++)
        {
            it_randnum = rand() % 10;
            type_int = rand() % 2; 
            switch (type_int)
            {
                case 0:
                    chunk->table[0][i][j].tile = TILE_SAND;
                    chunk->table[0][i][j].item.count = (it_randnum > 5) ? it_randnum/4 : 0;
                    if (chunk->table[0][i][j].item.count) {
                        chunk->table[0][i][j].item.id=IT_stone;
                    }
                    break;
                case 1:
                    chunk->table[0][i][j].tile = TILE_SANDSTONE;
                    break;
            }
        }
    }*/
}

void create_biome_lake(chunk * chunk)
{
    /*int it_randnum;
    int type_int = 0;
    for (int i=0; i<CHUNK_SIZE; i++)
    {
        for (int j=0; j<CHUNK_SIZE; j++)
        {
            it_randnum = rand() % 10;
            type_int = rand() % 4;
            switch(type_int)
            {
                case 0:
                    chunk->table[0][i][j].tile = TILE_WATER;
                    break;
                case 1:
                    chunk->table[0][i][j].tile = TILE_GRASS;
                    chunk->table[0][i][j].item.count = (it_randnum > 5) ? it_randnum/4 : 0;
                    if (chunk->table[0][i][j].item.count) {
                        if (rand() % 10 < 8) chunk->table[0][i][j].item.id=IT_stone;
                        else 
                        {
                            chunk->table[0][i][j].item.id=IT_log;
                            chunk->table[0][i][j].item.count=1;
                        }
                    }
                break;
                case 2:
                    chunk->table[0][i][j].tile = TILE_SAND;
                    chunk->table[0][i][j].item.count = (it_randnum > 5) ? it_randnum/4 : 0;
                    if (chunk->table[0][i][j].item.count) {
                        chunk->table[0][i][j].item.id=IT_stone;
                    }
                    break;
                case 3:
                    chunk->table[0][i][j].tile = TILE_DIRT;
                    chunk->table[0][i][j].item.count = (it_randnum > 5) ? it_randnum/4 : 0;
                    if (chunk->table[0][i][j].item.count) {
                        if (rand() % 10 < 7) chunk->table[0][i][j].item.id=IT_stone;
                        else 
                        {
                            chunk->table[0][i][j].item.count=1;
                            chunk->table[0][i][j].item.id=IT_log;
                        }
                    }
              break;
            }
        }
    }*/
}

void create_biome_sweet_tree(chunk * chunk)
{
    /*int it_randnum;
    int type_int = 0;
    for (int i=0; i<CHUNK_SIZE; i++)
    {
        for (int j=0; j<CHUNK_SIZE; j++)
        {
            it_randnum = rand() % 10;
            type_int = rand() % 1;
            switch(type_int)
            {
                case 0:
                    chunk->table[0][i][j].tile = TILE_SWEET_GRASS;
                    chunk->table[0][i][j].item.count = (it_randnum > 5) ? it_randnum/4 : 0;
                    if (chunk->table[0][i][j].item.count) {
                        if (rand() % 10 < 8) chunk->table[0][i][j].item.id=IT_stone;
                        else 
                        {
                            chunk->table[0][i][j].item.id=IT_log;
                            chunk->table[0][i][j].item.count=1;
                        }
                    }
                     break;*/
                /*case 1:
                    chunk->table[0][i][j].tile = TILE_SWEET_TREE;
                    break;
                case 2:
                    chunk->table[0][i][j].tile = TILE_SWEET_BUSH;
                    break;
                case 3:
                    chunk->table[0][i][j].tile = TILE_SWEET_FLOWER;
                    break;*/
            /*}
        }
    }*/
}

void generate_chunk(chunk *chunk, int chunk_x, int chunk_y)  
{
    for (int z = 0; z < CHUNK_SIZE; z++) 
    {
        for (int x = 0; x < CHUNK_SIZE; x++) 
        {
            int height = height_at(chunk_x, chunk_y, x, z);
            //printf("%3d ", height);
            for (int y = 0; y < CHUNK_SIZE; y++)
            {
                chunk->table[z][y][x].tile = (y < height-1) ? TILE_DIRT : (y == height-1 ? TILE_GRASS : TILE_AIR);
                chunk->table[z][y][x].weight = 4 + rand() % 9;
            }
        }
        //printf("\n");
    }

    for (int i = 0; i < 128; i++)
    {
        struct object *o = (struct object *)malloc(sizeof(struct object));
        o->type = OBJECT_NULL;
        o->x = rand() % 16;
        o->z = rand() % 16;
        o->y = height_at(chunk_x, chunk_y, o->x, o->z);

        chunk->objects[i] = o;
    }
    for (int i = 0; i < 19; i++)
    {
        struct object *o = (struct object *)malloc(sizeof(struct object));
        o->type = OBJECT_TREE;
        o->base_element_id = rand() % BASE_ELEMENTS;
        o->x = rand() % 16;
        o->z = rand() % 16;
        o->y = height_at(chunk_x, chunk_y, o->x, o->z);

        chunk->objects[i] = o;
    }
    for (int i = 0; i < 4; i++)
    {
        int b = rand() % BASE_ELEMENTS;
        Element *o = new Element(base_elements[b]);
        int x = rand() % 16;
        int z = rand() % 16;

        o->set_posittion(x, 0, x);
        o->set_posittion(x, height_at(chunk_x, chunk_y, x, z), z);

        chunk->items[i] = o;
    }

    /*enum biomes random_biome = (enum biomes) (rand() % 4);
    chunk->biome = random_biome;

    switch (random_biome)
    {
        case BIOME_FOREST: create_biome_forest(chunk); break;
        case BIOME_DESERT: create_biome_desert(chunk); break;
        case BIOME_SWEET_TREE: create_biome_sweet_tree(chunk); break;
        case BIOME_LAKE: create_biome_lake(chunk); break;
    }*/
}

char load_chunk(int x, int y)
{
    if (x >= 0 && x < WORLD_SIZE && y >= 0 && y < WORLD_SIZE) 
    {
        if (world_table[y][x] == NULL) 
        {
            chunk* c = (chunk*) calloc(1, sizeof(chunk));
//            printf("load %d %d\n", x, y);
            generate_chunk(c, x, y);
            world_table[y][x] = c;
            notify_load_chunk(x, y);
        }
        return 1;
    }
    return 0;
}

char traversable_tiles[TILE_MAX_NUM] =
{   
    1, //TILE_STONE,
    1, //TILE_DIRT,
//    1, //TILE_TREE,
    1,//0 //TILE_DUNG_WALL,
    1, //TILE_DUNG_FLOOR,
    1, //TILE_DUNG_DOOR,
    1, //TILE_SAND,
    1, //TILE_SANDSTONE,
//    1, //TILE_SWEET_TREE,
//    1, //TILE_SWEET_BUSH,
//    1, //TILE_SWEET_FLOWER,
    1, //TILE_GRASS,
    1, //TILE_SWEET_GRASS,
    1,//0, //TILE_WATER,
    1, //TILE_CAVE_DOOR,
    1, //TILE_CAVE_FLOOR,
    1,//0, //TILE_CAVE_WALL,
};

InventoryElement **get_item_at(int chunk_x, int chunk_y, int x, int y, int z)
{
    for (int i = 0; i < 128; i++)
    {
        InventoryElement *el = world_table[chunk_y][chunk_x]->items[i];
        if (el)
        {
            int el_x, el_y, el_z;
            el->get_posittion(&el_x, &el_y, &el_z);

            if (el_x == x && el_y == y && el_z == z )
            {
                return &world_table[chunk_y][chunk_x]->items[i];
            }
        }
    }
    return NULL;
}

struct object ** get_object_at(int chunk_x, int chunk_y, int x, int y, int z)
{
    for (int i = 0; i < 128; i++)
    {
        struct object *ob = world_table[chunk_y][chunk_x]->objects[i];
        if (ob)
        {
            if (ob->x == x && ob->y == y && ob->z == z)
            {
                return &world_table[chunk_y][chunk_x]->objects[i];
            }
        }

    }
    return NULL;
}

struct object ** get_object_at_ppos(Player * player)
{
    return get_object_at(player->map_x, player->map_y, player->x, player->y, player->z);
}

InventoryElement **get_item_at_ppos(Player * player)
{
    return get_item_at(player->map_x, player->map_y, player->x, player->y, player->z);
}

void set_item_at(InventoryElement *item, int chunk_x, int chunk_y, int x, int y, int z) 
{
    for (int i = 0; i < 128; i++)
    {
        if (!world_table[chunk_y][chunk_x]->items[i])
        {
            world_table[chunk_y][chunk_x]->items[i] = item;
            break;
        }
    }
}

void set_item_at_ppos(InventoryElement *item, Player *player) 
{
    set_item_at(item, player->map_x, player->map_y, player->x, player->y, player->z);
}


enum game_tiles get_tile_at(int chunk_x, int chunk_y, int x, int y, int z)
{
    return world_table[chunk_y][chunk_x]->table[z][y][x].tile;
}

enum game_tiles get_tile_at_ppos(Player *player)
{
    return get_tile_at(player->map_x, player->map_y, player->x, player->y, player->z);
}


