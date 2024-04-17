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
        seconds=0;
        minutes=0;
        hours=0;
        days=0; // 24 hours
        years=0; //360 days
    }
    char * get_time();
};

extern Game_time game_time;
extern void (*callback_daily)();
#endif
