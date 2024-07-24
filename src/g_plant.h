#ifndef G_PLANT_H
#define G_PLANT_H

#include <godot_cpp/classes/static_body3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {

class GPlant : public StaticBody3D {
    GDCLASS(GPlant, StaticBody3D)

public:
    GPlant();
    ~GPlant();
    
protected:
    static void _bind_methods();

};
}

#endif
