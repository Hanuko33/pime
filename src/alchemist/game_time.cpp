#include "game_time.h"
#include <cstdio>

Game_time * game_time;

void Game_time::update_time(int sec)
{
    int start_day=years*360+days;

    seconds+=sec;
    if (seconds >= 60)
    {
        minutes+=seconds/60;
        seconds=seconds % 60;        
    }
    if (minutes >= 60)
    {
        hours+=minutes/60;
        minutes=minutes % 60;

    }
    if (hours >= 24)
    {
        days+=hours/24;
        hours=hours % 24;  

    }
    if (days >= 360)
    {
        years+=days / 360;
        days=days % 360;  
    }
    int delta_days=years*360+days - start_day;
    for (int d=0; d < delta_days; d++)
        callback_daily();
}
char * Game_time::get_time()
{
    sprintf(cur_time,"%d-%d_%02d:%02d:%02d", years, days, hours, minutes, seconds);
    return cur_time;
}
