#include "window.h"

SDL_Renderer *renderer;
SDL_Window *main_window;

int  init_window()
{
	Uint32 flags;
    flags = SDL_WINDOW_HIDDEN;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) { fprintf(stderr, "\nUnable to initialize SDL:  %s\n", SDL_GetError()); }

    if (SDL_CreateWindowAndRenderer(0, 0, flags, &main_window, &renderer) < 0) {
        SDL_Log("SDL_CreateWindowAndRenderer() failed: %s\n", SDL_GetError());
        return 1;
	}
    SDL_SetWindowTitle(main_window, "game_window");
    SDL_SetWindowSize(main_window, WINDOW_WIDTH, WINDOW_HEIGHT);
//    app_surface = SDL_GetWindowSurface(main_window);
    SDL_SetWindowPosition(main_window, 0,0);

    SDL_ShowWindow(main_window);
	return 0;
}

void clear_window()
{
     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

unsigned int color(int r, int g, int b, int a) {
	return (a << 24) | (b << 16) | (g << 8) | r;
}

