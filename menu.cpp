#include "menu.h"
#include "window.h"
#include "music.h"
#include "alchemist/elements.h"
#include "texture.h"

extern struct Player player;
extern int active_hotbar;

Menu *menu_music;
Menu *menu_main;
Menu *menu_energy;
Menu *menu_help;
Menu *menu_help2;
Menu *current_menu;
Menu *menu_inventory_categories;
Menu *menu_inventory;

void load(char with_player);
void save(char with_player);

Menu::Menu(int opt)
{
    options=opt;
	menu_pos=0;
    added=0;
    entries=(const char**)calloc(opt, sizeof(char*));
    actions=(menu_actions*)calloc(opt, sizeof(enum menu_actions));
    values=(int*)calloc(opt, sizeof(int));
    el=(Element**)calloc(opt, sizeof(Element*));
    show_texture=false;
}

void Menu::add(const char * e, enum menu_actions a)
{
    entries[added] = e;
    actions[added] = a;
    added++;
}

void Menu::add(const char * e, enum menu_actions a, int val)
{
	values[added] = val;
    add(e, a);
}

void Menu::add(const char * e, enum menu_actions a, Element * p_el)
{
    int i=a & ~ MENU_ITEM; 
    el[i] = p_el;
    add(e, a);
    show_texture=true;
}


int Menu::get_val(int v)
{
	for (int i=0; i < added; i++)
	{
		if (actions[i] == v) return values[i];
	}
	return 0;
}

void Menu::show()
{
    int window_width;
    int window_height;  
    int i;
    int game_size; 

    SDL_GetWindowSize(main_window, &window_width, &window_height);

    if (window_width < window_height) 
        game_size = window_width;   
    else 
        game_size = window_height;

    int menu_opt_size = game_size/10;
    int mody;
    int mody2;

    if (options % 2)
    {
        mody = (game_size/2)-(menu_opt_size*(options/2)+menu_opt_size/2);
        mody2 = (game_size/2)+(menu_opt_size*(options/2)+menu_opt_size/2);
    }
    else
    {
        mody = (game_size/2)-(menu_opt_size*(options/2));
        mody2 = (game_size/2)+(menu_opt_size*(options/2));
    }

    int modx = (game_size/2)-(0.4*game_size);
    int modx2 = (game_size/2)+(0.4*game_size);
   
    SDL_Rect rect = {modx, mody, modx2-modx, mody2-mody};
    SDL_SetRenderDrawColor(renderer, 0, 0, 1, 100);
    SDL_RenderFillRect(renderer, &rect);
    
    int mody3 = mody+(((menu_pos+1)*menu_opt_size)-(menu_opt_size));
    int mody4 = mody+((menu_pos+1)*menu_opt_size);
    
    SDL_Rect rect2 = {modx, mody3, modx2-modx, mody4-mody3};
    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 100);
    SDL_RenderFillRect(renderer, &rect2);
    
    for (i=0; i < options; i++){
        if (show_texture) {
        SDL_Rect rect;
		rect.x = modx;
		rect.y = mody + i * menu_opt_size;
		rect.w = 32;
		rect.h = 32;

		SDL_Texture *texture = items_textures[el[i]->base->id];
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        write_text(rect.x+32, rect.y, entries[i], White, game_size/27, menu_opt_size);
        } 
        else 
            write_text(modx, mody+i * menu_opt_size, entries[i], White, game_size/27, menu_opt_size);
    }
}

void create_menus()
{
    menu_main = new Menu(4);
    menu_main->add("Exit", MENU_EXIT);
        //add("Save & Exit", MENU_SAVE_EXIT);
        //add("Save", MENU_SAVE);
        //add("Load", MENU_LOAD);
   	menu_main->add("Help", MENU_HELP);
    menu_main->add("Change music volume", MENU_MUSIC);
    menu_main->add("Cancel", MENU_CANCEL);

    menu_energy = new Menu(3);
    menu_energy->add("Regain 100 energy", MENU_REGAIN);
    menu_energy->add("Set the energy to 1000", MENU_BOOST);
    menu_energy->add("Cancel", MENU_CANCEL);

    menu_help = new Menu(9);
    menu_help->add("ESC - game menu", MENU_CANCEL);
    menu_help->add("m - energy", MENU_CANCEL);
    menu_help->add("arrows - moves", MENU_CANCEL);
    menu_help->add("w,a,s,d - moves", MENU_CANCEL);
    menu_help->add("ctrl - run", MENU_CANCEL);
    menu_help->add("shift - sneak", MENU_CANCEL);
    menu_help->add("e,ENTER - pickup", MENU_CANCEL);
    menu_help->add("i - inventory", MENU_CANCEL);
    menu_help->add("N E X T", MENU_HELP_2);

    menu_help2 = new Menu(6);
    menu_help2->add("P R E V I O U S", MENU_HELP_1);
    menu_help2->add("= - use item in hotbar", MENU_CANCEL);
	menu_help2->add("1234567890 - hotbar", MENU_CANCEL);
	menu_help2->add("TAB - hotbar next", MENU_CANCEL);
	menu_help2->add("` - hotbar previous", MENU_CANCEL);
	menu_help2->add("Cancel", MENU_CANCEL);

    menu_music = new Menu(3);
    menu_music->add("+5 Volume", MENU_LOUDER);
    menu_music->add("-5 Volume", MENU_QUIETER);
    menu_music->add("Cancel", MENU_CANCEL);
    
    menu_inventory_categories = new Menu(2);
    menu_inventory_categories->add("Solid form", MENU_INV_SOLID, Form_solid);
    menu_inventory_categories->add("Liquid form", MENU_INV_LIGQUID, Form_liquid);
}
                
