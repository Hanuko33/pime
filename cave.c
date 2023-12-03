#include "cave.h"
#include "tiles.h"
#include <stdlib.h>

void generate_cave(chunk* chunk, int entrance_x, int entrance_y)
{
    int gen_point_x = entrance_x;
	int gen_point_y = entrance_y;

	for (int i=0; i<CHUNK_SIZE; i++)
	{
		for (int j=0; j<CHUNK_SIZE; j++)
		{
			(*chunk)[2][i][j]=TILE_CAVE_WALL;
		}
	}
	for (int i=0; i<(CHUNK_SIZE*(CHUNK_SIZE*3)); i++)
    {
		// 0 for x
		// 1 for y
		int y_x_axis = rand() % 2;
        if (!y_x_axis)
		{
			// 0 for minus
			// 1 for plus
			int plus_minus = rand() % 2;
			(*chunk)[2][gen_point_y][gen_point_x] = TILE_CAVE_FLOOR;
            if (!plus_minus)
			{
				if (gen_point_x > 1) gen_point_x--;
			} 
            else
			{
				if (gen_point_x < (CHUNK_SIZE-2)) gen_point_x++;	
			}
		}
        else
		{
			// 0 for minus
			// 1 for plus
			int plus_minus = rand() % 2;
			(*chunk)[2][gen_point_y][gen_point_x] = TILE_CAVE_FLOOR;
            if (!plus_minus)
			{
				if (gen_point_y > 1) gen_point_y--;
			} 
            else 
            {
				if (gen_point_y < (CHUNK_SIZE-2)) gen_point_y++;
			}	
		}
	}
    (*chunk)[2][entrance_y][entrance_x] = TILE_CAVE_DOOR;
}

