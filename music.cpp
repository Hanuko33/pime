#include "music.h"

struct Musics music;

int init_music()
{
    if ( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        SDL_Log("Mix_OpenAudio() failed: %s\n", SDL_GetError());
        return 1;
    }
    return 0;
}
void load_music()
{
    music.music_two = Mix_LoadWAV("music2.wav");
    music.music_one = Mix_LoadWAV("music.wav");
}
