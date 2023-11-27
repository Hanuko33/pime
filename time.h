#ifndef GAME_TIME_H
#define GAME_TIME_H

typedef struct game_time
{
    int days;
    int hours;
    int minutes;
    int seconds;
} Game_time;

extern Game_time game_time;
void update_time();

#endif
