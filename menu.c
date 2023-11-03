#include "menu.h"
#include "window.h"
#include  <SDL2/SDL2_gfxPrimitives.h>


struct menu_struct menu_main;
struct menu_struct menu_energy;
struct menu_struct menu_help;
struct menu_struct * current_menu;

void load(char with_player);
void save(char with_player);

void create_menu(struct menu_struct * m, int opt)
{
    m->options=opt;
    m->entries=calloc(opt, sizeof(char*));
    m->added=0;
    m->actions=calloc(opt, sizeof(enum menu_actions));
}

void add_entry(struct menu_struct *m, const char * e, enum menu_actions a)
{
    m->entries[m->added] = e;
    m->actions[m->added] = a;
    m->added++;
}

void show_menu()
{
    if (!current_menu) return;

    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;  
    int i;
    int game_size; 

    SDL_GetWindowSize(main_window, &WINDOW_WIDTH, &WINDOW_HEIGHT);

    if (WINDOW_WIDTH < WINDOW_HEIGHT) 
        game_size = WINDOW_WIDTH;   
    else 
        game_size = WINDOW_HEIGHT;

    int menu_opt_size = game_size/10;
    int modx = (game_size/2)-(0.3*game_size);
    int mody = (game_size/2)-(menu_opt_size*(current_menu->options/2));
    int modx2 = (game_size/2)+(0.3*game_size);
    int mody2 = (game_size/2)+(menu_opt_size*(current_menu->options/2));
   
    boxColor(renderer, modx, mody, modx2, mody2, color(0, 0, 0, 100));
    boxColor(renderer, modx, mody+(((current_menu->menu_pos+1)*menu_opt_size)-(menu_opt_size)), 
            modx2, mody+((current_menu->menu_pos+1)*menu_opt_size), color(150, 150, 150, 100));

    for (i=0; i < current_menu->options; i++)
        write_text(modx, mody + i * menu_opt_size, current_menu->entries[i], White, game_size/37, menu_opt_size);
}

void create_menus()
{
    create_menu(&menu_main, 6);
        add_entry(&menu_main, "Save & Exit", MENU_SAVE_EXIT);
        add_entry(&menu_main, "Exit", MENU_EXIT);
        add_entry(&menu_main, "Save", MENU_SAVE);
        add_entry(&menu_main, "Load", MENU_LOAD);
        add_entry(&menu_main, "Help", MENU_HELP);
        add_entry(&menu_main, "Cancel", MENU_CANCEL);

    create_menu(&menu_energy, 2);
        add_entry(&menu_energy, "Regain 100 energy", MENU_REGAIN);
        add_entry(&menu_energy, "Set the energy to 1000", MENU_BOOST);

    create_menu(&menu_help, 6);
        add_entry(&menu_help, "ESC - game menu", MENU_CANCEL);
        add_entry(&menu_help, "m - energy", MENU_CANCEL);
        add_entry(&menu_help, "arrows - moves", MENU_CANCEL);
        add_entry(&menu_help, "w, a, s, d - moves", MENU_CANCEL);
        add_entry(&menu_help, "r - switch running", MENU_CANCEL);
        add_entry(&menu_help, "e, ENTER - interact", MENU_CANCEL);
}
                
void menu_go_down()
{
    current_menu->menu_pos++;
    if (current_menu->menu_pos == current_menu->options)
        current_menu->menu_pos = current_menu->options - 1;
}

void menu_go_up()
{
    current_menu->menu_pos--;
    if (current_menu->menu_pos < 0 )
        current_menu->menu_pos = 0;
}

int menu_interract(int key)
{
    switch (key)
    {
       case SDLK_ESCAPE:
       {
            if (current_menu) current_menu=NULL; else current_menu=&menu_main;
            return 1;
       }
       case SDLK_m:
       { 
           if (!current_menu) current_menu=&menu_energy; else if (current_menu ==  &menu_energy) current_menu=NULL;
           return 1;
       }
       case SDLK_DOWN:
       case SDLK_s:
       {
            if (current_menu)  menu_go_down();
            break;
       }
       case SDLK_w:
       case SDLK_UP:
       {
            if (current_menu)  menu_go_up();
            break;
       }

       case SDLK_RETURN:
       case SDLK_e:
       {
            if (current_menu) 
            {
                if (interact(current_menu->actions[current_menu->menu_pos])) 
                    current_menu=NULL;
            }
            break;
       }
    }
    return  current_menu ? 1 : 0;
}

int interact(enum menu_actions a)
{
    switch(a)
    {
        case MENU_CANCEL:
            break;
            
        case MENU_REGAIN:
            player.energy+=100;
            break;

        case MENU_BOOST:
            player.energy=1000;
            break;

        case MENU_SAVE:
            save(1);
            break;

        case MENU_SAVE_EXIT:
            save(1);
        case MENU_EXIT:
            SDL_Quit();
            exit(0);

        case MENU_LOAD:
            load(1);
            break;

        case MENU_HELP: 
            current_menu=&menu_help;
            return 0;          
    }
    return 1;
}
