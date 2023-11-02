#include "menu.h"
#include "window.h"
#include  <SDL2/SDL2_gfxPrimitives.h>

int menu_pos = 0;
SDL_Color White = {255, 255, 255};
SDL_Color Red = {255, 0, 0};

enum menu_types in_menu = MENU_CLOSED;

void load(char with_player);
void save(char with_player);

void draw_text_to_menu(const char* text, int which_option, int options, char first)
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
    SDL_Texture* text_sdl = create_font(text_but_char, 0);
    SDL_Rect text_rect = {modx, mody+menu_opt_size*which_option, (modx2-modx), menu_opt_size};
    SDL_RenderCopy(renderer, text_sdl, NULL, &text_rect);
    SDL_DestroyTexture(text_sdl);
}


void show_menu(enum menu_types menu_type)
{
    switch(menu_type)
    {
        case MENU_CLOSED:
            break;
        case MENU_ENERGY:
            draw_text_to_menu("Regain 100 energy    ", 0, 2, 1);
            draw_text_to_menu("Set the energy to 1000", 1, 2, 0);
            break;

        case MENU_EXIT:
            draw_text_to_menu("Save & Exit          ", 0, 6, 1);
            draw_text_to_menu("Exit                 ", 1, 6, 0);
            draw_text_to_menu("Save                 ", 2, 6, 0);
            draw_text_to_menu("Load                 ", 3, 6, 0);
            draw_text_to_menu("Help                 ", 4, 6, 0);
            draw_text_to_menu("Cancel               ", 5, 6, 0);
            break;
        case MENU_HELP:
            draw_text_to_menu("ESC - game menu      ", 0, 6, 1);
            draw_text_to_menu("m - energy menu      ", 1, 6, 0);
            draw_text_to_menu("arrows - moves       ", 2, 6, 0);
            draw_text_to_menu("w, a, s, d - moves   ", 3, 6, 0);
            draw_text_to_menu("r - switch running   ", 4, 6, 0);
            draw_text_to_menu("e, ENTER - interact  ", 5, 6, 0);
            break;
    }
}

void interact()
{
    switch(in_menu)
    {
        case MENU_CLOSED:
            break;
        case MENU_ENERGY:
            switch(menu_pos)
            {
                case 0:
                    player.energy+=100;
                    break;
                case 1:
                    player.energy=1000;
                    break;
            }
            in_menu = MENU_CLOSED;
            break;
        case MENU_EXIT:
            switch(menu_pos)
            {
                case 0:
                    save(1);
                case 1:
                    SDL_Quit();
                    in_menu = MENU_CLOSED;
                    exit(0);
                    break;
                case 2:
                    save(1);
                    in_menu = MENU_CLOSED;
                    break;
                case 3:
                    load(1);
                    in_menu = MENU_CLOSED;
                    break;
                case 4: 
                    in_menu = MENU_HELP;
                    break;
                case 5:
                    in_menu = MENU_CLOSED;
                    break;
            }
            break;
        case MENU_HELP:
            in_menu = MENU_CLOSED;
            break;
    }
}
