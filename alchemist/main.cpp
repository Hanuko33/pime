#include <ctime>
#include <cstdio>
#include <cstdlib>

#include "elements.h"
#include "../tiles.h"

SDL_Texture * tiles_textures[TILE_MAX_NUM];
SDL_Texture * items_textures[SOLID_ELEMENTS];
SDL_Texture * food_textures[FOOD_ELEMENTS];
SDL_Texture * liquid_textures[LIQUID_ELEMENTS];
SDL_Texture * gas_textures[GAS_ELEMENTS];
SDL_Texture * ing_textures[ING_ELEMENTS];
SDL_Texture * prod_textures[PROD_ELEMENTS];
SDL_Texture * objects_textures[OBJECT_MAX];
SDL_Texture * down_mask;

#include "axe_blade.h"
#include "axe_handle.h"
#include "axe.h"

void (*callback_daily)();

void daily_call()
{
  //  printf("daily call\n");
}

void test1()
{
	Element *elements[10];
    for (int i=0; i< 10; i++)
    {
        int b = rand() % 5;
        elements[i] = new Element(base_elements[b]);
        elements[i]->show();
    }
 
}
void test2()
{
    BaseElement *b1 = new BaseElement();
    BaseElement *b2 = new BaseElement();

    Element * el1 = new Element(b1);
    Element * el2 = new Element(b2);

//    el1->show();
  //  el2->show();

    AxeBlade * blade = new AxeBlade(el1);
    AxeHandle * handle = new AxeHandle(el2);
    
    if (!blade->craft()) {
        printf("can't create blade from:\n");
        blade->show();
        delete blade;
        return;
    }
    
    if (!handle->craft()) {
        printf("can't create handle from:\n");
        handle->show();
        delete handle;
        return;
    }
       
    Axe * axe = new Axe(blade, handle);
    if (axe->craft()) {
        axe->show();
    } 
    else 
    {
        printf("can't create axe\n");
        delete axe;
    }
}

void test3()
{
    BaseElement *b1 = new BaseElement();
    BaseElement *b2 = new BaseElement();

    Element * el1 = new Element(b1);
    Element * el2 = new Element(b2);

    Axe * axe = new Axe(el1, el2);
    if (axe->craft()) {
        axe->show();
    } 
    else 
    {
        printf("can't create axe\n");
        delete axe;
    }
}

int main()
{
    srandom(time(nullptr));
    
    callback_daily=daily_call;

    // test1();
  //   test2();
    test3();
   return 0;
}
