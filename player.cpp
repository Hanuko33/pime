#include "player.h"
#include "tiles.h"
#include "time.h"
#include "world.h"
#include <stdlib.h>

void Player::check_and_move(int new_map_x, int new_map_y, int new_x, int new_z)
{
    char moved = 0;
    if (world_table[new_map_y][new_map_x]->table[new_z][y-1][new_x].tile == TILE_AIR) 
    {
        y--;
        moved = 1;
    } 
    else if (world_table[new_map_y][new_map_x]->table[new_z][y][new_x].tile == TILE_AIR)
    {
        moved = 1;
    }
    else if (world_table[new_map_y][new_map_x]->table[new_z][y+1][new_x].tile == TILE_AIR)
    {
        y++;
        moved = 1;
    }
    
    if (moved)
    { 
        //game_time.seconds += (running ? 15 : sneaking ? 45 : 30);
        map_x = new_map_x;
        map_y = new_map_y;
        x = new_x;
        z = new_z;
        energy -= (running ? 2 : 1);
    }
}

void Player::move(int dx, int dz)
{
    int new_x = x + dx;
    int new_z = z + dz;
    int new_map_x = map_x;
    int new_map_y = map_y;
    
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
    check_and_move(new_map_x, new_map_y, new_x, new_z);
}

Player::Player()
{
	energy=250;
	back_x=0;
	back_y=0;
    health=1000;
    hunger=250;
    thirst=50;
    map_x = WORLD_CENTER;
    map_y = WORLD_CENTER;
    inventory = new InvList("inventory");

	y = 0;

	for (int i=0; i < 10; i++)
    {
        hotbar[i]=NULL;
        craftbar[i]=0;
    }
}
