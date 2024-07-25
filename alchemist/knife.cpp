#include "knife.h"
#include "elements.h"
#include <cstdio>
#include <cstdlib>
#include "../world.h"
#include "../player.h"

Knife::Knife(InventoryElement *el1, InventoryElement *el2): Product(el1, el2, PROD_KNIFE, Form_solid)
{

}

bool Knife::use(Player * player)
{
    for (int i = 0; i < CHUNK_SIZE*CHUNK_SIZE; i++) {
        Plant * p = world_table[player->map_y][player->map_x]->plants[i];
        if (p)
        {
            int x,y;
            p->get_posittion(&x, &y);
            if (player->x == x && player->y == y)
            {
                switch (p->type) {
                    case PLANTID_strawberry:
                    {
                        if (!p->alive)
                        {
                            Element * el = new Element(base_elements[ID_STRAWBERRY]);
                            el->set_posittion(x, y);
                            set_item_at_ppos(el, player);

                            Element * seed_el = new Element(base_elements[ID_STRAWBERRY_SEEDS]);
                            seed_el->set_posittion(x, y);
                            set_item_at_ppos(seed_el, player);

                            free(p);
                            p=NULL;
                            world_table[player->map_y][player->map_x]->plants[i]=NULL;
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
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
