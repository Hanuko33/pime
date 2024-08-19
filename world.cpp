#include "alchemist/elements.h"
#include "player.h"
#include "world.h"
//#include "cave.h"
//#include "dungeon.h"
#include "tiles.h"
#include "time.h"
#include "notifier.h"
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct OpenSimplex2F_context *simplex_context;

chunk * world_table[WORLD_SIZE][WORLD_SIZE];

void generator()
{
    load_chunk(WORLD_CENTER, WORLD_CENTER);
}

void create_biome_plains(chunk * chunk)
{
    for (int y = 0; y < CHUNK_SIZE; y++) 
    {
        for (int x = 0; x < CHUNK_SIZE; x++) 
        {
            chunk->table[y][x].tile = TILE_GRASS;
        }
        //printf("\n");
    }
    if (rand() % 2)
    {
        for (int i = 0; i < 16; i++)
        {
            int b = rand() % BASE_ELEMENTS;
            Element *o = new Element(base_elements[b]);
            int x = rand() % CHUNK_SIZE;
            int y = rand() % CHUNK_SIZE;

            o->set_posittion(x, y);

            chunk->items[i] = o;
        }
    }
    if (rand() % 2)
    {
        for (int i = 0; i < 16; i++)
        {
            Plant *p = new Plant();
            int x = rand() % CHUNK_SIZE;
            int y = rand() % CHUNK_SIZE;

            p->type = PLANTID_strawberry;

            p->set_posittion(x, y);

            chunk->plants[i] = p;
        }
    }
    if (rand() % 2)
    {
        for (int i = 0; i < 16; i++)
        {
            Plant *p = new Plant();
            int x = rand() % CHUNK_SIZE;
            int y = rand() % CHUNK_SIZE;

            p->type = PLANTID_watermelon;

            p->set_posittion(x, y);

            chunk->plants[i] = p;
        }
    }
    if (rand() % 2)
    {
        for (int i = 0; i < 16; i++)
        {
            Plant *p = new Plant();
            int x = rand() % CHUNK_SIZE;
            int y = rand() % CHUNK_SIZE;

            p->type = PLANTID_pumpkin;

            p->set_posittion(x, y);

            chunk->plants[i] = p;
        }
    }
    if (rand() % 2)
    {
        for (int i = 0; i < 2; i++)
        {
            Animal *a = new Animal();
            int x = rand() % CHUNK_SIZE;
            int y = rand() % CHUNK_SIZE;

            a->type = ANIMALID_pig;

            a->set_posittion(x, y);

            chunk->animals[i]=a;
        }
    }
}

void create_biome_forest(chunk * chunk)
{
    for (int y = 0; y < CHUNK_SIZE; y++) 
    {
        for (int x = 0; x < CHUNK_SIZE; x++) 
        {
            chunk->table[y][x].tile = TILE_GRASS;
        }
        //printf("\n");
    }
    for (int i = 0; i < 8; i++)
    {
        int b = rand() % BASE_ELEMENTS;
        Element *o = new Element(base_elements[b]);
        int x = rand() % CHUNK_SIZE;
        int y = rand() % CHUNK_SIZE;

        o->set_posittion(x, y);

        chunk->items[i] = o;
    }


    for (int i = 0; i < 32; i++)
    {
        Plant * tree = new Plant();

        int x = rand() % CHUNK_SIZE;
        int y = rand() % CHUNK_SIZE;

        for (int i = 0; i < 256; i++) {
            if (chunk->plants[i])
            {
                int ox=0;
                int oy=0;

                chunk->plants[i]->get_posittion(&ox, &oy);

                if (ox == x && oy == y)
                {
                    x = rand() % CHUNK_SIZE;
                    y = rand() % CHUNK_SIZE;
                }
            }
        
        }

        tree->set_posittion(x, y);
        switch (rand() % 3) {
            case 0:
                tree->type=PLANTID_tree;
                break;
            case 1:
                tree->type=PLANTID_tree1;
                break;
            case 2:
                tree->type=PLANTID_tree2;
                break;
        
        }

        chunk->plants[i]=tree;
    }
}

