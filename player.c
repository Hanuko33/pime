#include "player.h"
#include "dungeon.h"
#include "tiles.h"
#include "time.h"
#include "world.h"

struct Player player;

void check_and_move(struct Player * p, int new_map_x, int new_map_y, int new_x, int new_y)
{
     if (traversable_tiles[world_table[new_map_y][new_map_x]->table[p->z][new_y][new_x].tile]) {
        game_time.seconds += (p->running ? 15 : 30);
        p->map_x = new_map_x;
        p->map_y = new_map_y;
        p->x = new_x;
        p->y = new_y;
        p->energy -= (p->running ? 2 : 1);
    }
}

void move_player(struct Player *p, int dx, int dy)
{
    int new_x = p->x + dx;
    int new_y = p->y + dy;
    int new_map_x = p->map_x;
    int new_map_y = p->map_y;
    
    if (! ((new_x >= 0 && new_x < CHUNK_SIZE) && (new_y >= 0 && new_y < CHUNK_SIZE))) 
    {
        if (new_x < 0 || new_x >= CHUNK_SIZE) {
                new_map_x += dx;
                new_x += -CHUNK_SIZE * dx;
            if (!load_chunk(new_map_x, new_map_y)) return;
        }
    
        if (new_y < 0 || new_y >= CHUNK_SIZE)
        {
                new_map_y += dy;
                new_y += -CHUNK_SIZE * dy;
            if (!load_chunk(new_map_x, new_map_y)) return;
        }
    }
    check_and_move(p, new_map_x, new_map_y, new_x, new_y);
}

void init_player()
{
	player.energy=250;
	player.back_x=0;
	player.back_y=0;
    player.map_x = WORLD_CENTER;
    player.map_y = WORLD_CENTER;
    for (int i=0; i < IT_MAX; i++)
        player.inventory[i]=rand() % 10;
}
