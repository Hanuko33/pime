#ifndef __ELEMENTS__H
#define __ELEMENTS__H

#include <cstdio>

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
    Form_solid=1,
    Form_liquid,
    Form_gas,
};

class BaseElement
{
    public:
        char * name;
        int id;
        unsigned int density;
        unsigned char transparency;
        Edible *edible;
        Form form;
        Solid *solid;
        
        BaseElement(); 
        void show();
};        

class Element
{
    public:
        BaseElement * base;
        unsigned int sharpness;
        unsigned int smoothness;
        unsigned int mass; //density*volume
        unsigned int length;
        unsigned int width;
        unsigned int height;
        unsigned int volume; //lenght*width*height
    
		int x, y, z;

        Element(BaseElement *b);
        void show();
};

class Being : public Element
{
    public:
        unsigned int age;
        unsigned int max_age;
        void grow() {}
};
#define BASE_ELEMENTS 7

extern BaseElement base_elements[BASE_ELEMENTS];

void init_elements();
#endif

