#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "window.h"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL2_framerate.h>
#define SIZE 32 

using namespace std;

enum terrain
{
    STONE,
    DIRT,
    TREE
};

terrain terrain_list[SIZE][SIZE];

class Player
{
public:
    int map_y = 0;
    int map_x = 0;
    int y = 0;
    int x = 0;
    bool running = false;
    void interact(char key);
};

void generator()
{
    int type_int = 0;
    printf("running generator...\n");
    for (int i=0; i<=SIZE; i++)
    {
        for (int j=0; j<=SIZE; j++)
        {
            type_int = rand() % 3+0;
            terrain_list[i][j] = (terrain)type_int;
        }
    }
}

void save(bool with_player, Player* player)
{
    if (with_player)
    {
        FILE *file;
        char player_path[11];
        sprintf(player_path, "player.txt");
        file = fopen(player_path, "w");
        char to_write[9];
        sprintf(to_write, "%d\n%d\n%d\n%d\n", player->map_y, player->map_x, player->y, player->x);
        fwrite(to_write, sizeof(to_write), 1, file);
        fclose(file);
    }
    char filename[10];
    sprintf(filename, "%4d-%4d", player->map_x, player->map_y);
    FILE *chunk = fopen(filename, "w");
    fwrite(terrain_list, sizeof(terrain_list), 1, chunk);
    fclose(chunk);
}
void load(bool with_player, Player* player)
{
    if (with_player)
    {
        FILE *file;
        char player_path[11];
        sprintf(player_path, "player.txt");
        if ((file = fopen(player_path, "r")))
        {
            fscanf(file, "%d%d%d%d", &player->map_y, &player->map_x, &player->y, &player->x);
        }
        else 
        {
            file = fopen(player_path, "w");
        }
        fclose(file);
    }
    char filename[10];
    sprintf(filename, "%4d-%4d", player->map_x, player->map_y);
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
void Player::interact(char key)
{
    switch (key)
    {
        case 's':
        {
            if (running)
            {
                if (y < SIZE) y++;
                else {y=0; save(false, this); map_y++; load(false,this);}
            }
            if (y < SIZE) y++;
            else {y=0; save(false, this); map_y++; load(false,this);}
            break;
        }
        case 'w':
        {
            if (running)
            {
                if (y > 0) y--;
                else {y=SIZE; save(false, this); map_y--;load(false,this);}
            }
            if (y > 0) y--;
            else {y=SIZE; save(false, this); map_y--;load(false,this);}
            break;
        }
        case 'd':
        {
            if (running)
            {
                if (x < SIZE) x++;
                else {x=0; save(false, this); map_x++;load(false,this);}
            }
            if (x < SIZE) x++;
            else {x=0; save(false, this); map_x++;load(false,this);}
            break;
        }
        case 'a':
        {
            if (running)
            {
                if (x > 0) x--;
                else {x=SIZE; save(false, this); map_x--;load(false,this);}
            }
            if (x > 0) x--;
            else {x=SIZE; save(false, this); map_x--;load(false,this);}
            break;
        }
        case 'k':
        {
            save(true, this);
            break;
        }
        case 'l':
        {
            load(true, this);
            break;
        }
        case 'r':
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
        case 'e':
        {
            string name_of_terrain;
            switch (terrain_list[x][y])
            {
                case STONE:
                    name_of_terrain = "stone";
                    break;
                case DIRT:
                    name_of_terrain = "dirt";
                    break;
                case TREE:
                    name_of_terrain = "tree";
                    break;
            }
            break;
        }
    }
}

void draw(Player player)
{

    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;
    SDL_GetWindowSize(main_window, &WINDOW_WIDTH, &WINDOW_HEIGHT); 
    int TILE_SIZE = 16;
    if (WINDOW_WIDTH < WINDOW_HEIGHT)
    {
        TILE_SIZE = WINDOW_WIDTH/(SIZE+1);
    }
    if (WINDOW_HEIGHT < WINDOW_WIDTH)
    {
        TILE_SIZE = WINDOW_HEIGHT/(SIZE+1);
    }
    else
    {
        TILE_SIZE = WINDOW_WIDTH/(SIZE+1);
    }
    for (int i=0; i<=(SIZE); i++)
    {
        for (int j=0; j<=SIZE; j++)
        {
            int x = i*TILE_SIZE;
            int y = j*TILE_SIZE;
            switch (terrain_list[i][j])
            {
                case STONE:    
                    boxColor(renderer, x, y, x+TILE_SIZE, y+TILE_SIZE, color(100, 100, 100, 255));
                    break;
                case DIRT:    
                    boxColor(renderer, x, y, x+TILE_SIZE, y+TILE_SIZE, color(255, 128, 0, 255));
                    break;
                case TREE:    
                    boxColor(renderer, x, y, x+TILE_SIZE, y+TILE_SIZE, color(200, 100, 0, 255));
                    break;
            } 
        }
    }  
    int px = player.x*(TILE_SIZE);
    int py = player.y*(TILE_SIZE);
    boxColor(renderer, px, py, px+TILE_SIZE-1, py+TILE_SIZE, color(255,0,0,255));
}

int main()
{
    srand (time(NULL));
    Player player;
    load(true, &player);
    save(true, &player);
    char key;
    SDL_Event event;
    if (init_window()) return 1;
    for (;;)
    {   
        clear_window();
        draw(player);
        while (SDL_PollEvent(&event))
        {
            if (event.type==SDL_QUIT) {SDL_Quit(); return 0;};
            if(event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym) {
                    case SDLK_w:
                    case SDLK_a:
                    case SDLK_s:
                    case SDLK_d:
                    case SDLK_e:
                    case SDLK_k:
                    case SDLK_l:
                    case SDLK_r:
                        key = event.key.keysym.sym;
                        player.interact(key);
                        break;
                }
            }
            if (event.type == SDL_KEYUP)
            {
                key = 0;
            }
            
        }
        // DEBUG x, y, map_x, map_y, running
        printf("x: %d y: %d\n map_x: %d map_y: %d running: %d\n", player.x, player.y, player.map_x, player.map_y, player.running);                   

        SDL_RenderPresent(renderer);
        SDL_Delay(10);   
    }
}