Menu * create_inv_menu(int v)
{

	printf("szukam %d\n", v);
	int c=0;
	Element ** el = player.inventory->find_form((Form) v, &c);
	if (el) {
		printf("found %d elements\n", c);

		if (menu_inventory) delete menu_inventory;
		menu_inventory = new Menu(c);
		
		for (int i=0; i < c; i++)
		{
			printf("%s\n", el[i]->base->name);
			menu_inventory->add(el[i]->base->name, (menu_actions)(MENU_ITEM+i), el[i]);
		}
        free(el);
		return menu_inventory;
	}
	return NULL;
}

void Menu::go_down()
{
    menu_pos++;
    if (menu_pos == options)
        menu_pos = 0;
}

void Menu::go_up()
{
    menu_pos--;
    if (menu_pos < 0 )
        menu_pos = options - 1;
}

int menu_interact(int key)
{
    switch (key)
    {
       case SDLK_ESCAPE:
       {
            if (current_menu) current_menu=NULL; else current_menu=menu_main;
            return 1;
       }
       case SDLK_m:
       { 
           if (!current_menu) current_menu=menu_energy; else if (current_menu ==  menu_energy) current_menu=NULL;
           return 1;
       }
       case SDLK_i:
       {
			player.inventory->show();
            if (!current_menu) current_menu=menu_inventory_categories; 
			else if (current_menu == menu_inventory_categories) current_menu=NULL;
            return 1;
       }

       case SDLK_DOWN:
       case SDLK_s:
       {
            if (current_menu)  current_menu->go_down();
            break;
       }
       case SDLK_w:
       case SDLK_UP:
       {
            if (current_menu)  current_menu->go_up();
            break;
       }

       case SDLK_RETURN:
       case SDLK_e:
       {
            if (current_menu) 
            {
                if (interact(current_menu->actions[current_menu->menu_pos])) 
                {
                    current_menu=NULL;
                    return 1;
                }
            }
            break;
       }
    }
    return  current_menu ? 1 : 0;
}

int handle_item(int i)
{
	if (active_hotbar >=0) {
        Element *el=menu_inventory->el[i];
		for (int h=0; h< 10; h++)
        {
            if (player.hotbar[h] == el) return 0;
        };
        player.hotbar[active_hotbar]=menu_inventory->el[i];
	}
	return 1;
}	

int interact(enum menu_actions a)
{
    if (a & MENU_ITEM) return handle_item(a & ~MENU_ITEM);
    switch(a)
    {
        case MENU_MUSIC:
            current_menu=menu_music;
            return 0;
        case MENU_REGAIN:
            player.hunger+=100;
            break;

        case MENU_HELP_2:
            current_menu=menu_help2;
            return 0;
        case MENU_HELP_1:
            current_menu=menu_help;
            return 0;

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
            Mix_Quit();
            exit(0);

        case MENU_LOAD:
            load(1);
            break;

        case MENU_HELP: 
            current_menu=menu_help;
            return 0;   
        
        case MENU_INV_SOLID: 
        case MENU_INV_LIGQUID: 
             current_menu=create_inv_menu(menu_inventory_categories->get_val(a));
            return 0;   

        case MENU_LOUDER:
            Mix_Volume(0, Mix_Volume(0, -1)+5);
            Mix_Volume(1, Mix_Volume(1, -1)+5);
            printf("%d\n%d\n", Mix_Volume(1, -1), Mix_Volume(0, -1));
            return 0;
        case MENU_QUIETER:
            Mix_Volume(0, Mix_Volume(0, -1)-5);
            Mix_Volume(1, Mix_Volume(1, -1)-5);
            printf("%d\n%d\n", Mix_Volume(1, -1), Mix_Volume(0, -1));
            return 0;
    }
    return 1;
}
