#include "player_input_sync.h"

#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/scene_replication_config.hpp>

using namespace godot;

void PlayerInputSync::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_attack_direction", "p_attack_direction"), &PlayerInputSync::set_attack_direction);
    ClassDB::bind_method(D_METHOD("get_attack_direction"), &PlayerInputSync::get_attack_direction);
    ClassDB::add_property("PlayerInputSync", PropertyInfo(Variant::VECTOR2, "attack_direction"), "set_attack_direction", "get_attack_direction");

    ClassDB::bind_method(D_METHOD("set_move_direction", "p_move_direction"), &PlayerInputSync::set_move_direction);
    ClassDB::bind_method(D_METHOD("get_move_direction"), &PlayerInputSync::get_move_direction);
    ClassDB::add_property("PlayerInputSync", PropertyInfo(Variant::VECTOR2, "move_direction"), "set_move_direction", "get_move_direction");    

    ClassDB::bind_method(D_METHOD("set_action", "p_action"), &PlayerInputSync::set_action);
    ClassDB::bind_method(D_METHOD("get_action"), &PlayerInputSync::get_action);
    ClassDB::add_property("PlayerInputSync", PropertyInfo(Variant::INT, "action"), "set_action", "get_action");

    BIND_ENUM_CONSTANT(ACTION_NONE)
    BIND_ENUM_CONSTANT(ACTION_ATTACK)
}

Vector2 PlayerInputSync::get_move_direction() const{
    return move_direction;
}

void PlayerInputSync::set_move_direction(const Vector2 p_move_direction) {
    move_direction = p_move_direction;
}

Vector2 PlayerInputSync::get_attack_direction() const{
    return attack_direction;
}

void PlayerInputSync::set_attack_direction(const Vector2 p_attack_direction) {
    attack_direction = p_attack_direction;
}

PlayerInputSync::Action PlayerInputSync::get_action() const {
    return action;
}

void PlayerInputSync::set_action(Action p_action) {
    action = p_action;
}

void PlayerInputSync::_ready() {
    set_process(get_multiplayer_authority() == get_multiplayer()->get_unique_id());
    parent = Object::cast_to<Node2D>(get_parent());
}

void PlayerInputSync::_process(double delta) {
    move_direction = input->get_vector("ui_left", "ui_right", "ui_up", "ui_down");
//    UtilityFunctions::print(get_multiplayer()->is_server(), get_parent(), this, attack_direction);
/*    attack_direction = parent->get_global_mouse_position() - parent->get_position();
    move_direction = Vector2(0, 0);
    if (input->is_action_pressed("move_up")) {
        move_direction.y -= 1;
    }
    if (input->is_action_pressed("move_down")) {
        move_direction.y += 1;
    }
    if (input->is_action_pressed("move_right_up")) {
        move_direction += Vector2(3, -2);
    }
    if (input->is_action_pressed("move_right_down")) {
        move_direction += Vector2(3, 2);
    }
    if (input->is_action_pressed("move_left_up")) {
        move_direction += Vector2(-3, -2);
    }
    if (input->is_action_pressed("move_left_down")) {
        move_direction += Vector2(-3, 2);
    }
    if (input->is_action_pressed("attack")) {
        action = ACTION_ATTACK;
    } else {
        action = ACTION_NONE;
    }*/
}

void PlayerInputSync::_enter_tree() {
   // set_multiplayer_authority(Object::cast_to<CharacterBase>(get_parent())->get_player());
}

PlayerInputSync::PlayerInputSync() {
    /*set_root_path(".");
    Ref<SceneReplicationConfig> config = memnew(SceneReplicationConfig);//get_replication_config();
    config->add_property(":attack_direction");
    config->add_property(":move_direction");
    config->add_property(":action");
    set_replication_config(config);*/

    move_direction = Vector2(0, 0);
    attack_direction = Vector2(0, 0);
    parent = nullptr;
    input = Input::get_singleton();
}

PlayerInputSync::~PlayerInputSync() {
}
