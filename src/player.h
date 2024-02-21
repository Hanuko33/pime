#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/input_event_key.hpp>

#include "../game_gui/player.h"
#include "player_input_sync.h"

namespace godot {

class PlayerCharacter : public CharacterBody3D {
    GDCLASS(PlayerCharacter, CharacterBody3D)

private:
    PlayerInputSync* input_sync;
    int id;
    Vector2 move_direction;

public:
    void set_id(int p_id);
    int get_id() const;

public:
    PlayerCharacter();
    ~PlayerCharacter();

    void _ready() override;
    void _process(double delta) override;
    void _input(const Ref<InputEvent> &event) override;

    Player player;

    float speed;
    float total_pitch;
    void set_speed(float p_speed);
    float get_speed();
    
protected:
    static void _bind_methods();

};
}

#endif
