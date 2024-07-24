#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/input_event_key.hpp>

#include "player_input_sync.h"
#include "alchemist/el_list.h"
#include "alchemist/player_stats.h"

#include "item.h"

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

    //void status_line(String s);
    void _ready() override;
    void _process(double delta) override;
    void _physics_process(double delta) override;
    void _input(const Ref<InputEvent> &event) override;

    Node3D* looking_at;
    Vector3 looking_pos;
    Vector3 looking_norm;

    void eat(Item* thing);

    float speed;
    float total_pitch;
    void set_speed(float p_speed);
    float get_speed();

    float mouse_sensitivity;
    void set_mouse_sen(float s);
    float get_mouse_sen();

    //Player player;
    // game_gui properties
    int map_y;
    int map_x;
    int z;
    int y;
    int x;
	int back_y;
	int back_x;
    char running;
    char sneaking;
    char going_right;
    InvList * inventory; 
    InventoryElement * hotbar[10];
    int craftbar[10];
    Player_stats stats;
    Item* left_hand;
    Item* right_hand;

    void pick_up(Item* item, Item** hand);
    void drop(Item** hand);

    void craft_axe_blade();
    void craft_axe_handle();
    void craft_axe();


protected:
    static void _bind_methods();

};
}

#endif
