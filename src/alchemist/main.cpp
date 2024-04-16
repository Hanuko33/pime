#include "el_list.h"
#include <stdlib.h>
#include <time.h>
#include "test_axe.h"
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstdio>


InvList * inventory;
InvList * elements;

void help()
{
    printf("%sESC/e/?- clear screen/Exit/Help\n", colorWhite);
    printf("t - Test\n");
    printf("c - Craft\n");
    printf("s - Show\n");

}

void show()
{
    printf("%sb - base elements\n", colorCyan);
    printf("e - elements\n");
    printf("i - inventory\n");

    printf("%s%s", colorNormal, colorGreenBold);

    char c=wait_key('p');
        switch(c)
        {
            case 'b': show_base_elements(); break;
            case 'i': inventory->show(false); break;
            case 'e': elements->show(true); break;
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

InventoryElement * select_inventory()
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
    inv = show_cat->select_el();
    if (!inv) return nullptr;
    if (inv->el)
    {
        printf("%s%s", colorNormal, colorGreenBold);
        printf("wybrano %s\n", inv->el->get_name());
        return inv->el;
    }
 
    return nullptr;
}

InventoryElement * select_element()
{
    ListElement *elems = elements->head; 
    Show_list *show_cat = new Show_list('e');
    char k = 'a';
    
    while(elems) 
    {
        show_cat->add(new Show_el(k, elems));
        k++;
        elems=elems->next;
    }
    printf("%s", colorCyan);
    show_cat->show(false);
    elems = show_cat->select_el();
    if (!elems) return nullptr;
    if (elems->el)
    {
        printf("%s%s", colorNormal, colorGreenBold);
        printf("wybrano %s\n", elems->el->get_name());
        return elems->el;
    }
 
    return nullptr;
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
                    InventoryElement * el1 = select_inventory();
                    if (!el1) return;
                    InventoryElement * el2 = select_inventory();
                    if (!el2) return;
                    
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
                    InventoryElement *el=select_element();
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
                    InventoryElement *el=select_element();
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
        }
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
    for (int i=0; i < 5; i++)
        elements->add(new Element(base_elements[rand() % BASE_ELEMENTS]));

    play();

    tcsetattr(0, TCSANOW, &old_stdin);
    return 0;
}

