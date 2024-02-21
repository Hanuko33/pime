#include "el_list.h"
#include <cstdlib>
#include <cstdio>

ListElement::ListElement(InventoryElement *entry)
{
	el=entry;
	next = NULL;
}

void ListElement::add(ListElement * entry)
{
    next = entry;
}

void ListElement::show()
{
    el->show();
}

List::List(const char * n)
{
    name = n;
    nr_elements=0;
    tail = NULL;
    head = NULL;
}

ListElement * List::find(InventoryElement * what)
{
    ListElement * cur = head;
    while(cur) {
        if (cur->el == what) return cur;
        cur = cur->next;
    }
    return NULL;
}

InventoryElement ** List::find_form(enum Form f, int *count)
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

void List::show()
{
    ListElement * cur = head;
    printf("--- %s (%d) ---\n", name, nr_elements);
    while(cur) {
        cur->show();
        cur = cur->next;
    }
}

void List::add(InventoryElement *el)
{
	ListElement * entry = new ListElement(el);
    if (nr_elements) {
        tail->add(entry);
        tail = entry;
    } else {
        head = entry;
        tail = entry;
    }
    nr_elements++;
}

void List::remove(InventoryElement *el)
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

