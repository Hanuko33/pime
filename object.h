#ifndef OBJECT_H
#define OBJECT_H

enum object_types
{
    OBJECT_NULL,
    OBJECT_TREE,
    OBJECT_WALL,
    OBJECT_DOOR,
    OBJECT_MAX
};


struct object {
    int base_element_id;
    enum object_types type;
    int x, y;
};

#endif
