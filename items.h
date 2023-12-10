#ifndef ITEMENT_H
#define ITEMENT_H

#include "items_id.h"
#include "categories_id.h"

struct item {
    const char *name;
    enum category_id cat;
};

extern struct item items[IT_MAX];
void add_item(enum item_id id, enum category_id cat);

extern const char * items_names[IT_MAX];
extern const char * categories_names[CAT_MAX];
void init_items();
#endif

