#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_timer.h>
#include <cstdlib>
#include <time.h>
//#include <SDL2/SDL_render.h>
#include <stdlib.h>
#include <stdio.h>
#include "alchemist/elements.h"
#include "object.h"
#include "text.h"
#include "tiles.h"
#include "window.h"
#include <SDL2/SDL_keycode.h>
//#include <SDL2/SDL2_framerate.h>
#include "texture.h"
#include "player.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "menu.h"
#include "world.h"
#include "tiles.h"
#include <termios.h>
#include "alchemist/game_time.h"

/* #define OLDKB */

SDL_Texture *map;
int auto_explore;
int active_hotbar=0;

Player player;

void (*callback_daily)();

void daily_call()
{
}

const char * direction_names[4] =
{
    "up",
    "right",
    "down",
    "left",
};

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
    if (tile_size<48) tile_size = 48;

    SDL_SetWindowSize(main_window, (tile_size * CHUNK_SIZE) + PANEL_WINDOW, tile_size * CHUNK_SIZE + STATUS_LINE);
    SDL_GetWindowSize(main_window, &window_width, &window_height); 
}

void put_element()
{
    InventoryElement * el = player.hotbar[active_hotbar];
    if (el) {
        el->set_posittion(player.x, player.y, player.z);
        set_item_at_ppos(el, &player);
        player.inventory->remove(el);
        player.hotbar[active_hotbar]=NULL;
        printf("item %s placed\n", el->get_name());
    }
}

void use_tile()
{
    struct object ** ob_pointer = get_object_at_ppos(&player);
    InventoryElement ** item_pointer = get_item_at_ppos(&player);
    if (item_pointer)
    {
        InventoryElement * item = *item_pointer;
        player.inventory->add(item);
        for (int i = 0; i<10; i++) {
            if (!player.hotbar[i])
            {
                player.hotbar[i]=item;
                break;
            }
        }
        sprintf(status_line, "got item: %s (%s)", item->get_form_name(), item->get_name()); //player.inventory->get_count(item));
        *item_pointer=NULL; 

       status_code = 1; 
    }
    else if (ob_pointer)
    {
        struct object * ob = *ob_pointer;
        if (ob->type == OBJECT_TREE)
        {
            if (player.hotbar[active_hotbar])
            {
                if (player.hotbar[active_hotbar]->get_id() == PROD_AXE)
                {
                    sprintf(status_line, "Tree mine");
                    status_code = 1;
                    Element * el = new Element(base_elements[ob->base_element_id]);
                    el->set_posittion(player.x, player.y, player.z);
                    set_item_at_ppos(el, &player);
                    /* ob->type = OBJECT_NULL; */
                    free(ob);
                    ob=NULL;
                    *ob_pointer=NULL;
                }
                else
                {
                    sprintf(status_line, "Tree mine: NEED AXE");
                    status_code = 0;
                }
            }
            else
            {
                sprintf(status_line, "Tree mine: NEED AXE");
                status_code = 0;
            }
        }
    }
/*

    if (get_tile_at_ppos(&player) == TILE_DUNG_DOOR)
    {
        save(0);
        if (player.in == LOC_DUNGEON)
        {
            Mix_Pause(1); Mix_Resume(0);
            player.in = LOC_WORLD; player.z = 0;
        }
        else
        {
            Mix_Pause(0); Mix_Resume(1);
            player.in = LOC_DUNGEON; player.z = 1;
        }
        load(0); save(0);
    }
    if (get_tile_at_ppos(&player) == TILE_CAVE_DOOR)
    {
        save(0);
        if (player.in == LOC_CAVE)
        {
            Mix_Pause(1); Mix_Resume(0);
            player.in = LOC_WORLD; player.z = 0;
        }
        else
        {
            Mix_Pause(0); Mix_Resume(1);
            player.in = LOC_CAVE; player.z = 2;
        }
        load(0); save(0);
    }*/
}

