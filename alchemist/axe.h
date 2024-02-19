#ifndef AXE_H
#define AXE_H

#include "elements.h"

class Axe : public Product
{
    public:
    Axe(InventoryElement *el1, InventoryElement *el2);
    bool check_ing();
};
#endif
