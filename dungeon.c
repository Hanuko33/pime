#include "dungeon.h"
#include <stdlib.h>

void generate_dungeon(chunk* chunk, int door_x, int door_y)
{
    int gen_point_x = door_x;
	int gen_point_y = door_y;

	for (int i=0; i<CHUNK_SIZE; i++)
	{
		for (int j=0; j<CHUNK_SIZE; j++)
		{
			chunk->table[1][i][j]=TILE_DUNG_WALL;
		}
	}

	for (int i=0; i<(CHUNK_SIZE*(CHUNK_SIZE/3)); i++)
    {
		int move = rand() % 4;
		
            chunk->table[1][gen_point_y][gen_point_x] = TILE_DUNG_FLOOR;
		
        switch(move)
        {
            case 0: if ((gen_point_x > 1) && (chunk->table[1][gen_point_y][gen_point_x-1] != TILE_DUNG_FLOOR)) {
                        gen_point_x--; break;
                    }
            case 1: if ((gen_point_x < (CHUNK_SIZE - 2)) && (chunk->table[1][gen_point_y][gen_point_x+1] != TILE_DUNG_FLOOR)) {
                        gen_point_x++; break;
                    }
		    case 2: if ((gen_point_y > 1) && (chunk->table[1][gen_point_y-1][gen_point_x] != TILE_DUNG_FLOOR)) {
                        gen_point_y--; break;
                    }

            case 3: if ((gen_point_y < (CHUNK_SIZE - 2)) && (chunk->table[1][gen_point_y+1][gen_point_x] != TILE_DUNG_FLOOR)) {
                        gen_point_y++; break;
                    }
		}
	}
    chunk->table[1][door_y][door_x] = TILE_DUNG_DOOR;
    //printf("dung door1: %d, %d\n", gen_point_x, gen_point_y);
}

