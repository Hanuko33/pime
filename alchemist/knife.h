#ifndef KNIFE_H
#define KNIFE_H

#include "elements.h"

class Knife : public Product
{
    public:
    Knife(InventoryElement *el1, InventoryElement *el2);
    virtual bool use(Player * player);
    bool check_ing();
};
#endif
