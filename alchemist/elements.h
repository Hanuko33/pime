#ifndef __ELEMENTS__H
#define __ELEMENTS__H

#include <cstdio>
#include <SDL2/SDL.h>

class Edible
{
    public:
    unsigned int irrigation;
    unsigned int  poison;
    unsigned int caloric;

    Edible();
    void show();
};

class Solid
{       
    public:
        unsigned int stretching; // <-->
        unsigned int squeezing; //  >--<
        unsigned int bending;
        unsigned int fragility;  //kruchosc
        unsigned int solubility; // rozpuszczalnosc
        Solid();
        void show();
};

enum Form
{
    Form_none=0,
    Form_solid=1,
    Form_liquid,
    Form_gas,
};

class BaseElement
{
    public:
        char * name;
        int id; //texture id
        unsigned int density;
        unsigned char transparency;
        Edible *edible;
        Form form;
        Solid *solid;
        
        BaseElement(); 
        void show();
};        

class InventoryElement
{
	int x, y, z;
    public:
        InventoryElement() {}
        virtual void show() {}
        virtual Form get_form() {return Form_none; }
        virtual const char * get_name() {return NULL; }
        virtual int get_id() {return -1; }
        virtual SDL_Texture * get_texture() { return NULL;}
        void set_posittion(int _x, int _y, int _z) { x=_x; y=_y; z=_z; }
        void get_posittion(int *_x, int *_y, int *_z) { *_x=x; *_y=y; *_z=z; }
};

class Element : public InventoryElement
{
    BaseElement * base;
    public:
        unsigned int sharpness;
        unsigned int smoothness;
        unsigned int mass; //density*volume
        unsigned int length;
        unsigned int width;
        unsigned int height;
        unsigned int volume; //lenght*width*height
    
        void show();

        Element(BaseElement *b);
        Form get_form() {return base->form; }
        const char * get_name() {return base->name; }
        int get_id() {return base->id; }
        SDL_Texture * get_texture();
};

enum Ingredient_id
{
    ING_AX_BLADE,
    ING_AX_HANDLE,

    ING_HAMMER_HEAD,
    ING_HAMMER_HANDLE,

    ING_KNIFE_BLADE,
    ING_KNIFE_HANDLE,
    
};

extern const char * Ingredient_name[];

class Ingredient : public InventoryElement
{
    const char * name;
    public:
        int quality; //[0..100] slaby..najlepszy
        int resilience; // [0..100] wytrzymały..słaby
        int usage; // [0..100] łatwy..trudny
        Ingredient_id id;
        InventoryElement * el;

        Ingredient(InventoryElement * from, Ingredient_id i);
        void show();
        Form get_form() {return el->get_form(); }
        const char * get_name() {return name; }
        int get_id() {return id; }
        SDL_Texture * get_texture();
};

class Being : public Element
{
    public:
        unsigned int age;
        unsigned int max_age;
        void grow() {}
};
#define BASE_ELEMENTS 7
#define ING_ELEMENTS 2
extern BaseElement base_elements[BASE_ELEMENTS];

void init_elements();
#endif

