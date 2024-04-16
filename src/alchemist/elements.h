#ifndef __ELEMENTS__H
#define __ELEMENTS__H

#include <cstdio>

extern bool fantasy_game;

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

extern const char * Form_name[];

enum Class_id
{
    Class_Base=1,
    Class_Element,
    Class_Ingredient,
    Class_Product
};

class BaseElement
{
    public:
    static const Class_id c_id=Class_Base;
        const char * name;
        int id; //texture id
        unsigned int density;
        unsigned char transparency;
        Edible *edible;
        Form form;
        Solid *solid;
        
        BaseElement(); 
        void init_real();
        void init_fantasy();
        void show();
};        

class InventoryElement
{
	int x, y, z;
    public:
        Form req_form;
        bool known;
        InventoryElement() { req_form = Form_none; known = true; }
        virtual void show() {}
        virtual Form get_form() {return Form_none; }
        virtual const char * get_name() {return NULL; }
        virtual const char * get_form_name() { return NULL; }
        virtual int get_id() {return -1; }
        virtual bool craft() { return false; }
        void set_posittion(int _x, int _y, int _z) { x=_x; y=_y; z=_z; }
        void get_posittion(int *_x, int *_y, int *_z) { *_x=x; *_y=y; *_z=z; }
};


class Element : public InventoryElement
{
    BaseElement * base;
    public:
    static const Class_id c_id=Class_Element;
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
        const char * get_name() {
            if (!fantasy_game) return base->name;
            return known ? base->name : "unknown"; 
        }
        const char * get_form_name() { return Form_name[base->form]; }
        int get_id() {return base->id; }
};

enum Item_id
{
    ID_STONE,
    ID_LOG,
    ID_SAND,
    ID_STICK
};

enum Food_id
{
    ID_PUMPKIN,
    ID_WATERMELON
};

enum Ingredient_id
{
    ING_AXE_BLADE,
    ING_AXE_HANDLE,

    ING_HAMMER_HEAD,
    ING_HAMMER_HANDLE,

    ING_KNIFE_BLADE,
    ING_KNIFE_HANDLE,
    
};

enum Product_id
{
    PROD_AXE,
    PROD_HAMMER,
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
    static const Class_id c_id=Class_Ingredient;
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
        Ingredient(InventoryElement * from, Ingredient_id i, Form f);
        void show();
};

class Product : public InventoryElement
{
    const char * name;
    void init(Product_id i, int c, Form f);
    public:
    static const Class_id c_id=Class_Product;
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
        void show();


};
class Being : public Element
{
    public:
        unsigned int age;
        unsigned int max_age;
        void grow() {}
};
#define BASE_ELEMENTS 50
#define SOLID_ELEMENTS 4
#define FOOD_ELEMENTS 2
#define LIQUID_ELEMENTS 1
#define GAS_ELEMENTS 1
#define ING_ELEMENTS 2
#define PROD_ELEMENTS 1

extern BaseElement *base_elements[BASE_ELEMENTS];

void init_elements();
#endif

