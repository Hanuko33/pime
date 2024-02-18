#ifndef EL_LIST_H
#define EL_LIST_H

#include "elements.h"

class ListElement
{
public:	
    Element * el; 
	int count;
    ListElement * next;

	void add(ListElement * entry);
	void show();
	ListElement(Element *entry, int c);
};
    

class List
{
public:
    const char * name;
    int nr_elements;
    ListElement * head;
    ListElement * tail;

	List(const char *n);
	ListElement * find(Element * what);
	Element ** find_form(enum Form f, int *count);
	void show();
	void add(Element *el);
	void remove(Element *el);
	int get_count(Element *el);
};
#endif // EL_LIST_H
