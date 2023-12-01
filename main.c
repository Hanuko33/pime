#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "window.h"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL2_framerate.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "dungeon.h"
#include "cave.h"
#include "texture.h"
#include "player.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "menu.h"
#include "time.h"


enum  biomes
{
    BIOME_DESERT,
    BIOME_FOREST,
    BIOME_SWEET_TREE,
    BIOME_LAKE
};


Game_time game_time;

tile_table terrain_list;
tile_table * screen_list = &terrain_list;

struct Player player;

void generator()
{
    int random = 0;
    int type_int = 0;
    char chunk_contains_dung_entrance = 0;
    char chunk_contains_cave_entrance = 0;
    
    enum biomes random_biome = rand() % 4;

    printf("running generator... %d\n", random_biome);
    
    for (int i=0; i<DUNGEON_SIZE; i++)
    {
        for (int j=0; j<DUNGEON_SIZE; j++)
        {
            switch (random_biome)
	    	{
				case BIOME_FOREST:
            		type_int = rand() % 4;
	    			switch (type_int)
            		{
                		case 0:
                            random = rand() % 75;
                            if (random < 10 && !(chunk_contains_cave_entrance))
                            {
                                terrain_list[i][j] = TILE_CAVE_DOOR;
                                chunk_contains_cave_entrance = 1;
                            }
                            else
                            {
                                terrain_list[i][j] = TILE_STONE;
                            }
                            break;
                		case 1:
                    		terrain_list[i][j] = TILE_DIRT;
                    		break;
                		case 2:
                    		random = rand() % 100;
                    		if (random < 10 && !(chunk_contains_dung_entrance))
                    		{
                        		terrain_list[i][j] = TILE_DUNG_DOOR;
                        		chunk_contains_dung_entrance = 1;
                    		}
                    		else terrain_list[i][j] = TILE_TREE;
                    		break;
                        case 3:
                            terrain_list[i][j] = TILE_GRASS;
                            break;
            		}
	    			break;
		    	
				case     BIOME_DESERT:
					type_int = rand() % 2; 
					switch (type_int)
					{
						case 0:
							terrain_list[i][j] = TILE_SAND;
		    				break;
						case 1:
		    				terrain_list[i][j] = TILE_SANDSTONE;
		    				break;
					}
                    break;
                case BIOME_SWEET_TREE:
                    type_int = rand() % 4;
                    switch(type_int)
                    {
                        case 0:
                            terrain_list[i][j] = TILE_SWEET_GRASS;
                            break;
                        case 1:
                            terrain_list[i][j] = TILE_SWEET_TREE;
                            break;
                        case 2:
                            terrain_list[i][j] = TILE_SWEET_BUSH;
                            break;
                        case 3:
                            terrain_list[i][j] = TILE_SWEET_FLOWER;
                            break;
                    }
                    break;

                case BIOME_LAKE:
                    type_int = rand() % 4;
                    switch(type_int)
                    {
                        case 0:
                            terrain_list[i][j] = TILE_WATER;
                            break;
                        case 1:
                            terrain_list[i][j] = TILE_GRASS;
                            break;
                        case 2:
                            terrain_list[i][j] = TILE_SAND;
                            break;
                        case 3:
                            terrain_list[i][j] = TILE_DIRT;
                            break;
                    }
                    break;
			}
		}
    }
}