void create_biome_desert(chunk * chunk)
{
    for (int y = 0; y < CHUNK_SIZE; y++) 
    {
        for (int x = 0; x < CHUNK_SIZE; x++) 
        {
            chunk->table[y][x].tile = TILE_SAND;
        }
        //printf("\n");
    }

    for (int i = 0; i < 4; i++)
    {
        int b = rand() % BASE_ELEMENTS;
        Element *o = new Element(base_elements[b]);
        int x = rand() % CHUNK_SIZE;
        int y = rand() % CHUNK_SIZE;

        o->set_posittion(x, y);

        chunk->items[i] = o;
    }


}

void generate_chunk(chunk *chunk, int chunk_x, int chunk_y)  
{
    enum biomes random_biome = (enum biomes) (rand() % BIOMES);
    chunk->biome = random_biome;

    switch (random_biome)
    {
        case BIOME_FOREST: create_biome_forest(chunk); break;
        case BIOME_DESERT: create_biome_desert(chunk); break;
        case BIOME_PLAINS: create_biome_plains(chunk); break;
    }
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

Being **get_being_at(int chunk_x, int chunk_y, int x, int y)
{
    for (int i = 0; i < 128; i++)
    {
        Being *b = world_table[chunk_y][chunk_x]->beings[i];
        if (b)
        {
            int b_x, b_y;
            b->get_posittion(&b_x, &b_y);

            if (b_x == x && b_y == y)
            {
                return &world_table[chunk_y][chunk_x]->beings[i];
            }
        }
    }
    return NULL;
}

Being **get_being_at_ppos(Player * player)
{
    return get_being_at(player->map_x, player->map_y, player->x, player->y);
}

Animal **get_animal_at(int chunk_x, int chunk_y, int x, int y)
{
    for (int i = 0; i < 128; i++)
    {
        Animal *a = world_table[chunk_y][chunk_x]->animals[i];
        if (a)
        {
            int b_x, b_y;
            a->get_posittion(&b_x, &b_y);

            if (b_x == x && b_y == y)
            {
                return &world_table[chunk_y][chunk_x]->animals[i];
            }
        }
    }
    return NULL;
}
Animal **get_animal_at_ppos(Player * player)
{
    return get_animal_at(player->map_x, player->map_y, player->x, player->y);
}

Object **get_object_at(int chunk_x, int chunk_y, int x, int y)
{
    for (int i = 0; i < 128; i++)
    {
        Object * o = world_table[chunk_y][chunk_x]->objects[i];
        if (o)
        {
            int b_x, b_y;
            o->get_posittion(&b_x, &b_y);

            if (b_x == x && b_y == y)
            {
                return &world_table[chunk_y][chunk_x]->objects[i];
            }
        }
    }
    return NULL;
}

Object **get_object_at_ppos(Player * player)
{
    return get_object_at(player->map_x, player->map_y, player->x, player->y);
}


Plant **get_plant_at(int chunk_x, int chunk_y, int x, int y)
{
    for (int i = 0; i < 128; i++)
    {
        Plant *p = world_table[chunk_y][chunk_x]->plants[i];
        if (p)
        {
            int b_x, b_y;
            p->get_posittion(&b_x, &b_y);

            if (b_x == x && b_y == y)
            {
                return &world_table[chunk_y][chunk_x]->plants[i];
            }
        }
    }
    return NULL;
}

Plant **get_plant_at_ppos(Player * player)
{
    return get_plant_at(player->map_x, player->map_y, player->x, player->y);
}


InventoryElement **get_item_at(int chunk_x, int chunk_y, int x, int y)
{
    for (int i = 0; i < 128; i++)
    {
        InventoryElement *el = world_table[chunk_y][chunk_x]->items[i];
        if (el)
        {
            int el_x, el_y;
            el->get_posittion(&el_x, &el_y);

            if (el_x == x && el_y == y)
            {
                return &world_table[chunk_y][chunk_x]->items[i];
            }
        }
    }
    return NULL;
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


