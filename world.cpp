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
#include "texture.h"

struct OpenSimplex2F_context *simplex_context;

chunk * world_table[WORLD_SIZE][WORLD_SIZE];

void generator()
{
    load_chunk(WORLD_CENTER, WORLD_CENTER);
}

void create_biome_forest(chunk * chunk)
{
}

void create_biome_desert(chunk * chunk)
{
}

void create_biome_lake(chunk * chunk)
{
}

void create_biome_sweet_tree(chunk * chunk)
{
}

void generate_chunk(chunk *chunk, int chunk_x, int chunk_y)  
{
    chunk->biome = BIOME_FOREST;
    for (int y = 0; y < CHUNK_SIZE; y++) 
    {
        for (int x = 0; x < CHUNK_SIZE; x++) 
        {
            chunk->table[y][x].tile = TILE_GRASS;
            chunk->table[y][x].weight = 4 + rand() % 9;
        }
        //printf("\n");
    }
    for (int i = 0; i < CHUNK_SIZE; i++)
    {
        for (int j=0; j < CHUNK_SIZE; j++)
        {
            struct object o;
            o.base_element_id=0;
            o.type=OBJECT_NULL;
            chunk->objects[i][j] = o;

            if (rand() % 10 == 0)
            {
                chunk->objects[i][j].type = OBJECT_TREE;
                chunk->objects[i][j].base_element_id = rand() % BASE_ELEMENTS;
                while (base_elements[chunk->objects[i][j].base_element_id]->form != Form_solid)
                {
                    chunk->objects[i][j].base_element_id = rand() % BASE_ELEMENTS;
                }
                chunk->objects[i][j].texture = tree_textures[rand() % 3];
            }
        }
    }
    for (int i = 0; i < 4; i++)
    {
        int b = rand() % BASE_ELEMENTS;
        Element *o = new Element(base_elements[b]);
        int x = rand() % 16;
        int y = rand() % 16;

        o->set_posittion(x, y);

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
    1, //TILE_SAND,
    1, //TILE_SANDSTONE,
    1, //TILE_SWEET_TREE,
    1, //TILE_SWEET_BUSH,
    1, //TILE_SWEET_FLOWER,
    1, //TILE_GRASS,
    1, //TILE_SWEET_GRASS,
    1, //TILE_WATER,
};


InventoryElement **get_item_at(int chunk_x, int chunk_y, int x, int y)
{
    for (int i = 0; i < 128; i++)
    {
        InventoryElement *el = world_table[chunk_y][chunk_x]->items[i];
        if (el)
        {
            int el_x, el_y, el_z;
            el->get_posittion(&el_x, &el_y);

            if (el_x == x && el_y == y)
            {
                return &world_table[chunk_y][chunk_x]->items[i];
            }
        }
    }
    return NULL;
}

struct object * get_object_at(int chunk_x, int chunk_y, int x, int y)
{
    return &world_table[chunk_y][chunk_x]->objects[x][y];
}

struct object * get_object_at_ppos(Player * player)
{
    return get_object_at(player->map_x, player->map_y, player->x, player->y);
}

InventoryElement **get_item_at_ppos(Player * player)
{
    return get_item_at(player->map_x, player->map_y, player->x, player->y);
}

void set_item_at(InventoryElement *item, int chunk_x, int chunk_y, int x, int y) 
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
    set_item_at(item, player->map_x, player->map_y, player->x, player->y);
}


enum game_tiles get_tile_at(int chunk_x, int chunk_y, int x, int y)
{
    return world_table[chunk_y][chunk_x]->table[y][x].tile;
}

enum game_tiles get_tile_at_ppos(Player *player)
{
    return get_tile_at(player->map_x, player->map_y, player->x, player->y);
}