void save(char with_player)
{
    if (with_player)
    {
        int temp;
        FILE *file;
        char to_write[60];
        
        file = fopen("world/player.txt", "w");
        sprintf(to_write, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n", player.map_y, player.map_x, player.y, player.x, (int)player.in_dungeon, player.energy, player.back_x, player.back_y, game_time.days, game_time.hours, game_time.minutes, game_time.seconds, player.in_cave);
        fwrite(to_write, sizeof(to_write), 1, file);
        fclose(file);
    }
    if (player.in_dungeon)
    {
        char filename[31];
        sprintf(filename, "world/%9d-%9ddung", player.map_x, player.map_y);
        FILE *chunk = fopen(filename, "w");
        fwrite(dungeon_terrain_list, sizeof(dungeon_terrain_list), 1, chunk);
        fclose(chunk);
    }
    else if (player.in_cave)
    {
        char filename[31];
        sprintf(filename, "world/%9d-%9dcave", player.map_x, player.map_y);
        FILE *chunk = fopen(filename, "w");
        fwrite(cave_terrain_list, sizeof(cave_terrain_list), 1, chunk);
        fclose(chunk);
    }
    else
    {
        char filename[26];
        sprintf(filename, "world/%9d-%9d", player.map_x, player.map_y);
        FILE *chunk = fopen(filename, "w");
        fwrite(terrain_list, sizeof(terrain_list), 1, chunk);
        fclose(chunk);
    }
}
void load(char with_player)
{
    if (with_player)
    {
        int temp;
        FILE *file;
        
        if (file = fopen("world/player.txt", "r"))
        {
            printf("loading player: ");
            if (fscanf(file, "%d%d%d%d%d%d%d%d%d%d%d%d%d", 
                   &player.map_y, &player.map_x, &player.y, &player.x, 
                   &temp, &player.energy, 
                   &player.back_x, &player.back_y, 
                   &game_time.days, &game_time.hours, &game_time.minutes, &game_time.seconds, &player.in_cave) == 13 )
            {
                player.in_dungeon=(char)(temp & 255);
                if (player.in_dungeon) 
                    screen_list=&dungeon_terrain_list;
                else if (player.in_cave)
                    screen_list=&cave_terrain_list;
                else 
                    screen_list = &terrain_list;
                printf(" done\n");
            }
            else printf("failed\n");
            fclose(file);
        }
    }
    if (player.in_dungeon)
    {
        char filename[30];
        sprintf(filename, "world/%9d-%9ddung", player.map_x, player.map_y);
        FILE* chunk;
 		
        player.back_x = player.x;
		player.back_y = player.y;       
        
        if (chunk = fopen(filename, "r"))
        {
            player.x=0;
            player.y=0;
            printf("loading: %s\n", filename);
            fread(dungeon_terrain_list, sizeof(dungeon_terrain_list), 1, chunk);
            fclose(chunk);
			int stuck = 1;
			while(stuck)
			{
				if (player.x<DUNGEON_SIZE-2)
				{
					player.x++;
				}
				else
				{
					player.x=1;
                    if (player.y < DUNGEON_SIZE-2) player.y++;
				}
                if (dungeon_terrain_list[player.x][player.y] == TILE_DUNG_FLOOR || dungeon_terrain_list[player.x][player.y] == TILE_DUNG_DOOR) 
                {
                    stuck = 0;
                    break;
                }
          	}
        }
        else
        {
            dungeon_generator();
		}
    }
    else if (player.in_cave)
    {
        char filename[30];
        sprintf(filename, "world/%9d-%9dcave", player.map_x, player.map_y);
        FILE* chunk;
 		
        player.back_x = player.x;
		player.back_y = player.y;       
        
        if (chunk = fopen(filename, "r"))
        {
            player.x=0;
            player.y=0;
            printf("loading: %s\n", filename);
            fread(cave_terrain_list, sizeof(cave_terrain_list), 1, chunk);
            fclose(chunk);
			int stuck = 1;
			while(stuck)
			{
				if (player.x<DUNGEON_SIZE-1)
				{
					player.x++;
				}
				else
				{
					player.x=0;
                    if (player.y<DUNGEON_SIZE-1) player.y++;
				}
                if (cave_terrain_list[player.x][player.y] == TILE_CAVE_FLOOR || cave_terrain_list[player.x][player.y] == TILE_CAVE_DOOR) 
                {
                    stuck = 0;
                    break;
                }
          	}
        }
        else
        {
            cave_generator();
            player.x=0;
            player.y=0;
			int stuck = 1;
			while(stuck)
			{
				if (player.x<DUNGEON_SIZE-1)
				{
					player.x++;
				}
				else
				{
					player.x=0;
                    if (player.y<DUNGEON_SIZE-1) player.y++;
				}
                if (cave_terrain_list[player.x][player.y] == TILE_CAVE_FLOOR || cave_terrain_list[player.x][player.y] == TILE_CAVE_DOOR) 
                {
                    stuck = 0;
                    break;
                }
          	}
		}
    }
   else
    {
        char filename[26];
        sprintf(filename, "world/%9d-%9d", player.map_x, player.map_y);
        FILE* chunk;
        if (chunk = fopen(filename, "r"))
        {
            printf("loading: %s\n", filename);
            fread(terrain_list, sizeof(terrain_list), 1, chunk);
            fclose(chunk);
        }
        else
        {
            generator();
        }
    }
}


void player_interact(int key )
{
    if ( menu_interact(key)) return;
    
    switch (key)
    {
        case SDLK_F1:
            screen_list=&terrain_list;
            generator();
            player.in_cave = 0;
            player.in_dungeon = 0;
        break;

        case SDLK_F2:
            screen_list=&dungeon_terrain_list;
            dungeon_generator();
            player.in_dungeon = 1;
            player.in_cave = 0;
        break;

        case SDLK_F3:
            screen_list=&cave_terrain_list;
            cave_generator();
            player.in_cave = 1;
            player.in_dungeon = 0;
        break;
                    
        case SDLK_DOWN:
        case SDLK_s:
        {
            if (player.in_dungeon)
            {
                if(player.y < DUNGEON_SIZE-1 && (dungeon_terrain_list[player.x][player.y+1] != TILE_DUNG_WALL))
                {
                    if (player.running)
                    {
                        game_time.seconds+=15;
                        player.y++;
                        player.energy-=2;
                    }
                    else
                    {
                        game_time.seconds+=30;
                        player.y++;
                        player.energy--;
                    }
                    break;
                }
                break;
            }
            if (player.in_cave)
            {
                if (player.y < DUNGEON_SIZE-1 && (cave_terrain_list[player.x][player.y+1] != TILE_CAVE_WALL))
                {
                    if (player.running)
                    {
                        player.y++;
                        player.energy-=2;
                        game_time.seconds+=15;
                    }
                    else
                    {
                        player.y++;
                        player.energy--;
                        game_time.seconds+=30;
                    }
                    break;
                }
                break;
            }
            
            if (terrain_list[player.x][player.y+1] != TILE_WATER)
            {
                if (player.running)
                {
                    player.energy-=2;
                    if (player.y < DUNGEON_SIZE-1) player.y++;
                    else {
                            player.y=0; 
                            save(0);
                            player.map_y++;
                            load(0);
                    }
                    game_time.seconds+=15;
                }
                else
                {
                    game_time.seconds+=30;
                    if (player.y < DUNGEON_SIZE-1) player.y++;
                    else {
                        player.y=0; 
                        save(0); 
                        player.map_y++; 
                        load(0);
                    }
                    player.energy--;
                }
            }
            break;
        }
        case SDLK_w:
        case SDLK_UP:
        {
            if (player.in_dungeon)
            {
                if (player.y > 0 && (dungeon_terrain_list[player.x][player.y-1] != TILE_DUNG_WALL))
                {
                    if (player.running)
                    {
                        player.y--;
                        player.energy-=2;
                        game_time.seconds+=15;
                    }
                    else
                    {
                        player.y--;
                        player.energy--;
                        game_time.seconds+=30;
                    }
                    break;
                }
                break;
            }
            if (player.in_cave)
            {
                if (player.y > 0 && (cave_terrain_list[player.x][player.y-1] != TILE_CAVE_WALL))
                {
                    if (player.running)
                    {
                        player.y--;
                        player.energy-=2;
                        game_time.seconds+=15;
                    }
                    else
                    {
                        player.y--;
                        player.energy--;
                        game_time.seconds+=30;
                    }
                    break;
                }
                break;
            }
            if (terrain_list[player.x][player.y-1] != TILE_WATER)
            // IN MAIN WORLD
            {
                if (player.running)
                {
                    if (player.y > 0) player.y--;
                    else {player.y=DUNGEON_SIZE-1; save(0); player.map_y--;load(0);}
                    player.energy-=2;
                    game_time.seconds+=15;
                }
                else
                {
                    if (player.y > 0) player.y--;
                    else {player.y=DUNGEON_SIZE-1; save(0); player.map_y--;load(0);}
                    player.energy--;
                    game_time.seconds+=30;
                }
            }
            break;
        }
        case SDLK_RIGHT:
        case SDLK_d:
        {
            if (player.in_dungeon)
            {
                if (player.x < DUNGEON_SIZE-1 && (dungeon_terrain_list[player.x+1][player.y] != TILE_DUNG_WALL)) 
                {
                    if (player.running)
                    {
                        player.energy-=2;
                        player.x++;
                        game_time.seconds+=15;
                    }
                    else
                    {
                        player.energy--;
                        player.x++;
                        game_time.seconds+=30;
                    }
                }
            }
            else if (player.in_cave)
            {
                if (player.x < DUNGEON_SIZE-1 && (cave_terrain_list[player.x+1][player.y] != TILE_CAVE_WALL))
                {
                    if (player.running)
                    {
                        player.energy-=2;
                        player.x++;
                        game_time.seconds+=15;
                    }
                    else
                    {
                        player.energy--;
                        player.x++;
                        game_time.seconds+=30;
                    }
                }
            }
            else
            {
                if (terrain_list[player.x+1][player.y] != TILE_WATER)
                {
                    if (player.running)
                    {
                        player.energy-=2;
                        game_time.seconds+=15;
                        if (player.x < DUNGEON_SIZE-1) player.x++;
                        else if (!player.in_dungeon) {player.x=0; save(0); player.map_x++;load(0);}
                    }
                    else
                    {
                        player.energy--;
                        game_time.seconds+=30;
                        if (player.x < DUNGEON_SIZE-1) player.x++;
                        else if (!player.in_dungeon) {player.x=0; save(0); player.map_x++;load(0);}
                    }
                }
            }
            player.going_right=1;
            break;
        }
        case SDLK_LEFT:
        case SDLK_a:
        {
            if (player.in_dungeon)
            {
                if (player.x > 0 && (dungeon_terrain_list[player.x-1][player.y] != TILE_DUNG_WALL)) 
                {
                    if (player.running)
                    {
                        player.energy-=2;
                        player.x--;
                        game_time.seconds+=15;
                    }
                    else
                    {
                        player.energy--;
                        player.x--;
                        game_time.seconds+=30;
                    }
                }
            }
            else if (player.in_cave)
            {
                if (player.x > 0 && (cave_terrain_list[player.x-1][player.y] != TILE_CAVE_WALL))
                {
                    if (player.running)
                    {
                        player.energy-=2;
                        player.x--;
                        game_time.seconds+=15;
                    }
                    else
                    {
                        player.energy--;
                        player.x--;
                        game_time.seconds+=30;
                    }
                }
            }
            else
            {
                if ((terrain_list[player.x-1][player.y] != TILE_WATER))
                {
                if (player.running)
                {
                    player.energy-=2;
                    game_time.seconds+=15;
                    if (player.x > 0) player.x--;
                    else if (!player.in_dungeon) {player.x=DUNGEON_SIZE-1; save(0); player.map_x--;load(0);}
                }
                else
                {
                    player.energy--;
                    game_time.seconds+=30;
                    if (player.x > 0) player.x--;
                    else if (!player.in_dungeon) {player.x=DUNGEON_SIZE-1; save(0); player.map_x--;load(0);}
                }
                }
            }
            player.going_right = 0;
            break;
        }
        case SDLK_r:
        {  
            player.running ^= 1;
            break;
        }
        case SDLK_RETURN:
        case SDLK_e:
        {
            if ((*screen_list)[player.x][player.y] == TILE_DUNG_DOOR)
            {
                save(0);
                game_time.minutes++;
                if (player.in_dungeon == 1)
                {
                    player.in_dungeon = 0;
                    screen_list=&terrain_list;
				    player.x = player.back_x;
				    player.y = player.back_y;
                }
                else
                {
                    player.in_dungeon = 1;
                    screen_list=&dungeon_terrain_list;
                }
                load(0);
                save(0);
            }
            if ((*screen_list)[player.x][player.y] == TILE_CAVE_DOOR)
            {
                save(0);
                game_time.minutes++;
                if (player.in_cave == 1)
                {
                    player.in_cave = 0;
                    screen_list=&terrain_list;
                    player.x = player.back_x;
                    player.y = player.back_y;
                }
                else
                {
                    player.in_cave = 1;
                    screen_list=&cave_terrain_list;
                }
                load(0);
                save(0);
            }
            break;
        }
    }
}

void draw()
{
    int window_width;
    int window_height;
    int game_size;
    int tile_dungeon_size;

    SDL_GetWindowSize(main_window, &window_width, &window_height); 
    if (window_width < window_height)
    {
        game_size = window_width;
        tile_dungeon_size = window_width/(DUNGEON_SIZE);
    } 
	else
    {
        game_size = window_height;
        tile_dungeon_size = window_height/(DUNGEON_SIZE);
    }
    for (int i=0; i < DUNGEON_SIZE ; i++)
    {
        int x = i * tile_dungeon_size;
        for (int j=0; j < DUNGEON_SIZE; j++)
        {
            SDL_Texture *texture;
            SDL_Rect img_rect = {x, j * tile_dungeon_size, tile_dungeon_size, tile_dungeon_size};
            switch ((*screen_list)[i][j])
            {
                case TILE_CAVE_WALL:
                    texture = Texture.cave_wall;
                    break;
                case TILE_CAVE_FLOOR:
                    texture = Texture.cave_floor;
                    break;
                case TILE_CAVE_DOOR:
                    texture = Texture.cave_door;
                    break;
                case TILE_WATER:
                    texture = Texture.water;
                    break;
                case TILE_SWEET_GRASS:
                    texture = Texture.sweet_grass;
                    break;
                case TILE_GRASS:
                    texture = Texture.grass;
                    break;
                case TILE_SWEET_BUSH:
                    texture = Texture.sweet_bush;
                    break;
                case TILE_SWEET_TREE:
                    texture = Texture.sweet_tree;
                    break;
                case TILE_SWEET_FLOWER:
                    texture = Texture.sweet_flower;
				    break;
                case TILE_SANDSTONE:
					texture = Texture.sandstone;
					break;
                case TILE_STONE:    
                    texture = Texture.stone;
                    break;
                case TILE_DIRT:
                    texture = Texture.dirt;
                    break;
                case TILE_TREE:    
                    texture = Texture.tree;
                    break;
                case TILE_DUNG_DOOR:
                    texture = Texture.dung_door;
                    break;
                case TILE_DUNG_WALL:
                    texture = Texture.dung_wall;
                    break;
                case TILE_DUNG_FLOOR:
                    texture = Texture.dung_floor;
				    break;
				case TILE_SAND:
				    texture = Texture.sand;
				    break;
            }
            SDL_RenderCopy(renderer, texture, NULL, &img_rect);
        }
    }  
    SDL_Rect img_rect = {player.x * tile_dungeon_size, player.y * tile_dungeon_size, tile_dungeon_size, tile_dungeon_size};
    if (player.going_right) SDL_RenderCopy(renderer, Texture.playerr, NULL, &img_rect);
    else SDL_RenderCopy(renderer, Texture.playerl, NULL, &img_rect);
    
    int icon_size = game_size/10;
    if (player.running)
    {
        SDL_Rect running_icon_rect = {(game_size-(icon_size*2)), 0, icon_size, icon_size};
        SDL_RenderCopy(renderer, Texture.run_icon, NULL, &running_icon_rect);
    }

    if (player.energy > 1000) player.energy = 1000;   

    char text_energy[20];
    char text_y[20];
    char text_x[20];
    char text_time[100];

    sprintf(text_energy, "Energy: %d", player.energy);
    sprintf(text_y, "Y: %d", player.y + (player.map_y * DUNGEON_SIZE));
    sprintf(text_x, "X: %d", player.x + (player.map_x * DUNGEON_SIZE));
	sprintf(text_time, "Time: %d:%d:%d:%d", game_time.days, game_time.hours, game_time.minutes, game_time.seconds);

	write_text(game_size/60, (game_size/60), text_energy, player.energy < 100 ? Red : White, 0,0);
	write_text(game_size/60, (game_size/60)*5, text_y, White,0,0);
	write_text(game_size/60, (game_size/60)*10, text_x, White,0,0);
    write_text(game_size/60, (game_size/60)*15, text_time, White,0,0);
    
    
    show_menu();
}

void update_window_size()
{
    int tile_size;
    int window_width;
    int window_height;

    SDL_GetWindowSize(main_window, &window_width, &window_height); 
    if (window_width < window_height)
    {
        tile_size = window_width/(DUNGEON_SIZE);
    } 
	else
    {
        tile_size = window_height/(DUNGEON_SIZE);
    }
    if (tile_size<32) tile_size = 32;
    SDL_SetWindowSize(main_window, tile_size * DUNGEON_SIZE, tile_size * DUNGEON_SIZE);
}

int main(int argi, char** agrs)
{
    struct stat statbuf;
    int ret = stat("textures", &statbuf);
    if (ret) {
        chdir("..");
        ret = stat("textures", &statbuf);
        if (ret) {
            printf("missing directory with textures\n");
            return 2;
        }
    }

    ret = stat("world", &statbuf);
    if (ret)
    {
#if defined(_WIN32)
        mkdir("world");
#else
        mkdir("world", 0777);
#endif
    }

    srand (time(NULL));
	player.energy=250;
	player.back_x=0;
	player.back_y=0;

    load(1);
    
	if (init_window()) return 1;
    if (load_font()) return 1;

    load_textures();
    create_menus();
    for (;;)
    {   
        SDL_Event event;
        clear_window();

        draw();

        while (SDL_PollEvent(&event))
        {
            if (event.type==SDL_QUIT) {SDL_Quit(); return 0;};
            if(event.type == SDL_KEYDOWN)
            {
                int key= event.key.keysym.sym;
                player_interact(key);
            }
            if (event.type==SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                //printf("window event %d %d \n", event.window.data1, event.window.data2);
                update_window_size();
            }
        }
        update_time();
        if (player.energy <= 0)
        {
            printf("You ran out of energy (death)\n");
            printf("This is still in early alpha, the world isn't lost, just run the game and use the new menu (wip) set energy to 400\n");
            printf("Thank you for playing =)\n");
            SDL_Quit();
            return 0;
        }
        // DEBUG x, y, map_x, map_y, running
        //printf("x: %d y: %d\n map_x: %d map_y: %d running: %d\n", player.x, player.y, player.map_x, player.map_y, player.running);                   
        // DEBUG in_menu
        //printf("in_menu: %d\n", in_menu);
        
        SDL_RenderPresent(renderer);
        SDL_Delay(20);   
    }
}
