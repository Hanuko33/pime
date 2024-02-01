#ifndef PLAYER_INPUT_SYNC_H
#define PLAYER_INPUT_SYNC_H

#include <godot_cpp/classes/multiplayer_synchronizer.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/input.hpp>

namespace godot {

class PlayerInputSync : public MultiplayerSynchronizer {
    GDCLASS(PlayerInputSync, MultiplayerSynchronizer)

public:
    enum Action {
        ACTION_NONE,
        ACTION_ATTACK
    };

    Vector2 move_direction;
    Vector2 attack_direction;
    Action action;
    Node2D* parent;
    Input *input;

    Vector2 get_move_direction() const;
    void set_move_direction(const Vector2 p_move_direction);
    Vector2 get_attack_direction() const;
    void set_attack_direction(const Vector2 p_attack_direction);
    Action get_action() const;
    void set_action(Action p_action);


    void _ready() override;
    void _process(double delta) override;
    void _enter_tree() override;

    PlayerInputSync();
    ~PlayerInputSync();

protected:
    static void _bind_methods();

};

}

VARIANT_ENUM_CAST(PlayerInputSync::Action)

#endif
