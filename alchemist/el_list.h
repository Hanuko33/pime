#ifndef EL_LIST_H
#define EL_LIST_H

#include "elements.h"

class ListElement
{
public:	
    InventoryElement * el; 
    ListElement * next;

	void add(ListElement * entry);
	void show();
	ListElement(InventoryElement *entry);
};
    

class List
{
public:
    const char * name;
    int nr_elements;
    ListElement * head;
    ListElement * tail;

	List(const char *n);
	ListElement * find(InventoryElement * what);
	InventoryElement ** find_form(enum Form f, int *count);
	void show();
	void add(InventoryElement *el);
	void remove(InventoryElement *el);
	int get_count(InventoryElement *el);
};
#endif // EL_LIST_H
