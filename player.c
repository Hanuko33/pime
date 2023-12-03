#include "player.h"
#include "dungeon.h"
#include "tiles.h"
#include "time.h"
#include "world.h"

void player_move(struct Player *player, int dx, int dy)
{
    int new_x = player->x + dx;
    int new_y = player->y + dy;
    int new_map_x = player->map_x;
    int new_map_y = player->map_y;

    if (new_x < 0 || new_x >= CHUNK_SIZE || new_y < 0 || new_y >= CHUNK_SIZE)
    {
        if (load_chunk(player->map_x + dx, player->map_y + dy))
        {
            new_map_y += dy;
            new_map_x += dx;
            new_x += -CHUNK_SIZE * dx;
            new_y += -CHUNK_SIZE * dy;
        }
        else 
        {
            return;
        }
    }
    if (traversable_tiles[(*world_table[new_map_y][new_map_x])[player->z][new_y][new_x]]) {
        game_time.seconds += (player->running ? 15 : 30);
        player->map_x = new_map_x;
        player->map_y = new_map_y;
        player->x = new_x;
        player->y = new_y;
        player->energy -= (player->running ? 2 : 1);
    }


}


