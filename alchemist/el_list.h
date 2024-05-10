#ifndef EL_LIST_H
#define EL_LIST_H

#include "elements.h"

class ListElement
{
    bool enabled;
public:	
    InventoryElement * el; 
    ListElement * next;

	void add(ListElement * entry);
    void disable() { enabled=false; };
    void enable() { enabled=true; };
    bool is_enabled() { return enabled; };
	virtual void show(bool details=true);
    virtual bool tick() { return el->tick(); };
	ListElement(InventoryElement *entry);
    ListElement(): el(nullptr), next(nullptr) {}
};
    

class InvList
{
public:
    const char * name;
    int nr_elements;
    ListElement * head;
    ListElement * tail;

	InvList(const char *n);
    ListElement * find(void * what);
    bool virtual find_check(ListElement *el, void * what) { return el == what; }
//    bool virtual find_at_check(ListElement *el, void * pos) { return false; }
	InventoryElement ** find_form(enum Form f, int *count);
	void show(bool details=true);
	void add(InventoryElement *el);
	void add(ListElement *el);
	void remove(InventoryElement *el);
	int get_count(InventoryElement *el);
    void tick();
    void enable_all();
};

class Show_el : public ListElement
{
public:
    char c;
    bool selected;
    ListElement *l_el;
    Show_el(char _c, ListElement *_el);
    void show(bool details=true);
};

class Show_list : public InvList
{
    char prompt;
public:
    Show_list(char p): InvList("select list") { prompt = p;}
    ListElement * select_el();
    bool multi_select();
    void unselect_all();
    bool find_check(ListElement *el, void * what);
};

extern const char * colorGray;
extern const char * colorRed;
extern const char * colorRedBold;
extern const char * colorGreen;
extern const char * colorGreenBold;
extern const char * colorYellow;
extern const char * colorYellowBold;
extern const char * colorBlue;
extern const char * colorMagenta;
extern const char * colorCyan;
extern const char * colorWhite;
extern const char * colorNormal;
extern const char * clrscr;

int kbhit();
char wait_key(char prompt);


#endif // EL_LIST_H
