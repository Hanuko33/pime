#ifndef BUSH_H
#define BUSH_H

#include "alchemist/elements.h"
#include "ftree.h"
#include "item.h"

namespace godot {

class Bush : public FTree {
    GDCLASS(Bush, FTree)

public:
    Bush();
    Bush(BaseElement* fruit_type);
    ~Bush();

    Item* collect();
    BaseElement* fruit_type;
    
protected:
    static void _bind_methods();

};
}

#endif
