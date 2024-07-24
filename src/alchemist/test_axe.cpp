#include "test_axe.h"
#include <stdlib.h>

Axe * test_axe()
{
    Element * element1=new Element(base_elements[rand() % BASE_ELEMENTS]);
    Element * element2=new Element(base_elements[rand() % BASE_ELEMENTS]);
    printf("element1 = %s\n", element1->get_name());
    printf("element2 = %s\n", element2->get_name());
    
    AxeBlade * axe_blade=new AxeBlade(element1);  
    if (!axe_blade->craft()) return nullptr; 
//    axe_blade->show();

    AxeHandle * axe_handle=new AxeHandle(element2);
    if (!axe_handle->craft()) return nullptr;
  //  axe_handle->show();

    Axe * axe=new Axe(axe_blade, axe_handle);
    if (!axe->craft()) return nullptr;
    axe->show();

    return axe;
}

