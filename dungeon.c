#include "dungeon.h"

void dungeon_generator()
{
    int random_int = 0;
    int random_x = 0;
    int random_y = 0;
    for (int i=0; i<25; i++)
    {
        random_x = rand() % DUNGEON_SIZE+0;
        random_y = rand() % DUNGEON_SIZE+0;
        dungeon_terrain_list[random_x][random_y] = TILE_DUNG_FLOOR;
    }
    for (int j=0; j<DUNGEON_SIZE; j++)
    {
        for (int ij=0; ij<DUNGEON_SIZE; ij++)
        {
            random_int = rand() % 2;
            if (random_int == 1 || 
					(( dungeon_terrain_list[j-1][ij] == TILE_DUNG_WALL && dungeon_terrain_list[j-2][ij] == TILE_DUNG_WALL ) 
					 && ( dungeon_terrain_list[j][ij-1] == TILE_DUNG_WALL && dungeon_terrain_list[j][ij-2] == TILE_DUNG_WALL ) 
					 && (dungeon_terrain_list[j-1][j-1] == TILE_DUNG_WALL && dungeon_terrain_list[j-2][j-2] == TILE_DUNG_WALL)
					))
            {
                dungeon_terrain_list[j][ij] = TILE_DUNG_FLOOR;
            }
            else
            {
               dungeon_terrain_list[j][ij] = TILE_DUNG_WALL;
            }
        }
    }
    int random_int_max = DUNGEON_SIZE - 3;
    random_x = rand() % random_int_max+4;
    random_x = rand() % random_int_max+4;
    dungeon_terrain_list[random_x-2][random_y-2] = TILE_DUNG_EXIT;

    dungeon_terrain_list[random_x-1][random_y-1] = TILE_DUNG_FLOOR;
    dungeon_terrain_list[random_x-1][random_y-2] = TILE_DUNG_FLOOR;
    dungeon_terrain_list[random_x-1][random_y-3] = TILE_DUNG_FLOOR;
    
    dungeon_terrain_list[random_x-2][random_y-1] = TILE_DUNG_FLOOR;
    dungeon_terrain_list[random_x-2][random_y-3] = TILE_DUNG_FLOOR;

    dungeon_terrain_list[random_x-3][random_y-1] = TILE_DUNG_FLOOR;
    dungeon_terrain_list[random_x-3][random_y-2] = TILE_DUNG_FLOOR;
    dungeon_terrain_list[random_x-3][random_y-3] = TILE_DUNG_FLOOR;
}
