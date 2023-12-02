#include "dungeon.h"
#include "player.h"

tile_table dungeon_terrain_list;

void generate_dungeon()
{
    int gen_point_x = 1 + rand() % (DUNGEON_SIZE-3) + 1;
	int gen_point_y = 1 + rand() % (DUNGEON_SIZE-3) + 1;

	for (int i=0; i<DUNGEON_SIZE; i++)
	{
		for (int j=0; j<DUNGEON_SIZE; j++)
		{
			dungeon_terrain_list[i][j]=TILE_DUNG_WALL;
		}
	}

	for (int i=0; i<(DUNGEON_SIZE*(DUNGEON_SIZE/3)); i++)
    {
		int move = rand() % 4;
		
            dungeon_terrain_list[gen_point_y][gen_point_x] = TILE_DUNG_FLOOR;
		
        switch(move)
        {
            case 0: if ((gen_point_x > 1) && (dungeon_terrain_list[gen_point_y][gen_point_x-1] != TILE_DUNG_FLOOR)) {
                        gen_point_x--; break;
                    }
            case 1: if ((gen_point_x < (DUNGEON_SIZE - 2)) && (dungeon_terrain_list[gen_point_y][gen_point_x+1] != TILE_DUNG_FLOOR)) {
                        gen_point_x++; break;
                    }
		    case 2: if ((gen_point_y > 1) && (dungeon_terrain_list[gen_point_y-1][gen_point_x] != TILE_DUNG_FLOOR)) {
                        gen_point_y--; break;
                    }

            case 3: if ((gen_point_y < (DUNGEON_SIZE - 2)) && (dungeon_terrain_list[gen_point_y+1][gen_point_x] != TILE_DUNG_FLOOR)) {
                        gen_point_y++; break;
                    }
		}
	}
    dungeon_terrain_list[gen_point_y][gen_point_x] = TILE_DUNG_DOOR;
    printf("dung door1: %d, %d\n", gen_point_x, gen_point_y);
}

void dungeon_generator()
{

    int temp_x=0;
    int temp_y=0;
    int stuck = 1;
			
    generate_dungeon();

    while(stuck)
    {
        if (temp_x < DUNGEON_SIZE-1)
        {
            temp_x++;
        }
        else
        {
            temp_x=1;
            if (temp_y < DUNGEON_SIZE-1) temp_y++;
        }
        if (dungeon_terrain_list[temp_y][temp_x] == TILE_DUNG_FLOOR || dungeon_terrain_list[temp_y][temp_x] == TILE_DUNG_DOOR) 
        {
            stuck = 0;
            dungeon_terrain_list[temp_y][temp_x] = TILE_DUNG_DOOR;
            printf("dung door2: %d, %d\n", temp_x, temp_y);
            player.x=temp_x;
            player.y=temp_y;
            break;
        }
    }
}
