#ifndef WORLD_GODOT_H
#define WORLD_GODOT_H

#include <godot_cpp/classes/tile_map.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {

class World : public TileMap {
    GDCLASS(World, TileMap)
private:


public:
    Dictionary map;
    void refresh(Dictionary p_map);
    void refresh_chunk(int id, Dictionary p_map);
    void init_peer_map(int id);

    World();
    ~World();

    void _process(double delta) override;
    void _ready() override;
    void _notification(int p_what);

    
protected:
    static void _bind_methods();

};
}

#endif
