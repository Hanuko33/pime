#ifndef TERRAIN_H
#define TERRAIN_H

//#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/array_mesh.hpp>

#include "../game_gui/world.h"

#include "chunk_renderer.h"

namespace godot {

class Terrain : public Node3D {
    GDCLASS(Terrain, Node3D)

public:
    Terrain();
    ~Terrain();

    void _ready() override;
    void _process(double delta) override;
    //ArrayMesh array_mesh;
    //Array mesh_data;
    //void set_mesh_data(Array a);
    //Array get_mesh_data();
    void generate_chunk(int x, int y);
    int foo_x;
    int foo_z;
    ChunkRenderer* chunks[WORLD_SIZE][WORLD_SIZE];

    void mine(Dictionary& result);
    void place(Dictionary& result);

    
protected:
    static void _bind_methods();

};
}

#endif
