#include <cstdlib>

#include "elements.h"
#include "names.h"

BaseElement base_elements[BASE_ELEMENTS];
       
Edible::Edible()
{
   caloric=rand() % 1000;
   irrigation=rand() % 100;
   poison=0;
   if (rand() %100 < 5) { //5%
       poison = 1 + rand() % 100;
   }
}

void Edible::show()
{
    printf("      *** Edible ***\n");
    printf("      caloric = %u\n", caloric);
    printf("      irrigation = %u\n", irrigation);
    printf("      poison = %u\n", poison);
}

Solid::Solid()
{
    stretching=1 + rand() % 10000;
            //styropian: 1
            //beton:     5
            //cyna:     14
            //drewno:   800
            //diament: 1800
            //żelazo:  3800
            //stal:   10000

    squeezing = 1 + rand() % 20000;
            //styropian:     1
            //beton:        50
            //kość:        150
            //kamień: 100-5000
            //diament:   17000

    fragility = 1000 * stretching / squeezing;
        // < 100 kruche
        // > spręzyste

    bending = 1 + rand() % 100;
            //1 łatwo zginalne 
            //100 trudno
    solubility=1+rand() % 100;
}

void Solid::show()
{
    printf("      *** Solid ***\n");
    printf("      stretching = %u\n", stretching);
    printf("      squeezing = %u\n", squeezing);
    printf("      bending = %u\n", bending);
    printf("      fragility = %u\n", fragility);
    printf("      solubility = %u\n", solubility);

}

int base_id;
BaseElement::BaseElement()
{
    id=base_id;
	base_id++;
    transparency=rand() % 256;
    solid=NULL;
    int f = rand() % 100;

    if (f < 60) { //60%
            form = Form_solid;
            solid=new Solid;
            density=50 + rand() % 2000;
    } else {
        if (f < 90) { //30%
            form = Form_liquid;
            density=500 + rand() % 500;
        } 
        else { //10%
            form = Form_gas;
            density=1;
        }
    }

    edible = NULL;
    if (rand() % 100 < 10) // 10 % food
    {
        edible=new Edible;
    }
    name = get_name(5 - form);
}

void BaseElement::show()
{
    printf("   *** BaseElement ***\n");
    printf("   density = %u\n", density);
    printf("   transparency = %u\n", transparency);
    printf("   form = %u\n", form);
    switch(form)
    {
        case Form_solid:
            solid->show();
            break;
    }
    if (edible) edible->show();
}

Element::Element(BaseElement *b)
{
    base = b;
    sharpness = rand() % 100;
    smoothness = rand() % 100;
    length = 1 + rand() % 100;
    width = 1 + rand() % 100;
    height = 1 + rand() % 100;
    volume = length * width * height;
    mass = b->density * volume/1000;
    
}

void Element::show()
{
    printf("\n*** Element: %s ***\n", base->name);
    printf("sharpness = %u\n", sharpness);
    printf("smoothness = %u\n", smoothness);
    printf("mass = %u: l=%u w=%u h=%u \n", mass, length, width, height);
    base->show();
}


