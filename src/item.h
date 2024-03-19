#ifndef ITEM_H
#define ITEM_H

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>

#include "alchemist/elements.h"


namespace godot {

class Item : public RigidBody3D {
    GDCLASS(Item, RigidBody3D)

public:
    Item();
    ~Item();

    InventoryElement* element;

    void _physics_process(double delta) override;
    bool is_picked_up;
    
protected:
    static void _bind_methods();

};
}

#endif
