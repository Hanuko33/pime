#include "text.h"
#include "window.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

void load(bool with_player);
void save(bool with_player);

#define FONT_NAME "nerdfont.ttf"
    
int Text::load_font()
{
     struct stat statbuf;
    int ret;
    ret = stat(FONT_NAME, &statbuf);
    if (ret) 
    {
        printf("load_font(%s): %s\n", FONT_NAME, strerror(errno));
        return 1;
    }
    font = TTF_OpenFont(FONT_NAME, 500);
    if (!(font)) return 1;
    else return 0;
}

SDL_Texture* Text::create_font(const char* text, bool warning)
{
    if (!(font)) abort();
    SDL_Surface* surface;
    if (!(warning))
    {
        surface = TTF_RenderText_Solid(font, text, White);
    }
    else
    {
        surface = TTF_RenderText_Solid(font, text, Red);
    }
    SDL_Texture* text_sdl = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return text_sdl;
}

void Text::draw_text_to_menu(char* text, int which_option, int options, bool first)
{   
    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;  
    
    int game_size; 

    SDL_GetWindowSize(main_window, &WINDOW_WIDTH, &WINDOW_HEIGHT);

    if (WINDOW_WIDTH < WINDOW_HEIGHT)
    {
        game_size = WINDOW_WIDTH;   
    }
    if (WINDOW_HEIGHT <= WINDOW_WIDTH)
    {
        game_size = WINDOW_HEIGHT;
    }
    if (pointer_y < 1)
    {
        pointer_y=1;
    }
    if (pointer_y > options)
    {
        pointer_y=options;
    }

    // menu_opt_size = single menu option size
    int menu_opt_size = game_size/15;
    int modx = (game_size/2)-(0.3*game_size);
    int mody = (game_size/2)-(menu_opt_size*(options/2));
    int modx2 = (game_size/2)+(0.3*game_size);
    int mody2 = (game_size/2)+(menu_opt_size*(options/2));
   
    if (first)
    {
        boxColor(renderer, modx, mody, modx2, mody2, color(0, 0, 0, 100));
        boxColor(renderer, modx, mody+((pointer_y*menu_opt_size)-menu_opt_size), modx2, mody+(pointer_y*menu_opt_size), color(100, 100, 100, 100));
    }
    char text_but_char[100];
    strcpy(text_but_char, text);
    SDL_Texture* text_sdl = create_font(text_but_char, false);
    SDL_Rect text_rect = {modx, mody+menu_opt_size*which_option, (modx2-modx), menu_opt_size};
    SDL_RenderCopy(renderer, text_sdl, NULL, &text_rect);
    SDL_DestroyTexture(text_sdl);

}


void Text::show_menu(menu_types menu_type)
{
    switch(menu_type)
    {
        case menu_types::CLOSED:
            break;
        case menu_types::ENERGY_MENU:
            draw_text_to_menu((char*)"Regain 100 energy    ", 0, 2, true);
            draw_text_to_menu((char*)"Set the energy to 400", 1, 2, false);
            break;

        case menu_types::EXIT:
            draw_text_to_menu((char*)"Exit & Save          ", 0, 4, true);
            draw_text_to_menu((char*)"Exit                 ", 1, 4, false);
            draw_text_to_menu((char*)"Save                 ", 2, 4, false);
            draw_text_to_menu((char*)"Load                 ", 3, 4, false);
            break;
    }
}

void Text::interact(menu_types in_menu)
{
    switch(in_menu)
    {
        case menu_types::CLOSED:
            break;
        case menu_types::ENERGY_MENU:
            switch(pointer_y)
            {
                case 1:
                    player.energy+=100;
                    break;
                case 2:
                    player.energy=400;
                    break;
            }
            break;
        case menu_types::EXIT:
            switch(pointer_y)
            {
                case 1:
                    save(true);
                case 2:
                    SDL_Quit();
                    exit(0);
                    break;
                case 3:
                    save(true);
                    break;
                case 4:
                    load(true);
                    break;
            }
            break;
    }
    
}
