#include "axe.h"
#include <cstdio>
#include "../player.h"
#include "../world.h"
#include "elements.h"

Axe::Axe(InventoryElement *el1, InventoryElement *el2): Product(el1, el2, PROD_AXE, Form_solid)
{

}

bool Axe::use(Player * player)
{
    for (int i = 0; i < CHUNK_SIZE*CHUNK_SIZE; i++) {
        Being * b = world_table[player->map_y][player->map_x]->beings[i];
        if (b)
        {
            int x,y;
            b->get_posittion(&x, &y);
            if (player->x == x && player->y == y)
            {
                if (b->type == BEINGID_tree || b->type == BEINGID_tree1 || b->type == BEINGID_tree2)
                {
                    
                    switch (b->type) {
                        case BEINGID_tree:
                        {
                            Element * el = new Element(base_elements[ID_LOG]);
                            el->set_posittion(x, y);
                            set_item_at_ppos(el, player);
                            break;
                        }
                        case BEINGID_tree1:
                        {
                            Element * el = new Element(base_elements[ID_LOG1]);
                            el->set_posittion(x, y);
                            set_item_at_ppos(el, player);
                            break;
                        }
                        case BEINGID_tree2:
                        {
                            Element * el = new Element(base_elements[ID_LOG2]);
                            el->set_posittion(x, y);
                            set_item_at_ppos(el, player);
                            break;
                        }
                    }
                    free(b);
                    b=NULL;
                    world_table[player->map_y][player->map_x]->beings[i]=NULL;
                    return true;
                }
                // TODO    break when used too much
            }
        }
    }
    return false;
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
