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
    enum object_types type;
    int x, y, z;
};



#endif
