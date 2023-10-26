#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "window.h"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL2_framerate.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "dungeon.h"
#include "texture.h"
#include "text.h"
#include "player.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define SIZE 16 

using namespace std;

game_tiles screen_list[SIZE][SIZE];
game_tiles terrain_list[SIZE][SIZE];
Dungeon dungeon;
Text text;

Player player;

int get_intiger_lenght(int intiger)
{
    int lenght;

    if ((intiger/1000000000)>=1 || (intiger/100000000)<=-1)
    {
        return 10;
    }
    if ((intiger/100000000)>=1 || (intiger/10000000)<=-1)
    {
        return 9;
    }
    if ((intiger/10000000)>=1 || (intiger/1000000)<=-1)
    {
        return 8;
    }
    if ((intiger/1000000)>=1 || (intiger/100000)<=-1)
    {
        return 7;
    }
    if ((intiger/100000)>=1 || (intiger/10000)<=-1)
    {
        return 6;
    }
    if ((intiger/10000)>=1 || (intiger/1000)<=-1)
    {
        return 5;
    }
    if ((intiger/1000)>=1 || (intiger/100)<=-1)
    {
        return 4;
    }
    if ((intiger/100)>=1 || (intiger/10)<=-1)
    {
        return 3;
    }
    if ((intiger/10)>=1 || (intiger/1)<=-1)
    {
        return 2;
    }
    return 1;
}

void generator()
{
    int random = 0;
    int type_int = 0;
    bool chunk_contains_dung_entrance = false;
    printf("running generator...\n");
    for (int i=0; i<=SIZE; i++)
    {
        for (int j=0; j<=SIZE; j++)
        {
            type_int = rand() % 3+0;
            switch (type_int)
            {
                case 0:
                    terrain_list[i][j] = game_tiles::STONE;
                    break;
                case 1:
                    terrain_list[i][j] = game_tiles::DIRT;
                    break;
                case 2:
                    random = rand() % 1000+0;
                    if (random == 51 && !(chunk_contains_dung_entrance))
                    {
                        terrain_list[i][j] = game_tiles::DUNG_ENTRANCE;
                        chunk_contains_dung_entrance = true;
                    }
                    else terrain_list[i][j] = game_tiles::TREE;
                    break;
            }
        }
    }
}

