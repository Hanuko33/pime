#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_timer.h>
#include <cstdlib>
#include <time.h>
//#include <SDL2/SDL_render.h>
#include <stdlib.h>
#include <stdio.h>
#include "alchemist/elements.h"
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
}
void load(char with_player)
{
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

    SDL_SetWindowSize(main_window, (tile_size * CHUNK_SIZE) + PANEL_WINDOW, tile_size * CHUNK_SIZE + STATUS_LINE);
    SDL_GetWindowSize(main_window, &window_width, &window_height); 
}

void put_element()
{
    InventoryElement * el = player.hotbar[active_hotbar];
    if (el) {
        el->set_posittion(player.x, player.y);
        set_item_at_ppos(el, &player);
        player.inventory->remove(el);
        player.hotbar[active_hotbar]=NULL;
        printf("item %s placed\n", el->get_name());
    }
}

void use_tile()
{
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
}

bool plant_with_seed(InventoryElement * el)
{
    if (el->get_id() == ID_ACORN2)
    {
        bool able=false;
        for (int i = 0; i < CHUNK_SIZE*CHUNK_SIZE; i++) {
            if (!world_table[player.map_y][player.map_x]->plants[i])
            {
                Plant *p = new Plant();

                p->type = PLANTID_tree2;

                p->set_posittion(player.x, player.y);
                
                p->phase=Plant_seed;
                p->grown=false;

                world_table[player.map_y][player.map_x]->plants[i]=p;

                sprintf(status_line, "Placing %s", p->name);
                status_code=1;
                able=true;
                break;
            }
        }
        
        if (able)
        {
            player.inventory->remove(el);
            player.hotbar[active_hotbar]=NULL;
            free(el);
            el=NULL;
            return true;
        }
    }
    if (el->get_id() == ID_ACORN1)
    {
        bool able=false;
        for (int i = 0; i < CHUNK_SIZE*CHUNK_SIZE; i++) {
            if (!world_table[player.map_y][player.map_x]->plants[i])
            {
                Plant *p = new Plant();

                p->type = PLANTID_tree1;

                p->set_posittion(player.x, player.y);
                
                p->phase=Plant_seed;
                p->grown=false;

                world_table[player.map_y][player.map_x]->plants[i]=p;

                sprintf(status_line, "Placing %s", p->name);
                status_code=1;
                able=true;
                break;
            }
        }
        
        if (able)
        {
            player.inventory->remove(el);
            player.hotbar[active_hotbar]=NULL;
            free(el);
            el=NULL;
            return true;
        }
    }
    if (el->get_id() == ID_ACORN)
    {
        bool able=false;
        for (int i = 0; i < CHUNK_SIZE*CHUNK_SIZE; i++) {
            if (!world_table[player.map_y][player.map_x]->plants[i])
            {
                Plant *p = new Plant();

                p->type = PLANTID_tree;

                p->set_posittion(player.x, player.y);
                
                p->phase=Plant_seed;
                p->grown=false;

                world_table[player.map_y][player.map_x]->plants[i]=p;

                sprintf(status_line, "Placing %s", p->name);
                status_code=1;
                able=true;
                break;
            }
        }
        
        if (able)
        {
            player.inventory->remove(el);
            player.hotbar[active_hotbar]=NULL;
            free(el);
            el=NULL;
            return true;
        }
    }
    if (el->get_id() == ID_STRAWBERRY_SEEDS)
    {
        bool able=false;
        for (int i = 0; i < CHUNK_SIZE*CHUNK_SIZE; i++) {
            if (!world_table[player.map_y][player.map_x]->plants[i])
            {
                Plant *p = new Plant();

                p->type = PLANTID_strawberry;

                p->set_posittion(player.x, player.y);
                
                p->phase=Plant_seed;
                p->grown=false;

                world_table[player.map_y][player.map_x]->plants[i]=p;

                sprintf(status_line, "Placing %s", p->name);
                status_code=1;
                able=true;
                break;
            }
        }
        
        if (able)
        {
            player.inventory->remove(el);
            player.hotbar[active_hotbar]=NULL;
            free(el);
            el=NULL;
            return true;
        }
    }
    return false;

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
        // case SDLK_g: // break same level
        //     switch ((int)player.direction) 
        //     {
        //         case (int)direction::down:
        //             if (player.y == CHUNK_SIZE-1)
        //             {
        //                 if (!load_chunk(player.map_x, player.map_y+1)) 
        //                 {
        //                     sprintf(status_line, "ON EDGE OF WORLD!");
        //                     status_code=0;
        //                     return;
        //                 };
        //                 world_table[player.map_y+1][player.map_x]->table[0][player.x].tile=TILE_AIR;
        //             }
        //             else
        //             {
        //                 world_table[player.map_y][player.map_x]->table[player.y+1][player.x].tile=TILE_AIR;
        //             }
        //             break;
        //         case (int)direction::up:
        //             if (player.y == 0)
        //             {
        //                 if (!load_chunk(player.map_x, player.map_y-1)) 
        //                 {
        //                     sprintf(status_line, "ON EDGE OF WORLD!");
        //                     status_code=0;
        //                     return;
        //                 };

        //                 world_table[player.map_y-1][player.map_x]->table[CHUNK_SIZE-1][player.x].tile=TILE_AIR;
        //             }
        //             else
        //             {
        //                 world_table[player.map_y][player.map_x]->table[player.y-1][player.x].tile=TILE_AIR;
        //             }
        //             break;

        //         case (int)direction::right:
        //             if (player.x == CHUNK_SIZE-1)
        //             {
        //                 if (!load_chunk(player.map_x+1, player.map_y)) 
        //                 {
        //                     sprintf(status_line, "ON EDGE OF WORLD!");
        //                     status_code=0;
        //                     return;
        //                 };

        //                 world_table[player.map_y][player.map_x+1]->table[player.y][0].tile=TILE_AIR;
        //             }
        //             else
        //             {
        //                 world_table[player.map_y][player.map_x]->table[player.y][player.x+1].tile=TILE_AIR;
        //             }
        //             break;
        //         case (int)direction::left:
        //             if (player.x == 0)
        //             {
        //                 if (!load_chunk(player.map_x-1, player.map_y)) 
        //                 {
        //                     sprintf(status_line, "ON EDGE OF WORLD!");
        //                     status_code=0;
        //                     return;
        //                 };

        //                 world_table[player.map_y][player.map_x-1]->table[player.y][CHUNK_SIZE-1].tile=TILE_AIR;
        //             }
        //             else
        //             {
        //                 world_table[player.map_y][player.map_x]->table[player.y][player.x-1].tile=TILE_AIR;
        //             }
        //             break;
        //     break;
        // }
        case SDLK_r:
            player.hotbar[active_hotbar]=NULL;
            break;
        case SDLK_SEMICOLON:
        {
            InventoryElement * el = player.hotbar[active_hotbar];
            if (el)
                el->show(true);
            InventoryElement ** at_ppos = get_item_at_ppos(&player);

            if (at_ppos)
            {
                el = *at_ppos;
                if (el)
                    el->show(true);
            }
            Being ** b_at_ppos = get_being_at_ppos(&player);
            if (b_at_ppos)
            {
                Being * b = *b_at_ppos;
                if (b)
                    b->show();
            }
            Plant ** p_at_ppos = get_plant_at_ppos(&player);
            if (p_at_ppos)
            {
                Plant * p = *p_at_ppos;
                if (p)
                    p->show();
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
        case SDLK_1: active_hotbar=0; break;
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
            InventoryElement * el = player.hotbar[active_hotbar];
            if (el)
            {
                if (el->use(&player))
                    break;
                if (plant_with_seed(el))
                    break;

            }
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
void update()
{
    for (int i = 0; i<CHUNK_SIZE*CHUNK_SIZE; i++) {
        Being * b = world_table[player.map_y][player.map_x]->beings[i];

        if (b)
        {
            b->tick();
        }
    }

    for (int i = 0; i<CHUNK_SIZE*CHUNK_SIZE; i++) {
        Plant * p = world_table[player.map_y][player.map_x]->plants[i];

        if (p)
        {
            p->tick();
        }
    }
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
    for (int y=0; y < CHUNK_SIZE; y++)
    {
        for (int x=0; x < CHUNK_SIZE; x++)
        {
            SDL_Rect img_rect = {x * tile_dungeon_size, y * tile_dungeon_size, tile_dungeon_size, tile_dungeon_size};
            enum game_tiles tile = get_tile_at(player.map_x, player.map_y, x, y);
            SDL_Texture *texture = tiles_textures[tile];
            SDL_RenderCopy(renderer, texture, NULL, &img_rect);
        }
    }
    // render items
    for (int i = 0; i < CHUNK_SIZE*CHUNK_SIZE; i++)
    {
        InventoryElement * o = world_table[player.map_y][player.map_x]->items[i];
        if (o) 
        {
            int x, y;
            o->get_posittion(&x, &y);
            SDL_Rect img_rect = {x * tile_dungeon_size, y * tile_dungeon_size, tile_dungeon_size, tile_dungeon_size};
            SDL_RenderCopy(renderer, o->get_texture(), NULL, &img_rect);
        }
    }
    // render beings
    for (int i = 0; i < CHUNK_SIZE*CHUNK_SIZE; i++)
    {
        Being * b = world_table[player.map_y][player.map_x]->beings[i];
        if (b)
        {
            int x,y;
            b->get_posittion(&x, &y);
            SDL_Rect img_rect = {x * tile_dungeon_size, y * tile_dungeon_size, tile_dungeon_size, tile_dungeon_size};
            SDL_RenderCopy(renderer, b->get_texture(), NULL, &img_rect);
        }
    }
    // render plants
    for (int i = 0; i < CHUNK_SIZE*CHUNK_SIZE; i++)
    {
        Plant * p = world_table[player.map_y][player.map_x]->plants[i];
        if (p)
        {
            int x,y;
            p->get_posittion(&x, &y);

            SDL_Rect img_rect = {x * tile_dungeon_size, y * tile_dungeon_size, tile_dungeon_size, tile_dungeon_size};
            SDL_RenderCopy(renderer, p->get_texture(), NULL, &img_rect);
        }
    }

    // render player
    SDL_Rect img_rect = {player.x * tile_dungeon_size, player.y * tile_dungeon_size, tile_dungeon_size, tile_dungeon_size};
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

        sprintf(text, "Player@(%d, %d)", 
			(player.x + player.map_x * CHUNK_SIZE) - (WORLD_SIZE*CHUNK_SIZE/2),
			(player.y + player.map_y * CHUNK_SIZE) - (WORLD_SIZE*CHUNK_SIZE/2)
			);
	write_text(tx, ty+25, text, White,15,30);

            
    
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
                    case BIOME_PLAINS:
                        pixels[y * WORLD_SIZE + x] = 0xff22ff22;
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
        case BIOME_PLAINS:
            sprintf(text, "biome: plains");
            break;
    }
   
    write_text(tx, ty+245, text, White, 15, 30);

    SDL_UnlockTexture(map);

    SDL_Rect window_rec;
    if (window_height > 650)
    {
        window_rec.w = WORLD_SIZE;
        window_rec.h = WORLD_SIZE;
    }
    else
    {
        window_rec.w = 0;
        window_rec.h = 0;
    }
    window_rec.x = width + 10;
    window_rec.y = window_height - WORLD_SIZE - STATUS_LINE;

    SDL_RenderCopy(renderer, map, NULL, &window_rec);
   
    sprintf(text, "%s: %s", status_line, status_code ? "OK" : "Failed"); 
    write_text(5, window_height - 32, text, White, 15, 30);

    if (current_menu) current_menu->show();
}

void intro()
{
     // int a; 
     // struct termios state, new_state; 
     // tcgetattr(0, &state); 
     // new_state=state; 
     // new_state.c_lflag &= ~(ECHO | ICANON |ECHOE| ISIG); 
     // new_state.c_cc[VMIN] = 1; 
     // tcsetattr(0, TCSANOW, &new_state); 

     // printf("Do you want music y/n? "); 
     // a=getchar(); 
     // if (a=='y') 
     // { 
     //     printf("\nInitializing music\n"); 
     //     if (init_music()) 
     //         printf("Failed to initialize music!\n"); ; 
     //     load_music(); 

     //     Mix_PlayChannel(0, music.music_one, 99999); 
     //     Mix_PlayChannel(1, music.music_two, 99999); 
     //     Mix_Volume(0, 0); 
     //     Mix_Volume(1, 0); 
     //     Mix_Pause(1); 
     // } else printf("\nGame without music\n"); 
     // tcflush(0, TCIFLUSH); 
     // tcsetattr(0, TCSANOW, &state); 
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
    
    // ret = stat("world", &statbuf);
    // if (ret)
    // {
// #if defined(_WIN32)
    //     mkdir("world");
// #else
    //     mkdir("world", 0777);
// #endif
    // }

    srand (time(NULL));
    intro();

    callback_daily=daily_call;
    init_elements();
	game_time=new Game_time;
    
    
	if (init_window()) return 1;
    if (load_font()) return 1;

    load_textures();
    generator();
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
        
        update();
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
