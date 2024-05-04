#include <cstdlib>

#include "elements.h"
#include "names.h"
#include "../texture.h"

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

const char * Plant_phase_name[]=
{
    "Seed",
    "Seedling",
    "Growing",
    "Flowers",
    "Fruits"
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
    printf("      stretching = %u\n", stretching); // rozciąganie
    printf("      squeezing = %u\n", squeezing); //ściskanie
    printf("      bending = %u\n", bending); // zginanie
    printf("      fragility = %u\n", fragility); //kruchość
    printf("      solubility = %u\n", solubility); //rozpuszczalność

}

BaseElement::BaseElement()
{
    transparency=rand() % 256;
    solid=NULL;
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
    if (rand() % 100 < 30) // 30 % food
    {
        edible=new Edible;
        id=rand() % FOOD_ELEMENTS;
    }
    name = create_name(5 - form);
    magic=false; //TODO
}

void BaseElement::show(bool details)
{
    printf("BaseElement name=%s form=%s\n", name, Form_name[form]); 
    if (!details) return;
    printf("   density = %u\n", density); //gęstość
    printf("   transparency = %u\n", transparency); //przezroczystość
    printf("   form = %s\n", Form_name[form]);
    switch(form)
    {
        case Form_solid:
            solid->show();
            break;
        default: break;
    }
    if (edible) edible->show();
}

Element::Element(BaseElement *b)
{
    c_id=Class_Element;
    base = b;
    sharpness = rand() % 100;
    smoothness = rand() % 100;
    length = 1 + rand() % 100;
    width = 1 + rand() % 100;
    height = 1 + rand() % 100;
    volume = length * width * height;
    mass = b->density * volume/1000;
    
}

void Element::show(bool details)
{
    printf("Element -> %d: base=%s form=%s\n", c_id, base->name, get_form_name());
    if (!details) return;
    printf("sharpness = %u\n", sharpness); //ostrość
    printf("smoothness = %u\n", smoothness); //gładkość
    printf("mass = %u: l=%u w=%u h=%u \n", mass, length, width, height);
    base->show(details);
}

#ifndef STUB_SDL
SDL_Texture * Element::get_texture()
{
    if (base->edible) return food_textures[base->id];

    switch (base->form)
    {
        case Form_solid: return items_textures[base->id]; break;
        case Form_liquid: return liquid_textures[base->id]; break;
        case Form_gas: return gas_textures[base->id]; break;
    }
    return NULL;
}
#endif
Ingredient::Ingredient(InventoryElement * from, Ingredient_id i, Form f)
{
    c_id=Class_Ingredient;
    el = from;
    name = Ingredient_name[i];
    id = i;
    req_form = f;
}
        
bool Ingredient::craft()
{
    if (req_form != get_form()) {
        printf("form != %d\n", req_form);
        return false;
    }

    quality = rand() % 100;
    resilience = rand() % 100;
    usage = rand() % 100;
    return true;
}

void Ingredient::show(bool details)
{
    printf("%s ->%d\n", name, c_id);
    if (!details) return;
    printf("quality = %d\n", quality);
    printf("resilience = %d\n", resilience);
    printf("usage = %d\n", usage);
    el->show(details);
}
        
#ifndef STUB_SDL
SDL_Texture *Ingredient::get_texture() 
{ 
    return ing_textures[id]; 
}
#endif
void Product::init(Product_id i, int c, Form f)
{
    name = Product_name[i];
    id = i;
    ing_count=c;
    req_form=f;
}

Product::Product(InventoryElement * el1, InventoryElement *el2, Product_id i, Form f)
{
    c_id=Class_Product;
    ings = (InventoryElement**) calloc(2, sizeof(InventoryElement));
    ings[0]=el1;
    ings[1]=el2;
    init(i, 2, f);
}

Product::Product(InventoryElement ** from, int count, Product_id i, Form f)
{
    c_id=Class_Product;
    ings = from;
    init(i, count, f);
}

bool Product::craft()
{
    for (int i=0; i < ing_count; i++)
    {
        if (req_form != ings[i]->get_form()) {
            printf("form != %d for inq[%d]\n", req_form, i);       
            return false;
        }
    }
    if (!check_ing()) return false;

    quality = rand() % 100;
    resilience = rand() % 100;
    usage = rand() % 100;
    return true;
}

void Product::show(bool details)
{
    printf("%s -> %d\n", name, c_id);
    if (!details) return;
    printf("quality = %d\n", quality);
    printf("resilience = %d\n", resilience);
    printf("usage = %d\n", usage);

    for (int i=0; i < ing_count; i++)
    {
        ings[i]->show(details);
    }
}

#ifndef STUB_SDL     
SDL_Texture *Product::get_texture() 
{ 
    return prod_textures[id]; 
}
#endif        
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
            default: return Form_none;
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

void show_base_elements(bool details)
{
    for (int i=0; i < BASE_ELEMENTS; i++)
    {
        base_elements[i]->show(details);
    }

}

Plant::Plant()
{
    c_id=Class_Plant;
    seedling_time=7 + rand() % 14;
    growing_time=seedling_time + rand() % 150;
    flowers_time=growing_time + rand() % 30;
    fruits_time=flowers_time + rand() % 30;
    max_age=fruits_time + rand() % 100;
    phase = (Plant_phase) (rand() %  (Plant_fruits+1));
    switch (phase)
    {
        case Plant_seed: age = 0; planted = false; break;
        case Plant_seedling: age = seedling_time;  planted = true; break;
        case Plant_growing: age = growing_time; planted = true; break;
        case Plant_flowers: age = flowers_time; planted = true; break;
        case Plant_fruits: age=fruits_time; planted = true; break;
    }
    edible = new Edible;
}

bool Plant::grow()
{
    if (!alive) return false;
    if (!Being::grow()) return false;
    if (age >= fruits_time) { change_phase(Plant_fruits); return alive; }
    if (age >= flowers_time) { change_phase(Plant_flowers); return alive; }
    if (age >= growing_time) { change_phase(Plant_growing); return alive; }
    if (age >= seedling_time) { change_phase(Plant_seedling); return alive; }
    return alive;
}
