#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "window.h"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL2_framerate.h>
#include <SDL2/SDL_image.h>
#include <string>
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
    bool going_right;
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
                if (x < SIZE-1) x++;
                else {x=0; save(false, this); map_x++;load(false,this);}
            }
            if (x < SIZE-1) x++;
            else {x=0; save(false, this); map_x++;load(false,this);}
            going_right = true;
            break;
        }
        case 'a':
        {
            if (running)
            {
                if (x > 0) x--;
                else {x=SIZE-1; save(false, this); map_x--;load(false,this);}
            }
            if (x > 0) x--;
            else {x=SIZE-1; save(false, this); map_x--;load(false,this);}
            going_right = false;
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

class textures
{
    public:
        SDL_Texture* playerr;
        SDL_Texture* playerl;
        SDL_Texture* stone;
        SDL_Texture* dirt;
        SDL_Texture* tree;
        void load_textures();
        SDL_Texture* load_texture(string texture_name);
};
void draw(Player player, textures texts)
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
    if (WINDOW_HEIGHT < WINDOW_WIDTH)
    {
        game_size = WINDOW_HEIGHT;
        TILE_SIZE = WINDOW_HEIGHT/(SIZE+1);
    }
    else
    {
        game_size = WINDOW_WIDTH;
        TILE_SIZE = WINDOW_WIDTH/(SIZE+1);
    }
    for (int i=0; i<(SIZE); i++)
    {
        for (int j=0; j<=SIZE; j++)
        {
            int x = i*TILE_SIZE;
            int y = j*TILE_SIZE;
            SDL_Rect screen_rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
            SDL_Rect img_rect = {x, y, TILE_SIZE, TILE_SIZE};
            switch (terrain_list[i][j])
            {
                case STONE:    
                    SDL_RenderCopy(renderer, texts.stone, &screen_rect, &img_rect);
                    break;
                case DIRT:    
                    SDL_RenderCopy(renderer, texts.dirt, &screen_rect, &img_rect);
                    break;
                case TREE:    
                    SDL_RenderCopy(renderer, texts.tree, &screen_rect, &img_rect);
                    break;
            } 
        }
    }  
    int px = player.x*(TILE_SIZE);
    int py = player.y*(TILE_SIZE);
    SDL_Rect screen_rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_Rect img_rect = {px, py, TILE_SIZE, TILE_SIZE};
    if (player.going_right) SDL_RenderCopy(renderer, texts.playerr, &screen_rect, &img_rect);
    else SDL_RenderCopy(renderer, texts.playerl, &screen_rect, &img_rect);
}

SDL_Texture* textures::load_texture(string texture_name)
{
    SDL_Texture* texture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(texture_name.c_str()); 
    if (loadedSurface == NULL)
    {
        printf("Unable to load texture: %s error: %s\n", texture_name.c_str(), SDL_GetError()); 
    }
    else 
    {    
        texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

        if (texture == NULL)
        {
            printf("Unable to create texture: %s error: %s\n", texture_name.c_str(), SDL_GetError());
        }
        SDL_FreeSurface(loadedSurface);
    }

    return texture;
}

void textures::load_textures()
{
    stone = load_texture("stone.png");
    dirt = load_texture("dirt.png");
    tree = load_texture("tree.png");
    playerr = load_texture("playerr.png");
    playerl = load_texture("playerl.png");

    if (playerl == NULL)
    {
        printf("Failed to load image: playerl.png error: %s\n", SDL_GetError());
    }
    if (playerr == NULL)
    {
        printf("Failed to load image: playerr.png error: %s\n", SDL_GetError());
    }
    if (stone == NULL)
    {
        printf("Failed to load image: stone.png error: %s\n", SDL_GetError());
    }
    if (dirt == NULL)
    {
        printf("Failed to load image: dirt.png error: %s\n", SDL_GetError());
    }
    if (tree == NULL)
    {
        printf("Failed to load image: tree.png error: %s\n", SDL_GetError());
    }
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
    textures Texture;
    Texture.load_textures();
    for (;;)
    {   
        clear_window();
        draw(player, Texture);
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
