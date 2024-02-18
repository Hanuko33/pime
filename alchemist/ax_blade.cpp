#include "ax_blade.h"
#include <cstdlib>

AxBlade::AxBlade(InventoryElement *from): Ingredient(from, ING_AX_BLADE)
{
    quality = rand() % 100;
    resilience = rand() % 100;
    usage = rand() % 100;
}