void player_interact(int key)
{
    if (menu_interact(key)) return;
    
    switch (key)
    {
        case SDLK_v:
            sprintf(status_line, "");
            status_code = 1;
            break;
        case SDLK_t: // break up
            world_table[player.map_y][player.map_x]->table[player.z][player.y+1][player.x].tile=TILE_AIR;
            switch (player.direction) {
                case direction::down:
                    if (player.z == CHUNK_SIZE-1)
                    {
                        if (!load_chunk(player.map_x, player.map_y+1)) 
                        {
                            sprintf(status_line, "ON EDGE OF WORLD!");
                            status_code=0;
                            return;
                        };

                        world_table[player.map_y+1][player.map_x]->table[0][player.y+1][player.x].tile=TILE_AIR;
                    }
                    else
                    {
                        world_table[player.map_y][player.map_x]->table[player.z+1][player.y+1][player.x].tile=TILE_AIR;
                    }
                    break;
                case direction::up:
                    if (player.z == 0)
                    {
                        if (!load_chunk(player.map_x, player.map_y-1)) 
                        {
                            sprintf(status_line, "ON EDGE OF WORLD!");
                            status_code=0;
                            return;
                        };

                        world_table[player.map_y-1][player.map_x]->table[CHUNK_SIZE-1][player.y+1][player.x].tile=TILE_AIR;
                    }
                    else
                    {
                        world_table[player.map_y][player.map_x]->table[player.z-1][player.y+1][player.x].tile=TILE_AIR;
                    }
                    break;
                case direction::right:
                    if (player.x == CHUNK_SIZE-1)
                    {
                        if (!load_chunk(player.map_x+1, player.map_y)) 
                        {
                            sprintf(status_line, "ON EDGE OF WORLD!");
                            status_code=0;
                            return;
                        };

                        world_table[player.map_y][player.map_x+1]->table[player.z][player.y+1][0].tile=TILE_AIR;
                    }
                    else
                    {
                        world_table[player.map_y][player.map_x]->table[player.z][player.y+1][player.x+1].tile=TILE_AIR;
                    }
                    break;
                case direction::left:
                    if (player.x == 0)
                    {
                        if (!load_chunk(player.map_x-1, player.map_y)) 
                        {
                            sprintf(status_line, "ON EDGE OF WORLD!");
                            status_code=0;
                            return;
                        };

                        world_table[player.map_y][player.map_x-1]->table[player.z][player.y+1][CHUNK_SIZE-1].tile=TILE_AIR;
                    }
                    else
                    {
                        world_table[player.map_y][player.map_x]->table[player.z][player.y+1][player.x-1].tile=TILE_AIR;
                    }
                    break;
            }
            break;
#ifdef OLDKB
        case SDLK_s:
            player.move(0, 1);
            player.direction=direction::down;
            break;
        case SDLK_w:
            player.move(0, -1);
            player.direction=direction::up;
            break;
        case SDLK_a:
            player.move(-1, 0);
            player.direction=direction::left;
            player.going_right=0;
            break;
        case SDLK_d:
            player.move(1, 0);
            player.direction=direction::right;
            player.going_right=1;
            break;

#endif 
        case SDLK_g: // break same level
            switch (player.direction) {
                case direction::down:
                    if (player.z == CHUNK_SIZE-1)
                    {
                        if (!load_chunk(player.map_x, player.map_y+1)) 
                        {
                            sprintf(status_line, "ON EDGE OF WORLD!");
                            status_code=0;
                            return;
                        };

                        world_table[player.map_y+1][player.map_x]->table[0][player.y][player.x].tile=TILE_AIR;
                    }
                    else
                    {
                        world_table[player.map_y][player.map_x]->table[player.z+1][player.y][player.x].tile=TILE_AIR;
                    }
                    break;
                case direction::up:
                    if (player.z == 0)
                    {
                        if (!load_chunk(player.map_x, player.map_y-1)) 
                        {
                            sprintf(status_line, "ON EDGE OF WORLD!");
                            status_code=0;
                            return;
                        };

                        world_table[player.map_y-1][player.map_x]->table[CHUNK_SIZE-1][player.y][player.x].tile=TILE_AIR;
                    }
                    else
                    {
                        world_table[player.map_y][player.map_x]->table[player.z-1][player.y][player.x].tile=TILE_AIR;
                    }
                    break;
                case direction::right:
                    if (player.x == CHUNK_SIZE-1)
                    {
                        if (!load_chunk(player.map_x+1, player.map_y)) 
                        {
                            sprintf(status_line, "ON EDGE OF WORLD!");
                            status_code=0;
                            return;
                        };

                        world_table[player.map_y][player.map_x+1]->table[player.z][player.y][0].tile=TILE_AIR;
                    }
                    else
                    {
                        world_table[player.map_y][player.map_x]->table[player.z][player.y][player.x+1].tile=TILE_AIR;
                    }
                    break;
                case direction::left:
                    if (player.x == 0)
                    {
                        if (!load_chunk(player.map_x-1, player.map_y)) 
                        {
                            sprintf(status_line, "ON EDGE OF WORLD!");
                            status_code=0;
                            return;
                        };

                        world_table[player.map_y][player.map_x-1]->table[player.z][player.y][CHUNK_SIZE-1].tile=TILE_AIR;
                    }
                    else
                    {
                        world_table[player.map_y][player.map_x]->table[player.z][player.y][player.x-1].tile=TILE_AIR;
                    }
                    break;
            }
            break;
        case SDLK_b: // break under
            if (player.y>1)
            {
                world_table[player.map_y][player.map_x]->table[player.z][player.y-1][player.x].tile=TILE_AIR;
                player.move(0, 0);
            }
            break;
        case SDLK_r:
            player.hotbar[active_hotbar]=NULL;
            break;
        case SDLK_SEMICOLON:
        {
            InventoryElement * el = player.hotbar[active_hotbar];
            if (el)
            {
                el->show(true);
            }
        }
        break;
        
        case SDLK_f:
        {
            InventoryElement * el = player.hotbar[active_hotbar];
            if (el)
            {
                Edible * edible = el->get_edible();
                if (edible)
                {
                    player.thirst+=edible->irrigation;
                    player.hunger+=edible->caloric;
                    player.inventory->remove(el);
                    player.hotbar[active_hotbar]=NULL;
                    sprintf(status_line, "eat");
                    status_code = 1;
                    if (edible->poison)
                    {
                        player.thirst-=edible->poison*10;
                        player.hunger-=edible->poison*10;
                        sprintf(status_line, "eat: GOT POISONED");
                    }
                }
                else
                {
                    sprintf(status_line, "eat: not food");
                    status_code = 0;
                }
            }
            
            break;
        }
        case SDLK_F11:
            update_window_size();
            break;
/*        case SDLK_EQUALS:
            if (player.hotbar[active_hotbar] == IT_pumpkin && player.inventory[IT_pumpkin] && player.hunger < 1000)
            {
                player.inventory[IT_pumpkin]--;
                player.hunger+=75;
            }
            if (player.hotbar[active_hotbar] == IT_watermelon && player.inventory[IT_watermelon]) 
            {
                player.inventory[IT_watermelon]--;
                player.hunger+=50;
                player.thirst+=100;
            }
            break;
  */    case SDLK_1: active_hotbar=0; break;
        case SDLK_2: active_hotbar=1; break;
        case SDLK_3: active_hotbar=2; break;
        case SDLK_4: active_hotbar=3; break;
        case SDLK_5: active_hotbar=4; break;
        case SDLK_6: active_hotbar=5; break;
        case SDLK_7: active_hotbar=6; break;
        case SDLK_8: active_hotbar=7; break;
        case SDLK_9: active_hotbar=8; break;
        case SDLK_0: active_hotbar=9; break;
    
		case SDLK_q: put_element(); break;

		case SDLK_BACKQUOTE: active_hotbar--; if (active_hotbar==-1) active_hotbar=9; break;
        case SDLK_TAB: active_hotbar++; if (active_hotbar==10) active_hotbar=0; break;
		
        case SDLK_MINUS: player.craftbar[active_hotbar]=0;  break;
        case SDLK_EQUALS: player.craftbar[active_hotbar]=1;  break;

        case SDLK_F5:
            auto_explore^=1;
        break;
		case SDLK_F4:
		{
           	InventoryElement ** item_pointer = get_item_at_ppos(&player);
            if (item_pointer)
                (*item_pointer)->show();
		}
		break;
                    

        case SDLK_RETURN:
        case SDLK_e:
            use_tile();
            break;
    }
}

