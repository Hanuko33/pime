#ifndef TERRAIN_H
#define TERRAIN_H

//#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/array_mesh.hpp>

#include "chunk_renderer.h"
#include "tiles.h"
#include "player.h"
#include "OpenSimplex/OpenSimplex2F.h"

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

    void mine(Vector3 pos, Vector3 norm);
    bool place(Vector3 pos, Vector3 norm, BaseElement* element);

    // game_gui properties
    struct OpenSimplex2F_context *simplex_context;
    struct OpenSimplex2F_context *simplex_context2;
    struct OpenSimplex2F_context *simplex_context3;

    chunk * world_table[WORLD_SIZE][WORLD_SIZE];

    /*enum game_tiles get_tile_at(int chunk_x, int chunk_y, int x, int y, int z);
    enum game_tiles get_tile_at_ppos(PlayerCharacter *player);

    InventoryElement ** get_item_at_ppos(PlayerCharacter *player);
    InventoryElement **get_item_at(int chunk_x, int chunk_y, int x, int y, int z);

    void set_item_at(InventoryElement * item, int chunk_x, int chunk_y, int x, int y, int z);
    void set_item_at_ppos(InventoryElement * item, PlayerCharacter* player);
*/
    void generator();
    void generate_chunk2(chunk* chunk, int chunk_x, int chunk_y);
    char load_chunk(int x, int y);
    int height_at(int chunk_x, int chunk_y, int x, int y);


    
protected:
    static void _bind_methods();

};
}

#endif
