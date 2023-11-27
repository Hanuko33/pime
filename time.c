#include "time.h"




void update_time()
{
    if (game_time.seconds >= 60)
    {
        game_time.minutes+=(game_time.seconds/60);
        game_time.seconds=game_time.seconds % 60;
    }
    if (game_time.minutes >= 60)
    {
        game_time.hours+=(game_time.minutes/60);
        game_time.minutes=game_time.minutes % 60;
    }
    if (game_time.hours >= 24)
    {
        game_time.days+=(game_time.hours/24);
        game_time.hours=game_time.hours % 24;  
    }
}