Uint64 move_interact(const Uint8 * keys, Uint64 last_time, int * last_frame_press)
{
    Uint64 current_time = SDL_GetTicks64();
    int time_period = 0;
    if (keys[SDL_SCANCODE_LSHIFT])
    {
        player.sneaking = 1;
        time_period = 200;
    }
    else
    {
        player.sneaking = 0;
        if (keys[SDL_SCANCODE_LCTRL] && player.hunger && player.thirst)
        {
            player.running = 1;
            time_period = 50;
        }
        else
        {
            player.running = 0;
            time_period = 100;
        }
    }
      
    if (current_menu==NULL && ((current_time - last_time > time_period) || !(*last_frame_press)))
    {
        if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S])
        {
            player.move(0, 1);
            player.direction=direction::down;
            *last_frame_press=1;
        }
        else if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W])
        {
            player.direction=direction::up;
            player.move(0, -1);
            *last_frame_press=1;
        }
        if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT])
        {
            player.going_right=1;
            player.direction=direction::right;
            player.move(1, 0);
            *last_frame_press=1;
        }
        else if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])
        {
            player.going_right=0;
            player.direction=direction::left;
            player.move(-1, 0);
            *last_frame_press=1;
        }
        if (last_frame_press)
        {
            return SDL_GetTicks64();
        }
    }
    if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])
    {
        *last_frame_press=1;
        return 0;
    }

    *last_frame_press=0;
    return 0;
}

