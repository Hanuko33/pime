#include "level_node.h"

#include "player.h"

#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/core/math.hpp>
#include <godot_cpp/variant/string.hpp>

using namespace godot;

void LevelNode::_bind_methods() {
    ClassDB::bind_method(D_METHOD("add_player", "p_id"), &LevelNode::add_player);
    ClassDB::bind_method(D_METHOD("del_player", "p_id"), &LevelNode::del_player);

}

void LevelNode::_ready() {
    Ref<MultiplayerAPI> multi = get_multiplayer();
    if (!get_multiplayer()->is_server())
        return;
    multi->connect("peer_connected", Callable(this, "add_player"));
    multi->connect("peer_disconnected", Callable(this, "del_player"));

    for (auto p : multi->get_peers()) {
        add_player(p);
    }
    if (!OS::get_singleton()->has_feature("dedicated server")) {
        add_player(1);
    }
}

void LevelNode::_exit_tree() {
    if (!get_multiplayer()->is_server())
        return;
    Ref<MultiplayerAPI> multi = get_multiplayer();
    multi->disconnect("peer_connected", Callable(this, "add_player"));
    multi->disconnect("peer_disconnected", Callable(this, "del_player"));
}

void LevelNode::add_player(int id) {
    Ref<PackedScene> loaded = ResourceLoader::get_singleton()->load("res://player.tscn");
    PlayerCharacter* character = Object::cast_to<PlayerCharacter>(loaded->instantiate());
    character->set_id(id);
    //Ref<RandomNumberGenerator> rng = memnew(RandomNumberGenerator);
    //Vector2 pos = Vector2::from_angle(rng->randf() * 2 * Math_PI);
    //character->set_position(pos*rng->randf_range(1, 50));
    character->set_name(String::num_int64(id));
    Node* players = get_node<Node>("Players");
    if (players) players->add_child(character, true);
    
    
}

void LevelNode::del_player(int id) {
    Node* players = get_node<Node>("Players");
    if (!players || !players->has_node(String::num_int64(id))) {
        return;
    }
    players->get_node<Node>(String::num_int64(id))->queue_free();
}

LevelNode::LevelNode() {

}

LevelNode::~LevelNode() {

}
