#include <cstdlib>

#include "elements.h"
#include "names.h"

bool fantasy_game=1;
BaseElement *base_elements[BASE_ELEMENTS];

const char * Form_name[]
{
    "nothing",
    "solid form",
    "liquid",
    "gas"
};

const char * Ingredient_name[]=
{
    "Axe_blade",
    "Axe_handle",

    "Hammer_head",
    "Hammer_handle",

    "Knife_blade",
    "Knife_handle",

};

const char * Product_name[]=
{
    "Axe",
    "Hammer",
    "Knife",
};

const char * items_name[]=
{
    "Stone",
    "Log",
    "Sand",
    "Stick",
};

const char * food_name[]=
{
    "Pumpkin",
    "Watermelon"
};

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

BaseElement::BaseElement()
{
    transparency=rand() % 256;
    solid=NULL;
    if (fantasy_game) 
        init_fantasy();
    else
        init_real();
    color = godot::Color((rand()%256)/255.0, (rand()%256)/255.0, (rand()%256)/255.0);
}

void BaseElement::init_fantasy()
{
    int f = rand() % 100;

    if (f < 60) { //60%
            form = Form_solid;
            solid=new Solid;
            density=50 + rand() % 2000;
            id=rand() % SOLID_ELEMENTS;
    } else {
        if (f < 90) { //30%
            form = Form_liquid;
            density=500 + rand() % 500;
            id=rand() % LIQUID_ELEMENTS;
        } 
        else { //10%
            form = Form_gas;
            density=1;
            id=rand() % GAS_ELEMENTS;
        }
    }

    edible = NULL;
    if (rand() % 100 < 30) // 10 % food
    {
        edible=new Edible;
        id=rand() % FOOD_ELEMENTS;
    }
    name = get_name(5 - form);
}

void BaseElement::init_real()
{
    form = Form_solid;
    solid=new Solid;
    density=50 + rand() % 2000;
    id=rand() % SOLID_ELEMENTS;
    name=items_name[id];
    
    edible = NULL;
    if (rand() % 100 < 30) // 10 % food
    {
        edible=new Edible;
        id=rand() % FOOD_ELEMENTS;
        name=food_name[id];
    }
}

void BaseElement::show()
{
    printf("   *** BaseElement -> %d ***\n", c_id);
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
    printf("\n*** Element -> %d: %s ***\n", c_id, base->name);
    printf("sharpness = %u\n", sharpness);
    printf("smoothness = %u\n", smoothness);
    printf("mass = %u: l=%u w=%u h=%u \n", mass, length, width, height);
    base->show();
}

Ingredient::Ingredient(InventoryElement * from, Ingredient_id i, Form f)
{
    el = from;
    name = Ingredient_name[i];
    id = i;
    req_form = f;
}
        
bool Ingredient::craft()
{
    if (req_form != get_form()) return false;

    quality = rand() % 100;
    resilience = rand() % 100;
    usage = rand() % 100;
    return true;
}

void Ingredient::show()
{
    printf("\nvvv %s ->%d vvv\n", name, c_id);
    printf("quality = %d\n", quality);
    printf("resilience = %d\n", resilience);
    printf("usage = %d\n", usage);
    el->show();
    printf("^^^ %s ^^^\n", name);
}
        
void Product::init(Product_id i, int c, Form f)
{
    name = Product_name[i];
    id = i;
    ing_count=c;
    req_form=f;
}

Product::Product(InventoryElement * el1, InventoryElement *el2, Product_id i, Form f)
{
    ings = (InventoryElement**) calloc(2, sizeof(InventoryElement));
    ings[0]=el1;
    ings[1]=el2;
    init(i, 2, f);
}

Product::Product(InventoryElement ** from, int count, Product_id i, Form f)
{
    ings = from;
    init(i, count, f);
}

bool Product::craft()
{
    for (int i=0; i < ing_count; i++)
    {
        if (req_form != ings[i]->get_form()) return false;
    }
    if (!check_ing()) return false;

    quality = rand() % 100;
    resilience = rand() % 100;
    usage = rand() % 100;
    return true;
}

void Product::show()
{
    printf("\n!!! %s -> %d!!!\n", name, c_id);
    printf("quality = %d\n", quality);
    printf("resilience = %d\n", resilience);
    printf("usage = %d\n", usage);

    for (int i=0; i < ing_count; i++)
    {
        ings[i]->show();
    }
    printf("iii %s iii\n", name);
}
        
Form Product::get_form()
{
    int solid=0;
    int liq=0;
    int gas=0;

    for (int i=0; i < ing_count; i++)
    {
        switch (ings[i]->get_form())
        {
            case Form_solid: solid++; break;
            case Form_liquid: liq++; break;
            case Form_gas: gas++; break;
        }
    }
    if (solid) return Form_solid;
    if (gas == ing_count) return Form_gas;
    if (liq) return Form_liquid;
    return Form_none;
}

const char * Product::get_form_name() 
{ 
    return Form_name[get_form()]; 
}

void init_elements()
{
    for (int i=0; i < BASE_ELEMENTS; i++)
    {
        base_elements[i] = new BaseElement;
    }
}
