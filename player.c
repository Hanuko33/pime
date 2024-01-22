#include "player.h"
#include "tiles.h"
#include "time.h"
#include "world.h"

void check_and_move(struct Player * p, int new_map_x, int new_map_y, int new_x, int new_z)
{
    char moved = 0;
    if (world_table[new_map_y][new_map_x]->table[new_z][p->y-1][new_x].tile == TILE_AIR) 
    {
        p->y--;
        moved = 1;
    } 
    else if (world_table[new_map_y][new_map_x]->table[new_z][p->y][new_x].tile == TILE_AIR)
    {
        moved = 1;
    }
    else if (world_table[new_map_y][new_map_x]->table[new_z][p->y+1][new_x].tile == TILE_AIR)
    {
        p->y++;
        moved = 1;
    }
    
    if (moved)
    { 
        game_time.seconds += (p->running ? 15 : 30);
        p->map_x = new_map_x;
        p->map_y = new_map_y;
        p->x = new_x;
        p->z = new_z;
        p->energy -= (p->running ? 2 : 1);
    }
}

void move_player(struct Player *p, int dx, int dz)
{
    int new_x = p->x + dx;
    int new_z = p->z + dz;
    int new_map_x = p->map_x;
    int new_map_y = p->map_y;
    
    if (! ((new_x >= 0 && new_x < CHUNK_SIZE) && (new_z >= 0 && new_z < CHUNK_SIZE))) 
    {
        if (new_x < 0 || new_x >= CHUNK_SIZE) {
                new_map_x += dx;
                new_x += -CHUNK_SIZE * dx;
            if (!load_chunk(new_map_x, new_map_y)) return;
        }
    
        if (new_z < 0 || new_z >= CHUNK_SIZE)
        {
                new_map_y += dz;
                new_z += -CHUNK_SIZE * dz;
            if (!load_chunk(new_map_x, new_map_y)) return;
        }
    }
    check_and_move(p, new_map_x, new_map_y, new_x, new_z);
}

void init_player(struct Player* player)
{
	player->energy=250;
	player->back_x=0;
	player->back_y=0;
    player->map_x = WORLD_CENTER;
    player->map_y = WORLD_CENTER;
    //for (int i=0; i < IT_MAX; i++)
    //	player->inventory[i]=rand() % 10;
    player->inventory[IT_stone] = 10;

    player->y = height_at(WORLD_CENTER, WORLD_CENTER, 0, 0);

	for (int i=0; i < 10; i++)
        player->hotbar[i]=-1;

}
