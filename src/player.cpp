#include "player.h"

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/camera2d.hpp>

extern "C" {
#include "../game_gui/tiles.h"
}
#include "player_input_sync.h"

using namespace godot;

void PlayerCharacter::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_id", "p_id"), &PlayerCharacter::set_id);
    ClassDB::bind_method(D_METHOD("get_id"), &PlayerCharacter::get_id);
    ClassDB::add_property("PlayerCharacter", PropertyInfo(Variant::INT, "id"), "set_id", "get_id");

}

PlayerCharacter::PlayerCharacter() {
    player.x = player.y = player.z = 0;
    player.map_x = WORLD_CENTER;
    player.map_y = WORLD_CENTER;

    input_sync = nullptr;
    id = 1;
    move_direction = Vector2i(0, 0);
}

PlayerCharacter::~PlayerCharacter() {
}


void PlayerCharacter::_ready() {
    input_sync = get_node<PlayerInputSync>("InputSynchronizer");
    if (id == get_multiplayer()->get_unique_id()) {
        get_node<Camera2D>("Camera2D")->make_current();
    }
}

void PlayerCharacter::_process(double delta) {
    if (get_multiplayer()->is_server()) {
         if (!Engine::get_singleton()->is_editor_hint()) {
            set_position(Vector2((CHUNK_SIZE * (player.map_x-128) + player.x)*32, (CHUNK_SIZE * (player.map_y-128) + player.y)*32));
            move_direction = input_sync->move_direction;
            move_player(&player, move_direction.x, move_direction.y);
           /* Input* in = Input::get_singleton();
            if (in->is_action_pressed("ui_accept")) {
                UtilityFunctions::print("accept");
            } else if (in->is_action_pressed("ui_left")) {
                player_move(&player, -1, 0);
            } else if (in->is_action_pressed("ui_right")) {
                player_move(&player, 1, 0);
            } else if (in->is_action_pressed("ui_up")) {
                player_move(&player, 0, -1);
            } else if (in->is_action_pressed("ui_down")) {
                player_move(&player, 0, 1);
            }*/

           // UtilityFunctions::print(player.map_x, player.map_y);
           // UtilityFunctions::print(get_position());
        }
    }
}

void PlayerCharacter::_input(const Ref<InputEvent> &event) {
    Ref<InputEventKey> ev = event;
    if (ev.is_valid()) {
        
    }
}

void PlayerCharacter::set_id(int p_id) {
    id = p_id;
    get_node<PlayerInputSync>("InputSynchronizer")->set_multiplayer_authority(id);
}

int PlayerCharacter::get_id() const{
    return id;
}
