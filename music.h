#ifndef MUSIC_H
#define MUSIC_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdlib.h>

struct Musics
{
    Mix_Chunk* music_one;
    Mix_Chunk* music_two;
};

int init_music();
void load_music();

extern struct Musics music;

#endif
