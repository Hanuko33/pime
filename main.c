#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "window.h"
#include "music.h"
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
#include "world.h"


struct Player player;


void save(char with_player)
{
 /*   if (with_player)
    {
        int temp;
        FILE *file;
        char to_write[60];
        
        file = fopen("world/player.txt", "w");
        sprintf(to_write, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n", player.map_y, player.map_x, player.y, player.x, player.energy, player.back_x, player.back_y, game_time.days, game_time.hours, game_time.minutes, game_time.seconds, player.in);
        fwrite(to_write, sizeof(to_write), 1, file);
        fclose(file);
    }
    if (player.in == LOC_DUNGEON)
    {
        char filename[31];
        sprintf(filename, "world/%9d-%9ddung", player.map_x, player.map_y);
        FILE *chunk = fopen(filename, "w");
        fwrite(dungeon_terrain_list, sizeof(dungeon_terrain_list), 1, chunk);
        fclose(chunk);
    }
    else if (player.in == LOC_CAVE)
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
    }*/
}
void load(char with_player)
{
/*    if (with_player)
    {
        int temp;
        FILE *file;
        
        if (file = fopen("world/player.txt", "r"))
        {
            printf("loading player: ");
            if (fscanf(file, "%d%d%d%d%d%d%d%d%d%d%d%d", 
                   &player.map_y, &player.map_x, &player.y, &player.x, 
                   &player.energy, 
                   &player.back_x, &player.back_y, 
                   &game_time.days, &game_time.hours, &game_time.minutes, &game_time.seconds, &player.in) == 12 )
            {
                if (player.in == LOC_DUNGEON) 
                {
                    screen_list=&dungeon_terrain_list;
                    Mix_Pause(0);
                    Mix_Resume(1);
                }
                else if (player.in == LOC_CAVE)
                {
                    screen_list=&cave_terrain_list;
                    Mix_Pause(0);
                    Mix_Resume(1);
                }
                else 
                    screen_list = &terrain_list;
                printf(" done\n");
            }
            else printf("failed\n");
            fclose(file);
        }
    }
    if (player.in == LOC_DUNGEON)
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
				if (player.x<DUNGEON_SIZE-1)
				{
					player.x++;
				}
				else
				{
					player.x=1;
                    if (player.y < DUNGEON_SIZE-2) player.y++;
				}
                if (dungeon_terrain_list[player.y][player.x] == TILE_DUNG_FLOOR || dungeon_terrain_list[player.y][player.x] == TILE_DUNG_DOOR) 
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
    else if (player.in == LOC_CAVE)
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
				if (player.x<DUNGEON_SIZE-2)
				{
					player.x++;
				}
				else
				{
					player.x=0;
                    if (player.y<DUNGEON_SIZE-2) player.y++;
				}
                if (cave_terrain_list[player.y][player.x] == TILE_CAVE_FLOOR || cave_terrain_list[player.y][player.x] == TILE_CAVE_DOOR) 
                {
                    stuck = 0;
                    break;
                }
          	}
        }
        else
        {
            cave_generator();
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
    }*/
}


void player_interact(int key )
{
    if ( menu_interact(key)) return;
    
    switch (key)
    {
        case SDLK_F1:
            generator();
            player.z = 0;
            player.in = LOC_WORLD;
        break;

        case SDLK_F2:
            generate_dungeon(world_table[player.map_y][player.map_x], player.x, player.y);
            player.z = 1;
            player.in = LOC_DUNGEON;
        break;

        case SDLK_F3:
            generate_cave(world_table[player.map_y][player.map_x], player.x, player.y);
            player.z = 2;
            player.in = LOC_CAVE;
        break;
                    
        case SDLK_DOWN:
        case SDLK_s:
        {
            player_move(&player, 0, 1);
            break;
        }
        case SDLK_w:
        case SDLK_UP:
        {
            player_move(&player, 0, -1);
            break;
        }
        case SDLK_RIGHT:
        case SDLK_d:
        {
            player_move(&player, 1, 0);
            break;
        }
        case SDLK_LEFT:
        case SDLK_a:
        {
            player_move(&player, -1, 0);
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
            printf("player: %d, %d\n", player.x, player.y);
            if (get_tile_at_player_position(&player) == TILE_DUNG_DOOR)
            {
                save(0);
                game_time.minutes++;
                if (player.in == LOC_DUNGEON)
                {
                    Mix_Pause(1);
                    Mix_Resume(0);
                    player.in = LOC_WORLD;
                    player.z = 0;
                }
                else
                {
                    Mix_Pause(0);
                    Mix_Resume(1);
                    player.in = LOC_DUNGEON;
                    player.z = 1;
                }
                load(0);
                save(0);
            }
            if (get_tile_at_player_position(&player) == TILE_CAVE_DOOR)
            {
                save(0);
                game_time.minutes++;
                if (player.in == LOC_CAVE)
                {
                    Mix_Pause(1);
                    Mix_Resume(0);
                    player.in = LOC_WORLD;
                    player.z = 0;
                }
                else
                {
                    Mix_Pause(0);
                    Mix_Resume(1);
                    player.in = LOC_CAVE;
                    player.z = 2;
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
        tile_dungeon_size = window_width/(CHUNK_SIZE);
    } 
    else
    {
        game_size = window_height;
        tile_dungeon_size = window_height/(CHUNK_SIZE);
    }
    for (int i=0; i < CHUNK_SIZE ; i++)
    {
        int y = i * tile_dungeon_size;
        for (int j=0; j < CHUNK_SIZE; j++)
        {
            SDL_Texture *texture;
            SDL_Rect img_rect = {j * tile_dungeon_size, y, tile_dungeon_size, tile_dungeon_size};
            switch ((*(world_table[player.map_y][player.map_x]))[player.z][i][j])
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
    sprintf(text_y, "Y: %d", player.y + (player.map_y * CHUNK_SIZE));
    sprintf(text_x, "X: %d", player.x + (player.map_x * CHUNK_SIZE));
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
        tile_size = window_width/(CHUNK_SIZE);
    } 
	else
    {
        tile_size = window_height/(CHUNK_SIZE);
    }
    if (tile_size<32) tile_size = 32;
    SDL_SetWindowSize(main_window, tile_size * CHUNK_SIZE, tile_size * CHUNK_SIZE);
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

    
	if (init_window()) return 1;
    if (load_font()) return 1;
    if (init_music()) return 1;

    load_textures();
    create_menus();
    load_music();

    Mix_PlayChannel(0, music.music_one, 99999); 
    Mix_PlayChannel(1, music.music_two, 99999);
    Mix_Volume(0, 0);
    Mix_Volume(1, 0);
    Mix_Pause(1);

    load(1);
    generator();
    
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
