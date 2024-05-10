#include "knife_blade.h"
#include "elements.h"
#include <cstdlib>

KnifeBlade::KnifeBlade(InventoryElement *from): Ingredient(from, ING_KNIFE_BLADE, Form_solid)
{
}