int next_to(int x1, int y1, int x2, int y2)
{
    if (x1==x2 && y1==y2)
        return 2;

    if ((x1==x2+1 || x1==x2-1) && y1==y2)
        return 1;
    if ((y1==y2+1 || y1==y2-1) && x1==x2)
        return 1;
    return 0;
}
void draw()
{
    int game_size;
    int tile_dungeon_size;
    int width = window_width - PANEL_WINDOW;
    char text[256];

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

    // render terrain
    int heightmap[CHUNK_SIZE][CHUNK_SIZE];
    for (int z=0; z < CHUNK_SIZE; z++)
    {
        int y_size = z * tile_dungeon_size;
        for (int x=0; x < CHUNK_SIZE; x++)
        {
            int dy = 0;
            SDL_Rect img_rect = {x * tile_dungeon_size, y_size, tile_dungeon_size, tile_dungeon_size};

            if (get_tile_at(player.map_x, player.map_y, x, player.y, z) == TILE_AIR) 
            {
                while (get_tile_at(player.map_x, player.map_y, x, player.y + dy, z) == TILE_AIR) 
                {
                    dy--;
                }
            }
            else 
            {
                while (get_tile_at(player.map_x, player.map_y, x, player.y + dy +1 , z) != TILE_AIR) 
                {
                    dy++;
                }
             }
             
            heightmap[x][z] = dy;
            if (player.y+dy < player.y)
            {
                enum game_tiles tile = get_tile_at(player.map_x, player.map_y, x, player.y+dy, z);
                SDL_Texture *texture = tiles_textures[tile];
                SDL_RenderCopy(renderer, texture, NULL, &img_rect);
            }
            else if (next_to(player.x, player.z, x, z) == 1 && player.y+dy < player.y+1)
            {
                enum game_tiles tile = get_tile_at(player.map_x, player.map_y, x, player.y+dy, z);
                SDL_Texture *texture = tiles_textures[tile];
                SDL_RenderCopy(renderer, texture, NULL, &img_rect);
                draw_rectangle(x * tile_dungeon_size, y_size, tile_dungeon_size, tile_dungeon_size, SDL_Color{255,255,255,100}, SDL_Color{255,255,255,100}, SDL_Color{255,255,255,100}, SDL_Color{255,255,255,100});
            }
        }
    }
    // render objects
    for (int i = 0; i < 128; i++)
    {
        struct object * o = world_table[player.map_y][player.map_x]->objects[i];
        if (o)
        {
            if (o->y<=player.y)
            {
                SDL_Rect img_rect = {o->x * tile_dungeon_size, o->z * tile_dungeon_size, tile_dungeon_size, tile_dungeon_size};
                int dy = o->y - player.y;
                if (heightmap[o->x][o->z]+1 == dy)
                {
                    SDL_RenderCopy(renderer, objects_textures[o->type], NULL, &img_rect);
                }
            }
        }
    }
    // render items
    for (int i = 0; i < 128; i++)
    {
        InventoryElement * o = world_table[player.map_y][player.map_x]->items[i];
        if (o) 
        {
            int x, y, z;
            o->get_posittion(&x, &y, &z);
            if (y<=player.y)
            {
                SDL_Rect img_rect = {x * tile_dungeon_size, z * tile_dungeon_size, tile_dungeon_size, tile_dungeon_size};
                int dy = y - player.y;
                if (heightmap[x][z]+1 == dy)
                {
                    SDL_RenderCopy(renderer, o->get_texture(), NULL, &img_rect);
                }
            }
        }
    }

    // render mask
    for (int y=0; y < CHUNK_SIZE ; y++)
    {
        int y_size = y * tile_dungeon_size;
        for (int x=0; x < CHUNK_SIZE; x++)
        {
            int dy = heightmap[x][y];
            SDL_Rect img_rect = {x * tile_dungeon_size, y_size, tile_dungeon_size, tile_dungeon_size};
            while (dy < -1)
            {
                draw_rectangle(x * tile_dungeon_size, y_size, tile_dungeon_size, tile_dungeon_size, SDL_Color{0,0,0,100}, SDL_Color{0,0,0,100}, SDL_Color{0,0,0,100}, SDL_Color{0,0,0,100});
                dy++;
            }
        }
    }
    
    // render player
   SDL_Rect img_rect = {player.x * tile_dungeon_size, player.z * tile_dungeon_size, tile_dungeon_size, tile_dungeon_size};
    if (player.going_right) SDL_RenderCopy(renderer, Texture.playerr, NULL, &img_rect);
    else SDL_RenderCopy(renderer, Texture.playerl, NULL, &img_rect);

    // render GUI
    int icon_size = game_size/10;
    if (player.running)
    {
        SDL_Rect running_icon_rect = {(int)(game_size-(icon_size*1.1)), 0, icon_size, icon_size};
        SDL_RenderCopy(renderer, Texture.run_icon, NULL, &running_icon_rect);
    }
    if (player.sneaking)
    {
        SDL_Rect sneaking_icon_rect = {(int)(game_size-(icon_size*1.1)), 0, icon_size, icon_size};
        SDL_RenderCopy(renderer, Texture.sneak_icon, NULL, &sneaking_icon_rect);
    }

     
    int tx=width+10;
    int ty=10;

    sprintf(text, "Hunger: %d", player.hunger);
    write_text(tx, ty, text, player.hunger < 100 ? Red : White, 15,30);
    ty +=25; 
    
    sprintf(text, "Irrigation: %d", player.thirst);
    write_text(tx, ty, text, player.thirst < 100 ? Red : White, 15,30);
    ty +=25; 

    sprintf(text, "Direction: %s", direction_names[(int)player.direction]);
    write_text(tx, ty, text, player.thirst < 100 ? Red : White, 15,30);
    ty +=25; 

        sprintf(text, "Player@(%d, %d, %d)->%d", 
			(player.x + player.map_x * CHUNK_SIZE) - (WORLD_SIZE*CHUNK_SIZE/2),
			player.y,
			(player.z + player.map_y * CHUNK_SIZE) - (WORLD_SIZE*CHUNK_SIZE/2),
            get_tile_at(player.map_x, player.map_y, player.x, player.y-1, player.z) 
			);
	write_text(tx, ty+25, text, White,15,30);

    //sprintf(text, "Time: %d:%d:%d:%d", game_time.days, game_time.hours, game_time.minutes, game_time.seconds);
    //write_text(tx, ty+50, text, White,15,30);
            
    
    InventoryElement ** ip = get_item_at_ppos(&player);
    if (ip) {
        InventoryElement * item = *ip;
        sprintf(text, "Item: %s (%s)", item->get_form_name(), item->get_name());
        write_text(tx, ty+75, text, White,15,30);
    }

	for (int i=0; i < 10; i++)
	{
		SDL_Rect rect;
		rect.x = tx+48*i;
		rect.y = ty+155;
		rect.w = 48;
		rect.h = 48;

		if (player.hotbar[i])
        {
            InventoryElement * item = player.hotbar[i];
			SDL_Texture *texture = item->get_texture();
            SDL_RenderCopy(renderer, texture, NULL, &rect);
            if (i == active_hotbar) {
                sprintf(text, "%s (%s)", item->get_form_name(), item->get_name() );
	    	    write_text(tx + 3 , rect.y+50, text, Yellow, 10,20);
            }
		}
		if (i == active_hotbar) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		} else  {
			SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
        }

        if (player.craftbar[i])
        {
            if (i == active_hotbar)
    		    {
    			    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
            }
            else 
            {
    			    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            }
        }

		SDL_RenderDrawRect(renderer, &rect);
    
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
                pixels[y * WORLD_SIZE + x] = 0xff202020;
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

    switch(world_table[player.map_y][player.map_x]->biome)
    {
        case BIOME_DESERT:
            sprintf(text, "biome: desert");
            break;
        case BIOME_FOREST:
            sprintf(text, "biome: forest");
            break;
        case BIOME_SWEET_TREE:
            sprintf(text, "biome: sweet forest");
            break;
        case BIOME_LAKE:
            sprintf(text, "biome: lake");
            break;
    }
   
    write_text(tx, ty+245, text, White, 15, 30);

    SDL_UnlockTexture(map);

    SDL_Rect window_rec;
    window_rec.w = WORLD_SIZE;
    window_rec.h = WORLD_SIZE;
    window_rec.x = width + 10;
    window_rec.y = window_height - WORLD_SIZE - STATUS_LINE;

    SDL_RenderCopy(renderer, map, NULL, &window_rec);
   
    sprintf(text, "%s: %s", status_line, status_code ? "OK" : "Failed"); 
    write_text(5, window_height - 32, text, White, 15, 30);

    if (current_menu) current_menu->show();
}

