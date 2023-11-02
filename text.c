#include "text.h"
#include "window.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

TTF_Font* font;

#define FONT_NAME "nerdfont.ttf"


int load_font()
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

SDL_Texture* create_font(const char* text, char warning)
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

void write_text(int x, int y, const char * text, SDL_Color color)
{
    SDL_Surface* surface;
	surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* text_sdl = SDL_CreateTextureFromSurface(renderer, surface);

    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;
    SDL_GetWindowSize(main_window, &WINDOW_WIDTH, &WINDOW_HEIGHT); 

    int game_size;
    
	if (WINDOW_WIDTH < WINDOW_HEIGHT)
    {
        game_size = WINDOW_WIDTH;
	} else {
		game_size = WINDOW_HEIGHT;
	}
    
	int t_size = strlen(text) * game_size/25;

    SDL_Rect rect = {x, y, t_size, game_size/10};
    SDL_RenderCopy(renderer, text_sdl, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(text_sdl);
}


