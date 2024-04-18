#include "el_list.h"
#include <stdlib.h>
#include <time.h>
#include "test_axe.h"
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstdio>
#include "game_time.h"

InvList * inventory;
InvList * elements;
InvList * plants;

void (*callback_daily)();

void daily_call()
{
  //  printf("daily call\n");
    plants->tick();
}

void help()
{
    printf("%sESC/e/?- clear screen/Exit/Help\n", colorWhite);
    printf("t - Test\n");
    printf("c - Craft\n");
    printf("s - Show\n");
    printf("h - Harvest plant\n");
    printf("o - sOw plant\n");
    printf("@ - change clock\n");
    printf("f - find\n");
}

InventoryElement * select_element(InvList * list)
{
    ListElement *elems = list->head;
    Show_list *show_cat = new Show_list('e');
    char k = 'a';

    while(elems)
    {
        if (elems->is_enabled())
        {
            show_cat->add(new Show_el(k, elems));
            k++;
        }
        elems=elems->next;
    }
    printf("%s", colorCyan);
    show_cat->show(false);
    elems = show_cat->select_el();
    if (!elems) return nullptr;
    if (elems->el)
    {
        printf("%s%s", colorNormal, colorGreenBold);
        printf("selected %s\n", elems->el->get_name());
        return elems->el;
    }

    return nullptr;
}

void harvest_plant()
{
    ListElement * cur = plants->head;
    while(cur) {
        Plant * p=(Plant*) cur->el;
        if (!p->planted) cur->disable();
        cur = cur->next;
    }

    InventoryElement *el=select_element(plants);
    plants->enable_all();
    if (!el) return;
    inventory->add(el);
    plants->remove(el);
    printf("plant: %s harvested to inventory\n", el->get_name());
}

void show()
{
    printf("%sb - base elements\n", colorCyan);
    printf("e - elements\n");
    printf("i - inventory\n");
    printf("p - plants\n");

    printf("%s%s", colorNormal, colorGreenBold);

    char c=wait_key('p');
        switch(c)
        {
            case 'b': show_base_elements(); break;
            case 'i': inventory->show(false); break;
            case 'e': elements->show(true); break;
            case 'p': plants->show(true); break;
        }
}

void add_new_plant()
{
    Plant *p=new Plant();
    if (p->phase == Plant_seed)
    {
        elements->add(p);
        printf("new Plant seed %s found\n", p->get_name());
    }
    else {
        plants->add(p);
        printf("new Plant %s found\n", p->get_name());
    }
}

void add_new_element()
{
    Element * el=new Element(base_elements[rand() % BASE_ELEMENTS]);
    elements->add(el);
    printf("new Element %s found\n", el->get_name());

}
void find_new()
{
    printf("%se - Element\n", colorCyan);
    printf("p - Plant\n");
    printf("%s%s", colorNormal, colorGreenBold);

    char c=wait_key('f');
        switch(c)
        {
            case 'e': add_new_element(); break;
            case 'p': add_new_plant(); break;
        }
}

void change_clock()
{
    printf("%sh - add 1 hour\n", colorCyan);
    printf("d - add 1 day\n");
    printf("m - add 30 days\n");
    printf("q - add 90 days\n");
    printf("y - add 1 year\n");
    printf("%s%s", colorNormal, colorGreenBold);

    char c=wait_key('@');
        switch(c)
        {
            case 'h': game_time.update_time(3600); break;
            case 'd': game_time.update_time(24*3600); break;
            case 'm': game_time.update_time(30*24*3600); break;
            case 'q': game_time.update_time(90*24*3600); break;
            case 'y': game_time.update_time(360*24*3600); break;
        }
}

void test()
{
    printf("%sa - test axe\n", colorCyan);

    printf("%s%s", colorNormal, colorGreenBold);

    char c=wait_key('t');
        switch(c)
        {
            case 'a': 
                {
                    Axe * axe=test_axe();
                    if (axe) inventory->add(axe); 
                }
                break;
        }
}

