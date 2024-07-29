#include "texture.h"
#include "alchemist/elements.h"
#include "tiles.h"
#include "window.h"
#include <SDL2/SDL_blendmode.h>

struct textures Texture;
SDL_Texture * tiles_textures[TILE_MAX_NUM];
SDL_Texture * items_textures[BASE_ELEMENTS];
SDL_Texture * ing_textures[ING_ELEMENTS];
SDL_Texture * prod_textures[PROD_ELEMENTS];
SDL_Texture * being_textures[BEINGS];
SDL_Texture * animall_textures[ANIMALS];
SDL_Texture * animalr_textures[ANIMALS];
SDL_Texture * plant_textures[PLANTS];
SDL_Texture * grown_plant_textures[PLANTS];
SDL_Texture * object_textures[OBJECTS];

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

    // SOLID START HERE
    add_items_texture(ID_STONE, "textures/items/solid/stone.png");
    add_items_texture(ID_LOG, "textures/items/solid/log.png");
    add_items_texture(ID_LOG1, "textures/items/solid/log1.png");
    add_items_texture(ID_LOG2, "textures/items/solid/log2.png");
    add_items_texture(ID_SAND, "textures/items/solid/sand.png");
    add_items_texture(ID_STICK, "textures/items/solid/stick.png");
    add_items_texture(ID_STRAWBERRY_SEEDS, "textures/items/solid/strawberry_seeds.png");
    add_items_texture(ID_ACORN, "textures/items/solid/acorn.png");
    add_items_texture(ID_ACORN1, "textures/items/solid/acorn1.png");
    add_items_texture(ID_ACORN2, "textures/items/solid/acorn2.png");

    // LIQUID START HERE
    add_items_texture(ID_WATER, "textures/items/liquid/water.png");

    // GAS START HERE
    add_items_texture(ID_HELIUM, "textures/items/gas/gas.png");

    // FOOD START HERE
    add_items_texture(ID_PUMPKIN, "textures/items/food/pumpkin.png");
    add_items_texture(ID_WATERMELON, "textures/items/food/watermelon.png");
    add_items_texture(ID_CHERRY, "textures/items/food/cherry.png");
    add_items_texture(ID_STRAWBERRY, "textures/items/food/strawberry.png");
    add_items_texture(ID_RAW_HAM, "textures/items/food/raw_ham.png");

    add_ing_texture(ING_AXE_BLADE, "textures/items/ingredients/axe_blade.png");
    add_ing_texture(ING_AXE_HANDLE, "textures/items/ingredients/axe_handle.png");
    add_ing_texture(ING_KNIFE_BLADE, "textures/items/ingredients/knife_blade.png");
    add_ing_texture(ING_KNIFE_HANDLE, "textures/items/ingredients/knife_handle.png");
    
    add_prod_texture(PROD_AXE, "textures/items/products/axe.png");
    add_prod_texture(PROD_KNIFE, "textures/items/products/knife.png");


    plant_textures[PLANTID_strawberry] = load_texture("textures/plants/strawberry.png");

    plant_textures[PLANTID_tree] = load_texture("textures/plants/tree/tree.png");
    plant_textures[PLANTID_tree1] = load_texture("textures/plants/tree/tree1.png");
    plant_textures[PLANTID_tree2] = load_texture("textures/plants/tree/tree2.png");

    grown_plant_textures[PLANTID_strawberry] = load_texture("textures/plants/strawberry_grown.png");

    grown_plant_textures[PLANTID_tree] = load_texture("textures/plants/tree/tree_grown.png");
    grown_plant_textures[PLANTID_tree1] = load_texture("textures/plants/tree/tree1_grown.png");
    grown_plant_textures[PLANTID_tree2] = load_texture("textures/plants/tree/tree2_grown.png");

    animall_textures[ANIMALID_pig] = load_texture("textures/animals/pigl.png");
    animalr_textures[ANIMALID_pig] = load_texture("textures/animals/pigr.png");

    object_textures[OBJECT_wall] = load_texture("textures/objects/wall.png");
}