void intro()
{
    /* int a; */
    /* struct termios state, new_state; */
    /* tcgetattr(0, &state); */
    /* new_state=state; */
    /* new_state.c_lflag &= ~(ECHO | ICANON |ECHOE| ISIG); */
    /* new_state.c_cc[VMIN] = 1; */
    /* tcsetattr(0, TCSANOW, &new_state); */

    /* printf("Do you want music y/n? "); */
    /* a=getchar(); */
    /* if (a=='y') */
    /* { */
    /*     printf("\nInitializing music\n"); */
    /*     if (init_music()) */ 
    /*         printf("Failed to initialize music!\n"); ; */
    /*     load_music(); */

    /*     Mix_PlayChannel(0, music.music_one, 99999); */ 
    /*     Mix_PlayChannel(1, music.music_two, 99999); */
    /*     Mix_Volume(0, 0); */
    /*     Mix_Volume(1, 0); */
    /*     Mix_Pause(1); */
    /* } else printf("\nGame without music\n"); */
    /* tcflush(0, TCIFLUSH); */
    /* tcsetattr(0, TCSANOW, &state); */
}


int main()
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
    intro();

    callback_daily=daily_call;
    init_elements();
	game_time=new Game_time;
    
    generator();
    
	player.y = height_at(WORLD_CENTER, WORLD_CENTER, 0, 0);
    
	if (init_window()) return 1;
    if (load_font()) return 1;

    load_textures();
    create_menus();
    map = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WORLD_SIZE, WORLD_SIZE);

    load(1);
    
    int dst_map_x=player.map_x;
    int dst_map_y=player.map_y;
    int last_frame_press=0;

    Uint64 last_time = 0;
   
    sprintf(status_line, "Welcome in game!");
    status_code = 1;
    
    int ww=0, wh=0;
    for (;;)
    {   
        SDL_Event event;
        clear_window();

        if (player.hunger < 0) player.hunger = 0;
        if (player.thirst < 0) player.thirst = 0;
        
        draw();

        // keyboard handling for not move
        while (SDL_PollEvent(&event))
        {
            if (event.type==SDL_QUIT) {SDL_Quit(); return 0;};
            if(event.type == SDL_KEYDOWN)
            {
                int key= event.key.keysym.sym;

                player_interact(key);
            }
            if (event.type==SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED )
            { 
                //i3 window manager sends these events if window is not floated 
                if (ww != event.window.data1 && wh != event.window.data2) {
                    printf("window event: resizing to %d, %d\n", event.window.data1, event.window.data2);
                    update_window_size();
                    ww=event.window.data1;
                    wh=event.window.data2;
                }
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

        // keyboard handling for move
#ifndef OLDKB
        if (player.hunger > 0 || rand() % 3)
        {
            const Uint8 *currentKeyState = SDL_GetKeyboardState(NULL);
            Uint64 tmp = move_interact(currentKeyState, last_time, &last_frame_press);
            if (tmp > 0)
            {
                last_time = tmp;
            }
        }
#endif
        // printf("%d\n", last_frame_press);


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
           if (dst_map_x > player.map_x) player.move(1, 0);
           if (dst_map_x < player.map_x) player.move(-1, 0);
           if (dst_map_y > player.map_y) player.move(0, 1);
           if (dst_map_y < player.map_y) player.move(0, -1);
         } else {
                dst_map_x=player.map_x;
                dst_map_y=player.map_y;
         }

        SDL_RenderPresent(renderer);
        if (!auto_explore) SDL_Delay(20);   
    }
}
