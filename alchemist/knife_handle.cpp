#include "knife_handle.h"
#include "elements.h"
#include <cstdlib>

KnifeHandle::KnifeHandle(InventoryElement *from): Ingredient(from, ING_KNIFE_HANDLE, Form_solid)
{
}
