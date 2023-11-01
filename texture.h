#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <stdlib.h>
#include <SDL2/SDL_image.h>

class textures
{
    public:
        SDL_Texture* playerr;
        SDL_Texture* playerl;
        SDL_Texture* stone;
        SDL_Texture* dirt;
        SDL_Texture* tree;
        SDL_Texture* dung_wall;
        SDL_Texture* dung_floor;
        SDL_Texture* dung_entrance;
        SDL_Texture* dung_exit;
        SDL_Texture* run_icon;
        void load_textures();
        SDL_Texture* load_texture(string texture_name);
};

#endif
