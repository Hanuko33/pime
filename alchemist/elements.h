#ifndef __ELEMENTS__H
#define __ELEMENTS__H

#ifdef STUB_SDL
typedef void * SDL_Texture;
#else
#include <SDL2/SDL.h>
#endif


#include <cstdio>
#include <cstdlib>
#include "names.h"

class Edible
{
    public:
    unsigned int irrigation;
    unsigned int poison;
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

extern const char * Form_name[];

enum Class_id
{
    Class_Element=1,
    Class_Ingredient,
    Class_Product,
    Class_Plant,
};

enum Item_id
{
    // SOLID START HERE
    ID_STONE,
    ID_LOG,
    ID_LOG1,
    ID_LOG2,
    ID_SAND,
    ID_STICK,
    ID_STRAWBERRY_SEEDS,
    // LIQUID START HERE
    ID_WATER,
    // GAS START HERE
    ID_HELIUM,
    // FOOD START HERE
    ID_PUMPKIN,
    ID_WATERMELON,
    ID_CHERRY,
    ID_STRAWBERRY
};

class BaseElement
{
    public:
        const char * name;
        int id; // texture id 
        unsigned int density;
        Edible *edible;
        Form form;
        Solid *solid;
        
        BaseElement(int index); 
        void show(bool details=true);
};        
class Player;
class InventoryElement
{
	int x, y, z;
    public:
        Class_id c_id;
        Form req_form;
        bool known;
        InventoryElement() { req_form = Form_none; known = true; }
        virtual bool use(Player * player) { return false; }
        virtual void show(bool details=true) { }
        virtual bool tick() { return false;}
        virtual Form get_form() {return Form_none; }
        virtual const char * get_name() {return NULL; }
        virtual const char * get_form_name() { return NULL; }
        virtual int get_id() {return -1; }
        virtual Edible * get_edible() { return NULL; }
#ifndef STUB_SDL     
        virtual SDL_Texture * get_texture() { return NULL;}
#endif
        virtual bool craft() { 
            printf("missing craft function\n");
            return false; 
        }
        void set_posittion(int _x, int _y) { x=_x; y=_y; }
        void get_posittion(int *_x, int *_y) { *_x=x; *_y=y; }
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
        Edible * get_edible()
        {
            return base->edible;
        }
        
        void show(bool details=true);

        Element(BaseElement *b);
        Form get_form() {return base->form; }
        const char * get_name() {
            return known ? base->name : "unknown"; 
        }
        const char * get_form_name() { return Form_name[base->form]; }
        int get_id() {return base->id; }
#ifndef STUB_SDL     
        SDL_Texture * get_texture();
#endif
};

enum Ingredient_id
{
    ING_AXE_BLADE,
    ING_AXE_HANDLE,

    ING_KNIFE_BLADE,
    ING_KNIFE_HANDLE,
    
};

enum Product_id
{
    PROD_AXE,
    PROD_KNIFE
};

extern const char * Ingredient_name[];
extern const char * Product_name[];
extern const char * items_name[];
extern const char * food_name[];

class Ingredient : public InventoryElement
{
    const char * name;
    public:
        int quality; //[0..100] slaby..najlepszy
        int resilience; // [0..100] wytrzymały..słaby
        int usage; // [0..100] łatwy..trudny

        Ingredient_id id;
        InventoryElement * el;
        
        Form get_form() {return el->get_form(); }
        const char * get_name() {return name; }
        const char * get_form_name() { return Form_name[el->get_form()]; }
        int get_id() {return id; }
        bool craft();
        Edible * get_edible() {return el->get_edible();}
        Ingredient(InventoryElement * from, Ingredient_id i, Form f);
        void show(bool details=true);
#ifndef STUB_SDL     
        SDL_Texture * get_texture();
#endif        
};

class Product : public InventoryElement
{
    const char * name;
    void init(Product_id i, int c, Form f);
    public:
        int quality; //[0..100] slaby..najlepszy
        int resilience; // [0..100] wytrzymały..słaby
        int usage; // [0..100] łatwy..trudny

        Product_id id;
        
        int ing_count;
        InventoryElement ** ings;
        
        Form get_form(); 
        const char * get_name() {return name; }
        int get_id() {return id; }
        const char * get_form_name();

        Product(InventoryElement * el1, InventoryElement *el2, Product_id i, Form f);
        Product(InventoryElement ** from, int count, Product_id i, Form f);
       
        bool craft();
        virtual bool check_ing() { return false; }
        void show(bool details=true);
#ifndef STUB_SDL     
        SDL_Texture * get_texture();
#endif
};

enum being_types
{
    BEINGID_tree,
    BEINGID_tree1,
    BEINGID_tree2
};

enum plant_types
{
    PLANTID_strawberry
};

class Being : public InventoryElement
{
    public:
        const char * name;
        unsigned int age;
        unsigned int max_age;
        bool alive;
        enum being_types type;
#ifndef STUB_SDL
        SDL_Texture * get_texture();
#endif
        virtual bool grow() {
            if (!alive) return false;
            age++;
            if (age >= max_age) alive=false;
            return alive;
        }
        Being()
        {
            alive = true;
            max_age = 1 + rand() % 36000; //100 years
            age = rand() % max_age;
            name = create_name(5);
        }
        bool is_alive() { return alive; }
        const char * get_name() {return name; }
        void show(bool details=true) {
           printf("Being %s age=%d/%d alive=%d\n", name, age, max_age, alive);
        }
        bool tick() {
            return grow();
        }
};

enum Plant_phase
{
    Plant_seed=0,
    Plant_seedling,
    Plant_growing,
    Plant_flowers,
    Plant_fruits
};

extern const char * Plant_phase_name[];

class Plant: public Being
{    
    Edible * edible;
    unsigned int seedling_time;
    unsigned int growing_time;
    unsigned int flowers_time;
    unsigned int fruits_time;
public:
    bool planted;
    enum plant_types type;
#ifndef STUB_SDL
        SDL_Texture * get_texture();
#endif
    Plant_phase phase;
    Plant();
    void show(bool details=true) {
       printf("Plant -> %d name=%s ", c_id, name);
       Being::show(details);
       if (details) {
              printf("phase=%s planted=%d times=%d/%d/%d/%d\n",
                     Plant_phase_name[phase], planted, seedling_time, growing_time, flowers_time, fruits_time);
              edible->show();
       }
    }
    void sow() {
        planted=1;
    }
    void change_phase(Plant_phase p)
    {
        if (phase != p){
            printf("%s growing: %s -> %s\n", name, Plant_phase_name[phase], Plant_phase_name[p]);
        }
        phase=p;
    }
    bool grow();

};

#define SOLID_ELEMENTS 7
#define FOOD_ELEMENTS 4
#define LIQUID_ELEMENTS 1
#define GAS_ELEMENTS 1

#define NOT_FOOD_ELEMENTS (SOLID_ELEMENTS+LIQUID_ELEMENTS+GAS_ELEMENTS)

#define BASE_ELEMENTS (SOLID_ELEMENTS+FOOD_ELEMENTS+LIQUID_ELEMENTS+GAS_ELEMENTS)

#define ING_ELEMENTS 4
#define PROD_ELEMENTS 2

extern BaseElement *base_elements[BASE_ELEMENTS];

void init_elements();
void show_base_elements(bool details);

#endif