bool select_inventory2(InventoryElement **el1, InventoryElement **el2)
{
    ListElement *inv = inventory->head; 
    Show_list *show_cat = new Show_list('i');
    char k = 'a';
    
    while(inv) {
        show_cat->add(new Show_el(k, inv));
        k++;
        inv = inv->next;
    }
    printf("%s", colorCyan);
    show_cat->show(false);
    bool sel = show_cat->multi_select();
    if (!sel) return false;

    printf("%swybrane zasoby: ", colorGreen);
    Show_el * el = (Show_el*) show_cat->head;
    int count = 0;

    while(el)
    {
        if (el->selected)
        {
            printf("%s ", el->el->el->get_name());
            count++;
            if (!*el1) *el1=el->el->el; else *el2=el->el->el;
        }
        el=(Show_el*)el->next;
    }
    if (count == 2)
    {
        puts("");
        return true;
    }
 
    return false;
}


void craft()
{
    printf("%sa - craft axe\n", colorCyan);
    printf("b - craft axe blade\n");
    printf("h - craft axe handle\n");

    printf("%s%s", colorNormal, colorGreenBold);

    char c=wait_key('c');
        switch(c)
        {
            case 'a': 
                {                    
                    InventoryElement * el1=nullptr, * el2=nullptr;
                    if (!select_inventory2(&el1, &el2)) return;
                    Axe * axe=new Axe(el1, el2);
                    if (!axe->craft()) return;
                    inventory->add(axe); 
                    inventory->remove(el1);
                    inventory->remove(el2);
                    printf("axe added to inventory\n");
                }
                break;
            case 'b':
                {
                    InventoryElement *el=select_element(elements);
                    if (!el) return;
                    AxeBlade * axe_blade=new AxeBlade(el);  
                    if (!axe_blade->craft()) return; 
                    inventory->add(axe_blade); 
                    elements->remove(el);
                    printf("axe blade added to inventory\n");
                }
                break;
            case 'h':
                {
                    InventoryElement *el=select_element(elements);
                    if (!el) return;
                    AxeHandle * axe_handle=new AxeHandle(el);  
                    if (!axe_handle->craft()) return; 
                    inventory->add(axe_handle); 
                    elements->remove(el);
                    printf("axe handle added to inventory\n");
                }
                break;
        }
}

void sow_plant()
{
    ListElement * cur = elements->head;
    while(cur) {
        if (cur->el->c_id != Class_Plant) cur->disable();
        cur = cur->next;
    }

    InventoryElement *el=select_element(elements);
    elements->enable_all();
    if (!el) return;
    elements->remove(el);
    plants->add(el);
    Plant * p=(Plant*) el;
    p->sow();
    printf("%s planted\n", el->get_name());
}

void play()
{
    help();
    while(1)
    {
        char c=wait_key('>');
        switch(c)
        {
            case 't': test(); break;
            case 'c': craft(); break;
            case 's': show(); break;
            case 'e': return;
            case 27: printf("%s", clrscr); break;
            case '?': help(); break;
            case '@': change_clock(); break;
            case 'h': harvest_plant(); break;
            case 'o': sow_plant(); break;
            case 'f': find_new(); break;
        }
        game_time.update_time(1);
    }
}

int main()
{
    struct termios old_stdin, stdin_tty;

    setbuf(stdout, nullptr);
    printf("%s", clrscr);
    tcgetattr(0,&old_stdin);
    stdin_tty = old_stdin;
    stdin_tty.c_lflag &= ~(ECHO | ICANON |ECHOE| ISIG);
    stdin_tty.c_cc[VMIN] = 1; 
    stdin_tty.c_cc[VTIME] = 0;	
    tcsetattr(0, TCSANOW, &stdin_tty);

    tcflush(0, TCIFLUSH);
    ioctl(0, TCXONC,1);
    
    srandom(time(nullptr));
    init_elements();

    inventory = new InvList("inventory");
    elements = new InvList("elements");
    plants = new InvList("plants");

    for (int i=0; i < 5; i++)
        add_new_element();

    for (int i=0; i < 10; i++)
    {
        add_new_plant();
    }

    callback_daily=daily_call;
    play();

    tcsetattr(0, TCSANOW, &old_stdin);
    return 0;
}

