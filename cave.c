#include "cave.h"
#include "player.h"

tile_table cave_terrain_list;

void cave_generator(int start_x, int start_y)
{
	int gen_point_x=start_x;
	int gen_point_y=start_y;
	for (int i=0; i<DUNGEON_SIZE; i++)
	{
		for (int j=0; j<DUNGEON_SIZE; j++)
		{
			cave_terrain_list[i][j]=TILE_CAVE_WALL;
		}
	}
	for (int i=0; i<(DUNGEON_SIZE*(DUNGEON_SIZE*3)); i++)
    {
		// 0 for x
		// 1 for y
		int y_x_axis = rand() % 2;
		if (y_x_axis == 0)
		{
			// 0 for minus
			// 1 for plus
			int plus_minus = rand() %2;
			cave_terrain_list[gen_point_x][gen_point_y] = TILE_CAVE_FLOOR;
			if (plus_minus == 0)
			{
				if (gen_point_x > 1) gen_point_x--;
			}
			if (plus_minus == 1)
			{
				if (gen_point_x < (DUNGEON_SIZE-1)) gen_point_x++;	
			}
		}
		if (y_x_axis == 1)
		{
			// 0 for minus
			// 1 for plus
			int plus_minus = rand() %2;
			cave_terrain_list[gen_point_x][gen_point_y] = TILE_CAVE_FLOOR;
			if (plus_minus == 0)
			{
				if (gen_point_y > 1) gen_point_y--;
			}
			if (plus_minus == 1)
			{
				if (gen_point_y < (DUNGEON_SIZE-1)) gen_point_y++;
			}	
		}
	}
    int temp_x=2;
    int temp_y=2;
    
    int stuck = 1;
    while(stuck)
    {
        if (temp_x<DUNGEON_SIZE-2)
        {
            temp_x++;
        }
        else
        {
            temp_x=2;
            if (temp_y<DUNGEON_SIZE-2) temp_y++;
        }
        if (cave_terrain_list[temp_x][temp_y] == TILE_CAVE_FLOOR || cave_terrain_list[temp_x][temp_y] == TILE_CAVE_DOOR) 
        {
            stuck = 0;
            cave_terrain_list[temp_x][temp_y] = TILE_CAVE_DOOR;
            break;
        }
    }
}