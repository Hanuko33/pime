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
        Plant * p = world_table[player->map_y][player->map_x]->plants[i];
        if (p)
        {
            int x,y;
            p->get_posittion(&x, &y);
            if (player->x == x && player->y == y && p->grown)
            {
                if (p->type == PLANTID_tree || p->type == PLANTID_tree1 || p->type == PLANTID_tree2)
                {
                    
                    switch (p->type) {
                        case PLANTID_tree:
                        {
                            Element * el = new Element(base_elements[ID_LOG]);
                            el->set_posittion(x, y);
                            set_item_at_ppos(el, player);
                            break;
                        }
                        case PLANTID_tree1:
                        {
                            Element * el = new Element(base_elements[ID_LOG1]);
                            el->set_posittion(x, y);
                            set_item_at_ppos(el, player);
                            break;
                        }
                        case PLANTID_tree2:
                        {
                            Element * el = new Element(base_elements[ID_LOG2]);
                            el->set_posittion(x, y);
                            set_item_at_ppos(el, player);
                            break;
                        }
                    }
                    free(p);
                    p=NULL;
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
