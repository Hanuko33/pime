#include "knife.h"
#include <cstdlib>

Knife::Knife(InventoryElement *el1, InventoryElement *el2): Product(el1, el2, PROD_KNIFE, Form_solid)
{

}

bool Knife::check_ing()
{
    int id1 = ings[0]->get_id();
    int id2 = ings[1]->get_id();

    if (
            (id1 == ING_KNIFE_BLADE && id2 == ING_KNIFE_HANDLE) || 
            (id1 == ING_KNIFE_HANDLE && id2 == ING_KNIFE_BLADE)
       ) return true;
    printf("wrong ingredients\n");
    return false;
}
