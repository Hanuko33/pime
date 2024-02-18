#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdlib.h>
#include <SDL2/SDL_image.h>
#include "tiles.h"

struct textures
{
        SDL_Texture* playerr;
        SDL_Texture* playerl;
        SDL_Texture* run_icon;
        SDL_Texture* sneak_icon;
};

extern SDL_Texture * tiles_textures[TILE_MAX_NUM];
extern SDL_Texture * items_textures[BASE_ELEMENTS];
extern SDL_Texture * ing_textures[ING_ELEMENTS];
extern SDL_Texture * objects_textures[OBJECT_MAX];
extern SDL_Texture * up_mask;
extern SDL_Texture * down_mask;
extern struct textures Texture;

void load_textures();

#endif
