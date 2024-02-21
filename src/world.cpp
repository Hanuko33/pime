#include "world.h"
//#include "../game_gui/world.c"
//#include "../game_gui/player.c"
#include "../game_gui/world.h"
#include "../game_gui/notifier.h"
#include "../game_gui/dungeon.h"
#include "../game_gui/cave.h"
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/multiplayer_peer.hpp>
#include <godot_cpp/classes/engine.hpp>


using namespace godot;

World* world_p;
//struct Player player;
//kvnfsdiujvbnedobvntoi

void World::_bind_methods() {
    ClassDB::bind_method(D_METHOD("refresh"), &World::refresh);
//    ClassDB::bind_method(D_METHOD("refresh_peer", "id"), &World::refresh_peer);
    ClassDB::bind_method(D_METHOD("init_peer_map", "id"), &World::init_peer_map);

}

World::World() {
    world_p = this;

}

World::~World() {
}

void World::_ready() {
    if (!Engine::get_singleton()->is_editor_hint()) {
        if (get_multiplayer()->is_server()) {
            srand(time(NULL));
            generator();
            get_multiplayer()->connect("peer_connected", Callable(this, "init_peer_map"));
        }
        //UtilityFunctions::print("dziala");
    }
    //dungeon_generator();
    //cave_generator();
    //screen_list = &cave_terrain_list;

}


void World::_process(double delta) {
    if (!Engine::get_singleton()->is_editor_hint()) {
//        player_move(&player, 1, 0);
    }
}

void World::_notification(int p_what) {
    if (p_what == NOTIFICATION_READY) {
        Dictionary opts;
        opts["rpc_mode"] = MultiplayerAPI::RPC_MODE_AUTHORITY;
        opts["transfer_mode"] = MultiplayerPeer::TRANSFER_MODE_RELIABLE;
        opts["call_local"] = false;
        opts["channel"] = 0;
        rpc_config("refresh", opts);
    }
}

void World::init_peer_map(int id) {
    rpc_id(id, "refresh", map);
}

void World::refresh(Dictionary p_map) {
    map = p_map;
    Array k = map.keys();
    for (int i = 0; i < k.size(); i++) {
        Vector2i loc = k[i];
        set_cell(0, k[i], 1, Vector2i(map[k[i]], 0));
        //UtilityFunctions::print(map[k[i]]);
        
    }
    //set_cell(0, Vector2i(0, 0), Vector2i())
}

void World::refresh_chunk(int id, Dictionary p_map) {
    rpc_id(id, "refresh", p_map);

}

void notify_load_chunk(int map_x, int map_y) {
/*    Dictionary chunk_map;
    for (int y = 0; y < CHUNK_SIZE; y++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            //UtilityFunctions::print(get_tile_at(map_x, map_y, x, y, 0));
            world_p->set_cell(0, Vector2i((map_x - WORLD_CENTER) * CHUNK_SIZE + x, (map_y - WORLD_CENTER) * CHUNK_SIZE + y), 1, Vector2i(get_tile_at(map_x, map_y, x, y, 0), 0));
            world_p->map[Vector2i((map_x - WORLD_CENTER) * CHUNK_SIZE + x, (map_y - WORLD_CENTER) * CHUNK_SIZE + y)] = get_tile_at(map_x, map_y, x, y, 0);
            chunk_map[Vector2i((map_x - WORLD_CENTER) * CHUNK_SIZE + x, (map_y - WORLD_CENTER) * CHUNK_SIZE + y)] = get_tile_at(map_x, map_y, x, y, 0);
        }
    }
    for (auto p : world_p->get_multiplayer()->get_peers()) {
        world_p->refresh_chunk(p, chunk_map);
    }
*/
}
