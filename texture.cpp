#include "texture.h"
#include "window.h"

SDL_Texture* textures::load_texture(const char * texture_name)
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
void textures::load_textures()
{
    dung_floor = load_texture("textures/dung_floor.png");
    dung_wall = load_texture("textures/dung_wall.png");
    dung_exit = load_texture("textures/dung_exit.png");
    dung_entrance = load_texture("textures/dung_entrance.png");
    stone = load_texture("textures/stone.png");
    dirt = load_texture("textures/dirt.png");
    tree = load_texture("textures/tree.png");
    playerr = load_texture("textures/playerr.png");
    playerl = load_texture("textures/playerl.png");
    run_icon = load_texture("textures/run_icon.png");
}
