#include "texture.h"
#include "window.h"

struct textures Texture;

SDL_Texture* load_texture(const char * texture_name)
{
    SDL_Texture* texture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(texture_name); 
    if (loadedSurface == NULL)
    {
        printf("Unable to load texture: %s error: %s\n", texture_name, SDL_GetError()); 
    }
    else 
    {    
        texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

        if (texture == NULL)
        {
            printf("Unable to create texture: %s error: %s\n", texture_name, SDL_GetError());
        }
        SDL_FreeSurface(loadedSurface);
    }

    return texture;
}

void load_textures()
{
    Texture.dung_floor = load_texture("textures/game_tiles/dungeons/dung_floor.png");
    Texture.dung_wall = load_texture("textures/game_tiles/dungeons/dung_wall.png");
    Texture.dung_door = load_texture("textures/game_tiles/dungeons/dung_door.png");
    Texture.stone = load_texture("textures/game_tiles/world/stone.png");
    Texture.dirt = load_texture("textures/game_tiles/world/dirt.png");
    Texture.tree = load_texture("textures/game_tiles/world/tree.png");
    Texture.playerr = load_texture("textures/player/playerr.png");
    Texture.playerl = load_texture("textures/player/playerl.png");
    Texture.run_icon = load_texture("textures/gui/run_icon.png");
    Texture.sand = load_texture("textures/game_tiles/world/sand.png");
    Texture.sandstone = load_texture("textures/game_tiles/world/sandstone.png");
    Texture.sweet_tree = load_texture("textures/game_tiles/world/sweet_tree.png");
    Texture.sweet_bush = load_texture("textures/game_tiles/world/sweet_bush.png");
    Texture.sweet_flower = load_texture("textures/game_tiles/world/sweet_flower.png");
    Texture.grass = load_texture("textures/game_tiles/world/grass.png");
    Texture.sweet_grass = load_texture("textures/game_tiles/world/sweet_grass.png");
    Texture.water = load_texture("textures/game_tiles/world/water.png");
    Texture.cave_floor = load_texture("textures/game_tiles/cave/cave_floor.png");
    Texture.cave_wall = load_texture("textures/game_tiles/cave/cave_wall.png");
    Texture.cave_door = load_texture("textures/game_tiles/cave/cave_door.png");
}
