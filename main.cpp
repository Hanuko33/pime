#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "window.h"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL2_framerate.h>
#include "dungeon.h"
#include "texture.h"
#define SIZE 16 

using namespace std;

game_tiles screen_list[SIZE][SIZE];
game_tiles terrain_list[SIZE][SIZE];
Dungeon dungeon;

class Player
{
public:
    int map_y = 0;
    int map_x = 0;
    int y = 0;
    int x = 0;
    bool running = false;
    void interact(char key, Dungeon& dungeon);
    bool going_right = false;
    bool in_dungeon = false;
};

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

void save(bool with_player, Player* player)
{
    if (with_player)
    {
        int temp;
        FILE *file;
        char player_path[11];
        sprintf(player_path, "player.txt");
        file = fopen(player_path, "w");
        char to_write[13];
        sprintf(to_write, "%d\n%d\n%d\n%d\n%d\n", player->map_y, player->map_x, player->y, player->x, (int)player->in_dungeon);
        fwrite(to_write, sizeof(to_write), 1, file);
        fclose(file);
    }
    if (player->in_dungeon)
    {
        char filename[25];
        sprintf(filename, "%9d-%9ddung", player->map_x, player->map_y);
        FILE *chunk = fopen(filename, "w");
        fwrite(dungeon.dungeon_terrain_list, sizeof(dungeon.dungeon_terrain_list), 1, chunk);
        fclose(chunk);
    }
    else
    {
        char filename[20];
        sprintf(filename, "%9d-%9d", player->map_x, player->map_y);
        FILE *chunk = fopen(filename, "w");
        fwrite(terrain_list, sizeof(terrain_list), 1, chunk);
        fclose(chunk);
    }
    
}
void load(bool with_player, Player* player)
{
    if (with_player)
    {
        int temp;
        FILE *file;
        char player_path[11];
        sprintf(player_path, "player.txt");
        if ((file = fopen(player_path, "r")))
        {
            fscanf(file, "%d%d%d%d%d", &player->map_y, &player->map_x, &player->y, &player->x, &temp);
            player->in_dungeon=temp;
        }
        else 
        {
            file = fopen(player_path, "w");
        }
        fclose(file);
    }
    if (player->in_dungeon)
    {
        char filename[25];
        sprintf(filename, "%9d-%9ddung", player->map_x, player->map_y);
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
        char filename[20];
        sprintf(filename, "%9d-%9d", player->map_x, player->map_y);
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


void Player::interact(char key, Dungeon &dungeon)
{
    switch (key)
    {
        case 's':
        {
            if (in_dungeon)
            {
                if (running)
                {
                    if (y < SIZE-1 && !(dungeon.dungeon_terrain_list[x][y+1] == game_tiles::DUNG_WALL)) y++;
                }
                if (y < SIZE-1 && !(dungeon.dungeon_terrain_list[x][y+1] == game_tiles::DUNG_WALL)) y++;
            }
            else
            {
                if (running)
                {
                    if (y < SIZE-1) y++;
                    else {y=0; save(false, this); map_y++; load(false,this);}
                }
                if (y < SIZE-1) y++;
                else {y=0; save(false, this); map_y++; load(false,this);}
            }
            break;
        }
        case 'w':
        {
            if (in_dungeon)
            {
                if (running)
                {
                    if (y > 0 && !(dungeon.dungeon_terrain_list[x][y-1] == game_tiles::DUNG_WALL)) y--;
                }
                if (y > 0 && !(dungeon.dungeon_terrain_list[x][y-1] == game_tiles::DUNG_WALL)) y--;
            }
            else
            {
                if (running)
                {
                    if (y > 0) y--;
                    else {y=SIZE-1; save(false, this); map_y--;load(false,this);}
                }
                if (y > 0) y--;
                else {y=SIZE-1; save(false, this); map_y--;load(false,this);}
            }
            break;
        }
        case 'd':
        {
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
                    else if (!in_dungeon) {x=0; save(false, this); map_x++;load(false,this);}
                }
                if (x < SIZE-1) x++;
                else if (!in_dungeon) {x=0; save(false, this); map_x++;load(false,this);}
            }
            going_right = true;
            break;
        }
        case 'a':
        {
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
                    else if (!in_dungeon) {x=SIZE-1; save(false, this); map_x--;load(false,this);}
                }
                if (x > 0) x--;
                else if (!in_dungeon) {x=SIZE-1; save(false, this); map_x--;load(false,this);}
            }
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
            switch (screen_list[x][y])
            {
                case game_tiles::STONE:
                    name_of_terrain = "stone";
                    break;
                case game_tiles::DIRT:
                    name_of_terrain = "dirt";
                    break;
                case game_tiles::TREE:
                    name_of_terrain = "tree";
                    break;
                case game_tiles::DUNG_ENTRANCE:
                    name_of_terrain = "dungeon entrance";
                    break;
                case game_tiles::DUNG_EXIT:
                    name_of_terrain = "dungeon exit";
                    break;
                case game_tiles::DUNG_FLOOR:
                    name_of_terrain = "dungeon floor";
                    break;
                case game_tiles::DUNG_WALL:
                    name_of_terrain = "dungeon wall";
                    break;
            }
            if (name_of_terrain == "dungeon exit")
            {
                in_dungeon = false;
            }
            if (name_of_terrain == "dungeon entrance")
            {
                dungeon.generator();
                in_dungeon = true;
            }
            break;
        }
    }
}

void update_screen_list(Player player, Dungeon& dungeon)
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

void draw(Player player, textures texts, Dungeon& dungeon)
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
}

int main()
{
    srand (time(NULL));
    Player player;
    player.in_dungeon = false;
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
        update_screen_list(player, dungeon);
        draw(player, Texture, dungeon);
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
                        player.interact(key, dungeon);
                        break;
                }
            }
            if (event.type == SDL_KEYUP)
            {
                key = 0;
            }
            
        }
        // DEBUG x, y, map_x, map_y, running
        //printf("x: %d y: %d\n map_x: %d map_y: %d running: %d\n", player.x, player.y, player.map_x, player.map_y, player.running);                   

        SDL_RenderPresent(renderer);
        SDL_Delay(10);   
    }
}