void save(bool with_player)
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
        fwrite(dungeon.dungeon_terrain_list, sizeof(dungeon.dungeon_terrain_list), 1, chunk);
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
void load(bool with_player)
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
            player.in_dungeon=temp;
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
            fread(dungeon.dungeon_terrain_list, sizeof(dungeon.dungeon_terrain_list), 1, chunk);
            fclose(chunk);
        }
        else
        {
            dungeon.generator();
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


void Player::interact(int key, Dungeon &dungeon)
{
    switch (key)
    {
        case SDLK_ESCAPE:
        {
            if ((!(in_menu == menu_types::EXIT)) && (!(in_menu == menu_types::CLOSED)))
            {
                in_menu = menu_types::CLOSED;
                break;
            }
            if (in_menu == menu_types::CLOSED)
            {
                in_menu = menu_types::EXIT;
                break;
            }
            if (in_menu == menu_types::EXIT)
            {
                in_menu = menu_types::CLOSED;
                break;
            }
            break;
        }
        case SDLK_m:
        {
            if (in_menu == menu_types::CLOSED) 
                in_menu = menu_types::ENERGY_MENU;
            else if (in_menu == menu_types::ENERGY_MENU) 
                in_menu = menu_types::CLOSED;

            break;
        }
        case SDLK_DOWN:
        case SDLK_s:
        {
            if (!(in_menu == menu_types::CLOSED))
            {
                text.menu_pos++;
                break;
            }

            if (in_dungeon)
            {
                if(y < SIZE-1 && !(dungeon.dungeon_terrain_list[x][y+1] == game_tiles::DUNG_WALL))
                {
                    if (running) 
                    {
                        y++;
                        energy--;
                    }
                    y++;
                    energy--;
                    break;
                }
                break;
            }
            
            if (running)
            {
                if (running) energy--;
                if (y < SIZE-1) y++;
                else {
                        y=0; 
                        save(false);
                        map_y++;
                        load(false);
                }
            }
            
            if (y < SIZE-1) y++;
            else {y=0; save(false); map_y++; load(false);}
            energy--;
            
            break;
        }
        case SDLK_w:
        case SDLK_UP:
        {
            if (!(in_menu == menu_types::CLOSED))
            {       
                text.menu_pos--;
                if (text.menu_pos < 0)
                {
                    text.menu_pos=0;
                }
                break;
            }

            if (in_dungeon)
            {
                if (y > 0 && !(dungeon.dungeon_terrain_list[x][y-1] == game_tiles::DUNG_WALL))
                {
                    if (running)
                    {
                            y--;
                            energy--;
                    }
                    y--;
                    energy--;
                    break;
                }
                break;
            }

            // IN MAIN WORLD
            if (running)
            {
                if (y > 0) y--;
                else {y=SIZE-1; save(false); map_y--;load(false);}
                energy--;
            }
            if (y > 0) y--;
            else {y=SIZE-1; save(false); map_y--;load(false);}
            
            energy--;
            break;
        }
        case SDLK_RIGHT:
        case SDLK_d:
        {
            if (!(in_menu == menu_types::CLOSED))
            {
                break;
            }
            if (in_dungeon)
            {
                if (running)
                {
                    if (x < SIZE-1 && !(dungeon.dungeon_terrain_list[x+1][y] == game_tiles::DUNG_WALL)) x++;
                }
                if (x < SIZE-1 && !(dungeon.dungeon_terrain_list[x+1][y] == game_tiles::DUNG_WALL)) x++;
            }
            else
            {
                if (running)
                {
                    if (x < SIZE-1) x++;
                    else if (!in_dungeon) {x=0; save(false); map_x++;load(false);}
                }
                if (x < SIZE-1) x++;
                else if (!in_dungeon) {x=0; save(false); map_x++;load(false);}
            }
            energy--;
            if (running) energy--;
            going_right = true;
            break;
        }
        case SDLK_LEFT:
        case SDLK_a:
        {
            if (!(in_menu == menu_types::CLOSED))
            {
                break;
            }
            if (in_dungeon)
            {
                if (running)
                {
                    if (x > 0 && !(dungeon.dungeon_terrain_list[x-1][y] == game_tiles::DUNG_WALL)) x--;
                }
                if (x > 0 && !(dungeon.dungeon_terrain_list[x-1][y] == game_tiles::DUNG_WALL)) x--;
            }
            else
            {
                if (running)
                {
                    if (x > 0) x--;
                    else if (!in_dungeon) {x=SIZE-1; save(false); map_x--;load(false);}
                }
                if (x > 0) x--;
                else if (!in_dungeon) {x=SIZE-1; save(false); map_x--;load(false);}
            }
            energy--;
            if (running) energy--;
            going_right = false;
            break;
        }
        case SDLK_r:
        {  
            if (!running)
            {
                running = true;
            }
            else if (running)
            {
                running = false;
            }
            break;
        }
        case SDLK_RETURN:
        case SDLK_e:
        {
            if (!(in_menu == menu_types::CLOSED))
            {
                text.interact(); 
                break;
            }

            if (screen_list[x][y] == game_tiles::DUNG_EXIT)
            {
                in_dungeon = false;
            }
            else if (screen_list[x][y] == game_tiles::DUNG_ENTRANCE)
            {
                in_dungeon = true;
                load(false);
            }
            break;
        }
    }
}

void update_screen_list(Dungeon& dungeon)
{
    if (player.in_dungeon)
    {
        for (int i=0; i < SIZE; i++)
        {
            for (int j=0; j < SIZE; j++)
            {
                screen_list[i][j] = dungeon.dungeon_terrain_list[i][j];
            }
        } 
    }
    else
    {
        for (int i=0; i < SIZE; i++)
        {
            for (int j=0; j < SIZE; j++)
            {
                screen_list[i][j] = terrain_list[i][j];
            }
        }
    }
}

void draw(textures texts, Dungeon& dungeon)
{
    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;
    int game_size;
    SDL_GetWindowSize(main_window, &WINDOW_WIDTH, &WINDOW_HEIGHT); 
    int TILE_SIZE = 16;
    if (WINDOW_WIDTH < WINDOW_HEIGHT)
    {
        game_size = WINDOW_WIDTH;
        TILE_SIZE = WINDOW_WIDTH/(SIZE+1);
    }
    if (WINDOW_HEIGHT <= WINDOW_WIDTH)
    {
        game_size = WINDOW_HEIGHT;
        TILE_SIZE = WINDOW_HEIGHT/(SIZE+1);
    }
    for (int i=0; i<(SIZE); i++)
    {
        for (int j=0; j<SIZE; j++)
        {
            int x = i*TILE_SIZE;
            int y = j*TILE_SIZE;
            SDL_Rect screen_rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
            SDL_Rect img_rect = {x, y, TILE_SIZE, TILE_SIZE};
            switch (screen_list[i][j])
            {
                case game_tiles::STONE:    
                    SDL_RenderCopy(renderer, texts.stone, &screen_rect, &img_rect);
                    break;
                case game_tiles::DIRT:    
                    SDL_RenderCopy(renderer, texts.dirt, &screen_rect, &img_rect);
                    break;
                case game_tiles::TREE:    
                    SDL_RenderCopy(renderer, texts.tree, &screen_rect, &img_rect);
                    break;
                case game_tiles::DUNG_ENTRANCE:
                    SDL_RenderCopy(renderer, texts.dung_entrance, &screen_rect, &img_rect);
                    break;
                case game_tiles::DUNG_EXIT:
                    SDL_RenderCopy(renderer, texts.dung_exit, &screen_rect, &img_rect);
                    break;
                case game_tiles::DUNG_WALL:
                    SDL_RenderCopy(renderer, texts.dung_wall, &screen_rect, &img_rect);
                    break;
                case game_tiles::DUNG_FLOOR:
                    SDL_RenderCopy(renderer, texts.dung_floor, &screen_rect, &img_rect);
            }
        }
    }  
    int px = player.x*(TILE_SIZE);
    int py = player.y*(TILE_SIZE);
    SDL_Rect screen_rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_Rect img_rect = {px, py, TILE_SIZE, TILE_SIZE};
    if (player.going_right) SDL_RenderCopy(renderer, texts.playerr, &screen_rect, &img_rect);
    else SDL_RenderCopy(renderer, texts.playerl, &screen_rect, &img_rect);
    
    int icon_size = game_size/10;
    if (player.running)
    {
        SDL_Rect running_icon_rect = {(game_size-(icon_size*2)), 0, icon_size, icon_size};
        SDL_RenderCopy(renderer, texts.run_icon, &screen_rect, &running_icon_rect);
    }

    if (player.energy > 1000) player.energy = 1000;   

    SDL_Texture* text_energy_sdl;
    char text_energy[20];
    sprintf(text_energy, "Energy: %d", player.energy);
    if (player.energy > 100) text_energy_sdl = text.create_font(text_energy, false);
    else if (player.energy <= 100) text_energy_sdl = text.create_font(text_energy, true);

    
    SDL_Texture* text_x_sdl;
    SDL_Texture* text_y_sdl;
    char text_y[20];
    char text_x[20];
    sprintf(text_y, "Y: %d", player.y+(player.map_y*SIZE));
    sprintf(text_x, "X: %d", player.x+(player.map_x*SIZE));
    text_y_sdl = text.create_font(text_y, false);
    text_x_sdl = text.create_font(text_x, false);
    int single_letter_size = game_size/25;

    int energy_number_lenght = get_intiger_lenght(player.energy);
    int x_lenght = get_intiger_lenght(player.x+(player.map_x*SIZE));
    int y_lenght = get_intiger_lenght(player.y+(player.map_y*SIZE));
    //printf("%d, %d, %d\n", player.x+(player.map_x*SIZE), get_intiger_lenght(player.x+(player.map_x*SIZE)), x_lenght);
    //fflush(stdout);

    SDL_Rect energy_text_rect = {10, 10, (energy_number_lenght+8)*single_letter_size, game_size/10};
    SDL_Rect energy_y_rect = {10, game_size/10, (y_lenght+3)*single_letter_size, game_size/10};
    SDL_Rect energy_x_rect = {10, game_size/10*2, (x_lenght+3)*single_letter_size, game_size/10};

    SDL_RenderCopy(renderer, text_x_sdl, NULL, &energy_x_rect);
    SDL_RenderCopy(renderer, text_y_sdl, NULL, &energy_y_rect);
    SDL_RenderCopy(renderer, text_energy_sdl, NULL, &energy_text_rect);
    
    SDL_DestroyTexture(text_energy_sdl);
    SDL_DestroyTexture(text_y_sdl);
    SDL_DestroyTexture(text_x_sdl);
    switch(in_menu)
    {
        case menu_types::CLOSED:
            break;
        case menu_types::ENERGY_MENU:
            text.show_menu(menu_types::ENERGY_MENU);
            break;
        case menu_types::EXIT:
            text.show_menu(menu_types::EXIT);
            break;
        case menu_types::HELP:
            text.show_menu(menu_types::HELP);
            break;
    }
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
    player.in_dungeon = false;
    load(true);
    save(true);
    int key;
    SDL_Event event;
    if (init_window()) return 1;
    if (text.load_font()) {printf("failed to load font");return 1;};
    textures Texture;
    Texture.load_textures();
    for (;;)
    {   
        clear_window();
        update_screen_list(dungeon);

        draw(Texture, dungeon);

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
                        player.interact(key, dungeon);
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
