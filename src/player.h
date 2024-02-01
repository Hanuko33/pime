#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/input_event_key.hpp>

extern "C" {
#include "../game_gui/player.h"
}
#include "player_input_sync.h"

namespace godot {

class PlayerCharacter : public Node2D {
    GDCLASS(PlayerCharacter, Node2D)

private:
    PlayerInputSync* input_sync;
    int id;
    Vector2i move_direction;

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
    
protected:
    static void _bind_methods();

};
}

#endif
