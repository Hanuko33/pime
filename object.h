#ifndef OBJECT_H
#define OBJECT_H

#include <SDL2/SDL_render.h>
enum object_types
{
    OBJECT_NULL,
    OBJECT_TREE
};


struct object {
    int base_element_id;
    enum object_types type;
    SDL_Texture *texture;
};

#endif
