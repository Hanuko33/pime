#include "axe.h"
#include <cstdio>
#include "../player.h"
#include "../world.h"
#include "elements.h"

Axe::Axe(InventoryElement *el1, InventoryElement *el2): Product(el1, el2, PROD_AXE, Form_solid)
{

}

void Axe::use(Player * player)
{
    for (int i = 0; i < CHUNK_SIZE*CHUNK_SIZE; i++) {
        Being * b = world_table[player->map_y][player->map_x]->beings[i];
        if (b)
        {
            int x,y;
            b->get_posittion(&x, &y);
            if (player->x == x && player->y == y)
            {
                if (b->type == being_tree)
                {
                    free(b);
                    b=NULL;
                    world_table[player->map_y][player->map_x]->beings[i]=NULL;

                    Element * el = new Element(base_elements[ID_LOG]);
                    el->set_posittion(x, y);
                    set_item_at_ppos(el, player);
                }
                // TODO    break when used too much
            }
        }
    }
}

bool Axe::check_ing()
{
    int id1 = ings[0]->get_id();
    int id2 = ings[1]->get_id();

    if (
            (id1 == ING_AXE_BLADE && id2 == ING_AXE_HANDLE) || 
            (id1 == ING_AXE_HANDLE && id2 == ING_AXE_BLADE)
       ) return true;
    printf("wrong ingredients\n");
    return false;
}
