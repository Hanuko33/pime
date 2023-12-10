#include "items.h"
#include "items_names.h"
#include "categories_names.h"

struct item items[IT_MAX];

void add_item(enum item_id id, enum category_id cat)
{
    items[id].name=items_names[id];
    items[id].cat=cat;
}

void init_items()
{
    add_item(IT_stone, CAT_material);
    add_item(IT_log,   CAT_material);
    add_item(IT_sand,  CAT_material);
}


