#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdlib.h>

#ifndef STUB_SDL
#include <SDL2/SDL_image.h>
#endif

#include "tiles.h"

struct textures
{
        SDL_Texture* playerr;
        SDL_Texture* playerl;
        SDL_Texture* run_icon;
        SDL_Texture* sneak_icon;
};

extern SDL_Texture * tiles_textures[TILE_MAX_NUM];
extern SDL_Texture * items_textures[SOLID_ELEMENTS];
extern SDL_Texture * food_textures[FOOD_ELEMENTS];
extern SDL_Texture * liquid_textures[LIQUID_ELEMENTS];
extern SDL_Texture * gas_textures[GAS_ELEMENTS];
extern SDL_Texture * ing_textures[ING_ELEMENTS];
extern SDL_Texture * prod_textures[PROD_ELEMENTS];
extern SDL_Texture * tree_textures[3];
extern SDL_Texture * up_mask;
extern SDL_Texture * down_mask;
extern struct textures Texture;

void load_textures();

#endif
