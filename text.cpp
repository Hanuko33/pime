#include "text.h"
#include "window.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

void load(bool with_player);
void save(bool with_player);

#define FONT_NAME "nerdfont.ttf"
menu_types in_menu = menu_types::CLOSED;
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
    SDL_Surface* surface;
    if (warning)
    {
        surface = TTF_RenderText_Solid(font, text, Red);
    }
    else
    {
        surface = TTF_RenderText_Solid(font, text, White);
    }
    SDL_Texture* text_sdl = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return text_sdl;
}

void Text::draw_text_to_menu(const char* text, int which_option, int options, bool first)
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
    if (menu_pos >= options)
    {
        menu_pos=options-1;
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
        boxColor(renderer, modx, mody+(((menu_pos+1)*menu_opt_size)-(menu_opt_size)), modx2, mody+((menu_pos+1)*menu_opt_size), color(150, 150, 150, 100));
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
            draw_text_to_menu("Regain 100 energy    ", 0, 2, true);
            draw_text_to_menu("Set the energy to 1000", 1, 2, false);
            break;

        case menu_types::EXIT:
            draw_text_to_menu("Save & Exit          ", 0, 6, true);
            draw_text_to_menu("Exit                 ", 1, 6, false);
            draw_text_to_menu("Save                 ", 2, 6, false);
            draw_text_to_menu("Load                 ", 3, 6, false);
            draw_text_to_menu("Help                 ", 4, 6, false);
            draw_text_to_menu("Cancel               ", 5, 6, false);
            break;
        case menu_types::HELP:
            draw_text_to_menu("ESC - game menu      ", 0, 6, true);
            draw_text_to_menu("m - energy menu      ", 1, 6, false);
            draw_text_to_menu("arrows - moves       ", 2, 6, false);
            draw_text_to_menu("w, a, s, d - moves   ", 3, 6, false);
            draw_text_to_menu("r - switch running   ", 4, 6, false);
            draw_text_to_menu("e, ENTER - interact  ", 5, 6, false);
            break;
    }
}

void Text::interact()
{
    switch(in_menu)
    {
        case menu_types::CLOSED:
            break;
        case menu_types::ENERGY_MENU:
            switch(menu_pos)
            {
                case 0:
                    player.energy+=100;
                    break;
                case 1:
                    player.energy=1000;
                    break;
            }
            in_menu = menu_types::CLOSED;
            break;
        case menu_types::EXIT:
            switch(menu_pos)
            {
                case 0:
                    save(true);
                case 1:
                    SDL_Quit();
                    in_menu = menu_types::CLOSED;
                    exit(0);
                    break;
                case 2:
                    save(true);
                    in_menu = menu_types::CLOSED;
                    break;
                case 3:
                    load(true);
                    in_menu = menu_types::CLOSED;
                    break;
                case 4: 
                    in_menu = menu_types::HELP;
                    break;
                case 5:
                    in_menu = menu_types::CLOSED;
                    break;
            }
            break;
        case menu_types::HELP:
            in_menu = menu_types::CLOSED;
            break;
    }
}
