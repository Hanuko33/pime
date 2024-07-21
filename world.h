#ifndef WORLD_H
#define WORLD_H

//#include "dungeon.h"
#include "time.h"
#include "tiles.h"

#include <stdlib.h>

extern char traversable_tiles[TILE_MAX_NUM];

extern chunk * world_table[WORLD_SIZE][WORLD_SIZE];

enum game_tiles get_tile_at(int chunk_x, int chunk_y, int x, int y);
enum game_tiles get_tile_at_ppos(Player *player);


Being ** get_being_at_ppos(Player *player);
Being ** get_being_at(int chunk_x, int chunk_y, int x, int y);

InventoryElement ** get_item_at_ppos(Player *player);
InventoryElement ** get_item_at(int chunk_x, int chunk_y, int x, int y);

void set_item_at(InventoryElement * item, int chunk_x, int chunk_y, int x, int y);
void set_item_at_ppos(InventoryElement * item, Player* player);

void generator();
void generate_chunk(chunk* chunk, int chunk_x, int chunk_y);
char load_chunk(int x, int y);
int height_at(int chunk_x, int chunk_y, int x, int y);


#endif
