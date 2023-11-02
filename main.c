#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "window.h"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL2_framerate.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "dungeon.h"
#include "texture.h"
#include "player.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "menu.h"

enum game_tiles screen_list[DUNGEON_SIZE][DUNGEON_SIZE];
enum game_tiles terrain_list[DUNGEON_SIZE][DUNGEON_SIZE];

struct Player player;

void generator()
{
    int random = 0;
    int type_int = 0;
    char chunk_contains_dung_entrance = 0;

    printf("running generator...\n");
    for (int i=0; i<DUNGEON_SIZE; i++)
    {
        for (int j=0; j<DUNGEON_SIZE; j++)
        {
            type_int = rand() % 3;
            switch (type_int)
            {
                case 0:
                    terrain_list[i][j] = TILE_STONE;
                    break;
                case 1:
                    terrain_list[i][j] = TILE_DIRT;
                    break;
                case 2:
                    random = rand() % 1000+0;
                    if (random == 51 && !(chunk_contains_dung_entrance))
                    {
                        terrain_list[i][j] = TILE_DUNG_ENTRANCE;
                        chunk_contains_dung_entrance = 1;
                    }
                    else terrain_list[i][j] = TILE_TREE;
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
        char player_path[17];
        sprintf(player_path, "world/player.txt");
        file = fopen(player_path, "w");
        char to_write[60];
        sprintf(to_write, "%d\n%d\n%d\n%d\n%d\n%d\n", player.map_y, player.map_x, player.y, player.x, (int)player.in_dungeon, player.energy);
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
        char player_path[17];
        sprintf(player_path, "world/player.txt");
        if ((file = fopen(player_path, "r")))
        {
            fscanf(file, "%d%d%d%d%d%d", &player.map_y, &player.map_x, &player.y, &player.x, &temp, &player.energy);
            player.in_dungeon=(char)(temp & 255);
        }
        else 
        {
            file = fopen(player_path, "w");
        }
        fclose(file);
    }
    if (player.in_dungeon)
    {
        char filename[30];
        sprintf(filename, "world/%9d-%9ddung", player.map_x, player.map_y);
        printf("%s\n", filename);
        FILE* chunk;
        if ((chunk = fopen(filename, "r")))
        {
            chunk = fopen(filename, "r");
            fread(dungeon_terrain_list, sizeof(dungeon_terrain_list), 1, chunk);
            fclose(chunk);
        }
        else
        {
            dungeon_generator();
        }
    }
    else
    {
        char filename[26];
        sprintf(filename, "world/%9d-%9d", player.map_x, player.map_y);
        FILE* chunk;
        if ((chunk = fopen(filename, "r")))
        {
            chunk = fopen(filename, "r");
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
    switch (key)
    {
        case SDLK_ESCAPE:
        {
            if ((!(in_menu == MENU_EXIT)) && (!(in_menu == MENU_CLOSED)))
            {
                in_menu = MENU_CLOSED;
                break;
            }
            if (in_menu == MENU_CLOSED)
            {
                in_menu = MENU_EXIT;
                break;
            }
            if (in_menu == MENU_EXIT)
            {
                in_menu = MENU_CLOSED;
                break;
            }
            break;
        }
        case SDLK_m:
        {
            if (in_menu == MENU_CLOSED) 
                in_menu = MENU_ENERGY;
            else if (in_menu == MENU_ENERGY) 
                in_menu = MENU_CLOSED;

            break;
        }
        case SDLK_DOWN:
        case SDLK_s:
        {
            if (!(in_menu == MENU_CLOSED))
            {
                menu_pos++;
                break;
            }

            if (player.in_dungeon)
            {
                if(player.y < DUNGEON_SIZE-1 && !(dungeon_terrain_list[player.x][player.y+1] == TILE_DUNG_WALL))
                {
                    if (player.running) 
                    {
                        player.y++;
                        player.energy--;
                    }
                    player.y++;
                    player.energy--;
                    break;
                }
                break;
            }
            
            if (player.running)
            {
                if (player.running) player.energy--;
                if (player.y < DUNGEON_SIZE-1) player.y++;
                else {
                        player.y=0; 
                        save(0);
                        player.map_y++;
                        load(0);
                }
            }
            
            if (player.y < DUNGEON_SIZE-1) player.y++;
            else {player.y=0; save(0); player.map_y++; load(0);}
            player.energy--;
            
            break;
        }
        case SDLK_w:
        case SDLK_UP:
        {
            if (!(in_menu == MENU_CLOSED))
            {       
                menu_pos--;
                if (menu_pos < 0)
                {
                    menu_pos=0;
                }
                break;
            }

            if (player.in_dungeon)
            {
                if (player.y > 0 && !(dungeon_terrain_list[player.x][player.y-1] == TILE_DUNG_WALL))
                {
                    if (player.running)
                    {
                            player.y--;
                            player.energy--;
                    }
                    player.y--;
                    player.energy--;
                    break;
                }
                break;
            }

            // IN MAIN WORLD
            if (player.running)
            {
                if (player.y > 0) player.y--;
                else {player.y=DUNGEON_SIZE-1; save(0); player.map_y--;load(0);}
                player.energy--;
            }
            if (player.y > 0) player.y--;
            else {player.y=DUNGEON_SIZE-1; save(0); player.map_y--;load(0);}
            
           player.energy--;
            break;
        }
        case SDLK_RIGHT:
        case SDLK_d:
        {
            if (!(in_menu == MENU_CLOSED))
            {
                break;
            }
            if (player.in_dungeon)
            {
                if (player.running)
                {
                    if (player.x < DUNGEON_SIZE-1 && !(dungeon_terrain_list[player.x+1][player.y] == TILE_DUNG_WALL)) player.x++;
                }
                if (player.x < DUNGEON_SIZE-1 && !(dungeon_terrain_list[player.x+1][player.y] == TILE_DUNG_WALL)) player.x++;
            }
            else
            {
                if (player.running)
                {
                    if (player.x < DUNGEON_SIZE-1) player.x++;
                    else if (!player.in_dungeon) {player.x=0; save(0); player.map_x++;load(0);}
                }
                if (player.x < DUNGEON_SIZE-1) player.x++;
                else if (!player.in_dungeon) {player.x=0; save(0); player.map_x++;load(0);}
            }
            player.energy--;
            if (player.running) player.energy--;
            player.going_right = 1;
            break;
        }
        case SDLK_LEFT:
        case SDLK_a:
        {
            if (!(in_menu == MENU_CLOSED))
            {
                break;
            }
            if (player.in_dungeon)
            {
                if (player.running)
                {
                    if (player.x > 0 && !(dungeon_terrain_list[player.x-1][player.y] == TILE_DUNG_WALL)) player.x--;
                }
                if (player.x > 0 && !(dungeon_terrain_list[player.x-1][player.y] == TILE_DUNG_WALL)) player.x--;
            }
            else
            {
                if (player.running)
                {
                    if (player.x > 0) player.x--;
                    else if (!player.in_dungeon) {player.x=DUNGEON_SIZE-1; save(0); player.map_x--;load(0);}
                }
                if (player.x > 0) player.x--;
                else if (!player.in_dungeon) {player.x=DUNGEON_SIZE-1; save(0); player.map_x--;load(0);}
            }
            player.energy--;
            if (player.running) player.energy--;
            player.going_right = 0;
            break;
        }
        case SDLK_r:
        {  
            if (!player.running)
            {
                player.running = 1;
            }
            else if (player.running)
            {
                player.running = 0;
            }
            break;
        }
        case SDLK_RETURN:
        case SDLK_e:
        {
            if (!(in_menu == MENU_CLOSED))
            {
                interact(); 
                break;
            }

            if (screen_list[player.x][player.y] == TILE_DUNG_EXIT)
            {
                player.in_dungeon = 0;
            }
            else if (screen_list[player.x][player.y] == TILE_DUNG_ENTRANCE)
            {
                player.in_dungeon = 1;
                load(0);
            }
            break;
        }
    }
}

void update_screen_list()
{
    for (int i=0; i < DUNGEON_SIZE; i++)
    {
        for (int j=0; j < DUNGEON_SIZE; j++)
        {
            if (player.in_dungeon)
                screen_list[i][j] = dungeon_terrain_list[i][j];
            else
                screen_list[i][j] = terrain_list[i][j];
        }
    } 
}

void draw()
{
    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;
    int game_size;
    SDL_GetWindowSize(main_window, &WINDOW_WIDTH, &WINDOW_HEIGHT); 
    int TILE_DUNGEON_SIZE = 16;
    if (WINDOW_WIDTH < WINDOW_HEIGHT)
    {
        game_size = WINDOW_WIDTH;
        TILE_DUNGEON_SIZE = WINDOW_WIDTH/(DUNGEON_SIZE + 1);
    } 
	else
    {
        game_size = WINDOW_HEIGHT;
        TILE_DUNGEON_SIZE = WINDOW_HEIGHT/(DUNGEON_SIZE + 1);
    }
    for (int i=0; i<(DUNGEON_SIZE); i++)
    {
        int x = i * TILE_DUNGEON_SIZE;
        for (int j=0; j<DUNGEON_SIZE; j++)
        {
            SDL_Texture *texture;
            SDL_Rect screen_rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
            SDL_Rect img_rect = {x, j * TILE_DUNGEON_SIZE, TILE_DUNGEON_SIZE, TILE_DUNGEON_SIZE};
            switch (screen_list[i][j])
            {
                case TILE_STONE:    
                    texture = Texture.stone;
                    break;
                case TILE_DIRT:
                    texture = Texture.dirt;
                    break;
                case TILE_TREE:    
                    texture = Texture.tree;
                    break;
                case TILE_DUNG_ENTRANCE:
                    texture = Texture.dung_entrance;
                    break;
                case TILE_DUNG_EXIT:
                    texture = Texture.dung_exit;
                    break;
                case TILE_DUNG_WALL:
                    texture = Texture.dung_wall;
                    break;
                case TILE_DUNG_FLOOR:
                    texture = Texture.dung_floor;
            }
            SDL_RenderCopy(renderer, texture, NULL, &img_rect);
        }
    }  
    SDL_Rect screen_rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_Rect img_rect = {player.x * TILE_DUNGEON_SIZE, player.y * TILE_DUNGEON_SIZE, TILE_DUNGEON_SIZE, TILE_DUNGEON_SIZE};
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

    sprintf(text_energy, "Energy: %d", player.energy);
    sprintf(text_y, "Y: %d", player.y + (player.map_y * DUNGEON_SIZE));
    sprintf(text_x, "X: %d", player.x + (player.map_x * DUNGEON_SIZE));
	
	write_text(10, 10, text_energy, player.energy < 100 ? Red : White);
	write_text(10, game_size/10, text_y, White);
	write_text(10, game_size/5, text_x, White);
    
    show_menu(in_menu);
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
        mkdir("world", 0777);
    }

    srand (time(NULL));
    player.in_dungeon = 0;
	player.energy=250;

    load(1);
    int key;
    SDL_Event event;
    
	if (init_window()) return 1;
    if (load_font()) return 1;

    load_textures();
    for (;;)
    {   
        clear_window();
        update_screen_list();

        draw();

        while (SDL_PollEvent(&event))
        {
            if (event.type==SDL_QUIT) {SDL_Quit(); return 0;};
            if(event.type == SDL_KEYDOWN)
            {
                key = event.key.keysym.sym;
                switch (key) {
                    case SDLK_w:
                    case SDLK_m:
                    case SDLK_a:
                    case SDLK_s:
                    case SDLK_d:
                    case SDLK_e:
                    case SDLK_UP:
                    case SDLK_DOWN:
                    case SDLK_LEFT:
                    case SDLK_RIGHT:
                    case SDLK_RETURN:
                    case SDLK_ESCAPE:
                    case SDLK_r:
                        player_interact(key);
                        break;
                }
            }
            if (event.type == SDL_KEYUP)
            {
                key = 0;
            }
            
        }
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
