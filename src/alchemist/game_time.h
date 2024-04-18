#ifndef GAME_TIME_H
#define GAME_TIME_H

#include "el_list.h"

class Game_time
{
    int years;    
    int days;
    int hours;
    int minutes;
    int seconds;
    char cur_time[30];
 public:    
    void update_time(int sec);
    Game_time()
    {
        seconds=rand() % 60;
        minutes=rand() % 60;
        hours=rand() % 24;
        days=rand() % 360;
        years=1000 + rand() % 1000 ;
    }
    char * get_time();
};

extern Game_time game_time;
extern void (*callback_daily)();
#endif
