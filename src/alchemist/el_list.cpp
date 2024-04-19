#include "el_list.h"
#include <cstdlib>
#include <cstdio>
#include <sys/ioctl.h>
#include <errno.h>
#include <cstring>
#include <unistd.h>
#include "game_time.h"

ListElement::ListElement(InventoryElement *entry)
{
	el=entry;
	next = NULL;
    enabled=true;
}

void ListElement::add(ListElement * entry)
{
    next = entry;
}

void ListElement::show(bool details)
{
    el->show(details);
}

InvList::InvList(const char * n)
{
    name = n;
    nr_elements=0;
    tail = NULL;
    head = NULL;
}

ListElement * InvList::find(void  * what)
{
    ListElement * cur = head;
    while(cur) {
        if (find_check(cur, what)) return cur;
        cur = cur->next;
    }
    return NULL;
}

InventoryElement ** InvList::find_form(enum Form f, int *count)
{
    ListElement * cur = head;
	InventoryElement ** a = (InventoryElement**)calloc(nr_elements, sizeof(InventoryElement*));
	int c = 0;
    while(cur) {
        if (cur->el->get_form() == f) {
			a[c] = cur->el;
			c++;
		}
        cur = cur->next;
    }
	if (!c) {
		free(a);
		return NULL;
	}
	else {
		*count = c;
		return a;
	}
}

void InvList::show(bool details)
{
    ListElement * cur = head;
    printf("--- %s (%d) ---\n", name, nr_elements);
    while(cur) {
        cur->show(details);
        cur = cur->next;
    }
}

void InvList::enable_all()
{
    ListElement * cur = head;

    while(cur) {
        cur->enable();
        cur = cur->next;
    }
}

void InvList::tick()
{
    ListElement * cur = head;

    while(cur) {
        ListElement * next=cur->next;
        bool alive=cur->tick();
        if (!alive)
        {
            remove(cur->el);
        }
        cur = next;
    }
}

void InvList::add(InventoryElement *el)
{
	ListElement * entry = new ListElement(el);
    add(entry);
}

void InvList::add(ListElement *entry)
{
    if (nr_elements) {
        tail->add(entry);
        tail = entry;
    } else {
        head = entry;
        tail = entry;
    }
    nr_elements++;
}


void InvList::remove(InventoryElement *el)
{
   ListElement * cur = head;
   ListElement * tmp;
   if (head->el == el)
   {
       tmp = head->next;
       if (tail->el == el) tail = NULL;
       free(head);
       nr_elements--;
       head = tmp;
	   return;
   }
   while(cur)
   {	   
       if (!cur->next) break;
        if (cur->next->el == el)
        {
            tmp=cur->next;
            cur->next = cur->next->next;
            if (tail->el == el)
            {
                tail = cur;
            }
            free(tmp);
            nr_elements--;
            return;
        }
        cur = cur->next;
   }
}

const char *colorGray="\033[1;30m";
const char *colorRed="\033[2;31m";
const char *colorRedBold="\033[1;31m";
const char *colorGreen="\033[2;32m";
const char *colorGreenBold="\033[1;32m";
const char *colorYellow="\033[2;33m";
const char *colorYellowBold="\033[1;33m";
const char *colorBlue="\033[2;34m";
const char *colorMagenta="\033[2;35m";
const char *colorCyan="\033[2;36m";
const char *colorWhite="\033[1;37m";
const char *colorNormal="\033[0m";
const char *clrscr="\033[H\033[J";

int kbhit()
{
    int lf;

    if (ioctl(0, FIONREAD, &lf)==0) return lf;
    else return -1;
}

char wait_key(char prompt)
{
    printf("\r%s%c> ", game_time->get_time(),   prompt);
    while(!kbhit())
    {
        usleep(1000);
    }
    char c;
    if (read(0,&c,1) == 1) {
        printf("%c\n", c);
        return c;
    } else return 0;
}

Show_el::Show_el(char _c, ListElement *_el)
{
    c = _c;
    el = _el;
    selected = false;
}

void Show_el::show(bool details)
{
    printf("%c (%c) - ", c, selected ? '*' : ' ');
    el->show(details);
}

bool Show_list::find_check(ListElement *el, void * what)
{
    Show_el * s_el = (Show_el*)el;
    char * c = (char *) what;
    if (s_el->c == *c) return true;
    return false;
}

ListElement *Show_list::select_el()
{
    char c=wait_key(prompt);
    Show_el * f = (Show_el*) find(&c);
    if (f) return f->el;
    return nullptr;
}

bool Show_list::multi_select()
{
    char c = 0;
    bool sel=0;
    printf("z - zakończ selekcje\n");
    while (1){
        c=wait_key(prompt);
        if (c == 'z') break;
        Show_el * f = (Show_el*) find(&c);
        if (f) {
            f->selected ^= true;
            sel = true;
        }
        show(false);
        printf("z - zakończ selekcje\n");
    }
    return sel;
}

void Show_list::unselect_all()
{
    ListElement * cur = head;
    while(cur) {
        Show_el * el= (Show_el*) cur;
        el->selected = false;
        cur = cur->next;
    }
}
