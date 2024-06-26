#include "texture.h"
#include "alchemist/elements.h"
#include "tiles.h"
#include "window.h"
#include <SDL2/SDL_blendmode.h>

struct textures Texture;
SDL_Texture * tiles_textures[TILE_MAX_NUM];
SDL_Texture * items_textures[SOLID_ELEMENTS];
SDL_Texture * food_textures[FOOD_ELEMENTS];
SDL_Texture * liquid_textures[LIQUID_ELEMENTS];
SDL_Texture * gas_textures[GAS_ELEMENTS];
SDL_Texture * ing_textures[ING_ELEMENTS];
SDL_Texture * prod_textures[PROD_ELEMENTS];
SDL_Texture * tree_textures[3];

SDL_Texture* load_texture(const char * texture_name)
{
    SDL_Texture* texture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(texture_name); 
    if (loadedSurface == NULL)
    {
        printf("Unable to load texture: %s error: %s\n", texture_name, SDL_GetError()); 
            exit(0);
    }
    else 
    {    
        texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

        if (texture == NULL)
        {
            printf("Unable to create texture: %s error: %s\n", texture_name, SDL_GetError());
            exit(0);
        }
        SDL_FreeSurface(loadedSurface);
    }

    return texture;
}

void add_tile_texture(enum game_tiles id, const char * file)
{
    tiles_textures[id]=load_texture(file);
};

void add_items_texture(int id, const char * file)
{
    items_textures[id]=load_texture(file);
};

void add_food_texture(int id, const char * file)
{
    food_textures[id]=load_texture(file);
};

void add_liquid_texture(int id, const char * file)
{
    liquid_textures[id]=load_texture(file);
};

void add_gas_texture(int id, const char * file)
{
    gas_textures[id]=load_texture(file);
};

void add_ing_texture(int id, const char * file)
{
    ing_textures[id]=load_texture(file);
};

void add_prod_texture(int id, const char * file)
{
    prod_textures[id]=load_texture(file);
};


void load_textures()
{
    Texture.playerr = load_texture("textures/player/playerr.png");
    Texture.playerl = load_texture("textures/player/playerl.png");
    Texture.run_icon = load_texture("textures/gui/run_icon.png");
    Texture.sneak_icon = load_texture("textures/gui/sneak_icon.png");

    add_tile_texture(TILE_AIR, "textures/game_tiles/air.png");
    add_tile_texture(TILE_STONE, "textures/game_tiles/stone.png");
    add_tile_texture(TILE_DIRT, "textures/game_tiles/dirt.png");
//    add_tile_texture(TILE_TREE, "textures/game_tiles/tree.png");
    add_tile_texture(TILE_SAND,"textures/game_tiles/sand.png");
    add_tile_texture(TILE_SANDSTONE,"textures/game_tiles/sandstone.png");
//    add_tile_texture(TILE_SWEET_TREE, "textures/game_tiles/sweet_tree.png");
//    add_tile_texture(TILE_SWEET_BUSH,"textures/game_tiles/sweet_bush.png");
//    add_tile_texture(TILE_SWEET_FLOWER,"textures/game_tiles/sweet_flower.png");
    add_tile_texture(TILE_GRASS,"textures/game_tiles/grass.png");
    add_tile_texture(TILE_SWEET_GRASS,"textures/game_tiles/sweet_grass.png");
    add_tile_texture(TILE_WATER,"textures/game_tiles/water.png");

    add_items_texture(ID_STONE, "textures/items/solid/stone.png");
    add_items_texture(ID_LOG, "textures/items/solid/log.png");
    add_items_texture(ID_SAND, "textures/items/solid/sand.png");
    add_items_texture(ID_STICK, "textures/items/solid/stick.png");

    add_food_texture(ID_PUMPKIN, "textures/items/food/pumpkin.png");
    add_food_texture(ID_WATERMELON, "textures/items/food/watermelon.png");
    add_food_texture(ID_CHERRY, "textures/items/food/cherry.png");

    add_liquid_texture(0, "textures/items/liquid/water.png");

    add_gas_texture(0, "textures/items/gas/gas.png");

    add_ing_texture(ING_AXE_BLADE, "textures/items/ingredients/axe_blade.png");
    add_ing_texture(ING_AXE_HANDLE, "textures/items/ingredients/axe_handle.png");
    add_ing_texture(ING_KNIFE_BLADE, "textures/items/ingredients/knife_blade.png");
    add_ing_texture(ING_KNIFE_HANDLE, "textures/items/ingredients/knife_handle.png");
    
    add_prod_texture(PROD_AXE, "textures/items/products/axe.png");
    add_prod_texture(PROD_KNIFE, "textures/items/products/knife.png");

    tree_textures[0] = load_texture("textures/objects/tree/tree.png");
    tree_textures[1] = load_texture("textures/objects/tree/tree1.png");
    tree_textures[2] = load_texture("textures/objects/tree/tree2.png");
}
