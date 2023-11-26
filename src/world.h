#ifndef WORLD_H
#define WORLD_H

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {

class World : public Node2D {
    GDCLASS(World, Node2D)

public:
    World();
    ~World();
    
protected:
    static void _bind_methods();

};
}

#endif
