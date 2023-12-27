#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiles.h"
#include "window.h"
#include "music.h"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL2_framerate.h>
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
#include "tiles.h"

SDL_Texture *map;
int auto_explore;

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
        case SDLK_F5:
            auto_explore^=1;
        break;
                    
        case SDLK_DOWN:
        case SDLK_s:
        {
            move_player(&player, 0, 1);
            break;
        }
        case SDLK_w:
        case SDLK_UP:
        {
            move_player(&player, 0, -1);
            break;
        }
        case SDLK_RIGHT:
        case SDLK_d:
        {
            player.going_right=1;
            move_player(&player, 1, 0);
            break;
        }
        case SDLK_LEFT:
        case SDLK_a:
        {
            player.going_right=0;
            move_player(&player, -1, 0);
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
            
            if (get_item_at_ppos(&player).count)
            {
                int item_id = get_item_at_ppos(&player).id;
                player.inventory[item_id]+=get_item_at_ppos(&player).count;
                printf("GOT ITEM: %s, new amount: %d\n", items_names[get_item_at_ppos(&player).id], player.inventory[item_id]);
                world_table[player.map_x][player.map_y]->table[player.z][player.y][player.x].item.count=0;
            }

            if (get_tile_at_ppos(&player) == TILE_SAND)
            {
                player.inventory[IT_sand]++;
                printf("GOT SAND, new amount: %d\n", player.inventory[IT_sand]);
            }

            if (get_tile_at_ppos(&player) == TILE_DUNG_DOOR)
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
            if (get_tile_at_ppos(&player) == TILE_CAVE_DOOR)
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
    int game_size;
    int tile_dungeon_size;
    int width = window_width - PANEL_WINDOW;

    if (width < window_height)
    {
        game_size = width;
        tile_dungeon_size = width/(CHUNK_SIZE);
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
            SDL_Rect img_rect = {j * tile_dungeon_size, y, tile_dungeon_size, tile_dungeon_size};
            struct tile * tile = &((world_table[player.map_y][player.map_x])->table[player.z][i][j]);
            SDL_Texture *texture = tiles_textures[tile->tile];
            SDL_RenderCopy(renderer, texture, NULL, &img_rect);
            if (tile->item.count) {
                SDL_Texture *texture = items_textures[tile->item.id];
                SDL_RenderCopy(renderer, texture, NULL, &img_rect);
            }
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

    char text[256];
     
    int tx=width+10;
    int ty=10;

    sprintf(text, "Energy: %d", player.energy);
	write_text(tx, ty, text, player.energy < 100 ? Red : White, 20,30);
    
    sprintf(text, "Y: %d", (player.y + (player.map_y * CHUNK_SIZE)) - (WORLD_SIZE*CHUNK_SIZE/2));
	write_text(tx, ty+25, text, White,20,30);

    sprintf(text, "X: %d", player.x + (player.map_x * CHUNK_SIZE)- (WORLD_SIZE*CHUNK_SIZE/2));
	write_text(tx, ty+50, text, White,20,30);
	
    sprintf(text, "Time: %d:%d:%d:%d", game_time.days, game_time.hours, game_time.minutes, game_time.seconds);
    write_text(tx, ty+75, text, White,20,30);
            
    
    struct tile * tile = &((world_table[player.map_y][player.map_x])->table[player.z][player.y][player.x]);
    if (tile->item.count) {
        sprintf(text, "Items: %s %d", items_names[tile->item.id], tile->item.count);
        write_text(tx, ty+125, text, White,20,30);
    }

    unsigned int * pixels;
    int pitch, x, y;

    SDL_LockTexture(map, NULL, (void**)&pixels, &pitch);

    for (y = 0; y < WORLD_SIZE; y++)
    {
        for (x = 0; x < WORLD_SIZE; x++)
        {
           chunk* chunk = world_table[y][x];
           if (chunk) {
               switch(chunk->biome)
               {
                   case BIOME_DESERT:
                       pixels[y * WORLD_SIZE + x] = 0xffffff00;
                       break;
                   case BIOME_FOREST:
                       pixels[y * WORLD_SIZE + x] = 0xff00ff00;
                       break;
                   case BIOME_SWEET_TREE:
                       pixels[y * WORLD_SIZE + x] = 0xff79510a;
                       break;
                   case BIOME_LAKE:
                       pixels[y * WORLD_SIZE + x] = 0xff0000ff;
                       break;
               }
           }
             else 
                  pixels[y * WORLD_SIZE + x] = 0xff000000;
        }
    }
    
    unsigned int p=pixels[player.map_y * WORLD_SIZE + player.map_x];
    for (y=0; y < 3; y++)
        for (x=0; x< 3; x++)
        {
            int py=player.map_y+y-1;
            int px=player.map_x+x-1;
            if (py >=0 && py < WORLD_SIZE && px >= 0 && px < WORLD_SIZE)
                pixels[py * WORLD_SIZE + px]=0xffffffff;
        }

    SDL_UnlockTexture(map);

    SDL_Rect window_rec;
    window_rec.w = WORLD_SIZE;
    window_rec.h = WORLD_SIZE;
    window_rec.x = width + 10;
    window_rec.y = window_height - WORLD_SIZE - 10 ;

    SDL_RenderCopy(renderer, map, NULL, &window_rec);

    show_menu();
}

void update_window_size()
{
    int tile_size;
    int width;
   SDL_GetWindowSize(main_window, &window_width, &window_height); 

   width = window_width - PANEL_WINDOW;

    if (width < window_height)
    {
        tile_size = width/(CHUNK_SIZE);
    } 
	else
    {
        tile_size = window_height/(CHUNK_SIZE);
    }
    if (tile_size<32) tile_size = 32;

    SDL_SetWindowSize(main_window, (tile_size * CHUNK_SIZE) + PANEL_WINDOW, tile_size * CHUNK_SIZE);
    SDL_GetWindowSize(main_window, &window_width, &window_height); 
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
    init_player();
    
	if (init_window()) return 1;
    if (load_font()) return 1;
    if (init_music()) return 1;
    
    init_items();
    load_textures();
    create_menus();
    load_music();

    map = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WORLD_SIZE, WORLD_SIZE);
    Mix_PlayChannel(0, music.music_one, 99999); 
    Mix_PlayChannel(1, music.music_two, 99999);
    Mix_Volume(0, 0);
    Mix_Volume(1, 0);
    Mix_Pause(1);

    load(1);
    generator();
    
    int dst_map_x=player.map_x;
    int dst_map_y=player.map_y;
   
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
              //  printf("window event %d %d \n", event.window.data1, event.window.data2);
                update_window_size();
            }

            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
             /*   switch (event.button.button)
                {
                    case 1: break;
                    case 2: break;
                    case 3: break;
                }
             */
                 printf("mouse %d,%d %d\n", event.button.x, event.button.y, event.button.button);
            }
            
        }
        if (auto_explore) {
           if ((dst_map_x == player.map_x) && (dst_map_y == player.map_y)) { 
                int dx = 5 - (rand() % 11);
                int dy = 5 - (rand() % 11);

               if (player.map_y+dy >= 0 && player.map_y+dy < WORLD_SIZE && player.map_x +dx >= 0 && player.map_x+dx < WORLD_SIZE)
               {
                    if (!world_table[player.map_y+dy][player.map_x+dx]) {                
                        dst_map_x=player.map_x + dx;
                        dst_map_y=player.map_y + dy;
                    }
               }
           }
           if (dst_map_x > player.map_x) move_player(&player, 1, 0);
           if (dst_map_x < player.map_x) move_player(&player, -1, 0);
           if (dst_map_y > player.map_y) move_player(&player, 0, 1);
           if (dst_map_y < player.map_y) move_player(&player, 0, -1);
         } else {
                dst_map_x=player.map_x;
                dst_map_y=player.map_y;
         }


        update_time();
#if 0
        if (player.energy <= 0)
        {
            printf("You ran out of energy (death)\n");
            printf("This is still in early alpha, the world isn't lost, just run the game and use the new menu (wip) set energy to 400\n");
            printf("Thank you for playing =)\n");
            SDL_Quit();
            return 0;
        }
#endif
        // DEBUG x, y, map_x, map_y, running
        //printf("x: %d y: %d\n map_x: %d map_y: %d running: %d\n", player.x, player.y, player.map_x, player.map_y, player.running);                   
        // DEBUG in_menu
        //printf("in_menu: %d\n", in_menu);
        
        SDL_RenderPresent(renderer);
        if (!auto_explore) SDL_Delay(20);   
    }
}
