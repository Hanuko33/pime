#include <ctime>
#include <cstdio>
#include <cstdlib>

#include "elements.h"

int main()
{
    srandom(time(nullptr));
	Element *elements[10];
    for (int i=0; i< 10; i++)
    {
        int b = rand() % 5;
        elements[i] = new Element(&base_elements[b]);
        elements[i]->show();
    }
 
   return 0;
}
