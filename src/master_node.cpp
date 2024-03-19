#include "master_node.h"
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/e_net_multiplayer_peer.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/button.hpp>

using namespace godot;

void MasterNode::_bind_methods() {
    ClassDB::bind_method(D_METHOD("host_game"), &MasterNode::host_game);
    ClassDB::bind_method(D_METHOD("join_game"), &MasterNode::join_game);
    ClassDB::bind_method(D_METHOD("change_level", "p_scene"), &MasterNode::change_level);
}

void MasterNode::_ready() {
    //get_tree()->set_pause(true);
    Button* b = get_node<Button>("UI/VBoxContainer/HBoxContainer/Button");
    if (b) b->connect("button_down", Callable(this, "host_game"));
    else UtilityFunctions::print("no button in scene");
    UtilityFunctions::print(b);
    b = get_node<Button>("UI/VBoxContainer/HBoxContainer/Button2");
    if (b) b->connect("button_down", Callable(this, "join_game"));

}

void MasterNode::start_game() {
    get_node<Control>("UI")->hide();
    get_tree()->set_pause(false);

    if (get_multiplayer()->is_server()) {
        ResourceLoader* loader = ResourceLoader::get_singleton();
        Ref<PackedScene> loaded = loader->load("res://test.tscn");
        call_deferred("change_level", loaded);

    }
}

void MasterNode::host_game() {
    UtilityFunctions::print("host");
    Ref<ENetMultiplayerPeer> peer = memnew(ENetMultiplayerPeer);
    peer->create_server(4433);
    if (peer->get_connection_status() == MultiplayerPeer::CONNECTION_DISCONNECTED) {
        OS::get_singleton()->alert("Failed to start server");
        return;
    }
    get_multiplayer()->set_multiplayer_peer(peer);
    start_game();
}

void MasterNode::join_game() {
    String ip = get_node<LineEdit>("UI/VBoxContainer/HBoxContainer/Remote")->get_text();
    if (ip == "") {
        OS::get_singleton()->alert("need a remote to connect");
        return;
    }
    Ref<ENetMultiplayerPeer> peer = memnew(ENetMultiplayerPeer);
    peer->create_client(ip, 4433);
    if (peer->get_connection_status() == MultiplayerPeer::CONNECTION_DISCONNECTED) {
        OS::get_singleton()->alert("failed to start client");
        return;
    }
    get_multiplayer()->set_multiplayer_peer(peer);
    start_game();
}

void MasterNode::change_level(Ref<PackedScene> scene) {
    Node *level = get_node<Node>("Level");
    TypedArray<Node> children = level->get_children();
    for (int i = children.size()-1; i >= 0; i--) {
        Node* c = Object::cast_to<Node>(children[i]);
        level->remove_child(c);
        c->queue_free();
    }
    level->add_child(scene->instantiate());
}

MasterNode::MasterNode() {}
MasterNode::~MasterNode() {}



