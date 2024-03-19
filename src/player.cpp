#include "player.h"

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/classes/physics_ray_query_parameters3d.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/packed_scene.hpp>

#include "alchemist/axe.h"
#include "alchemist/axe_handle.h"
#include "tiles.h"

#include "terrain.h"
#include "player_input_sync.h"
#include "alchemist/axe_blade.h"

using namespace godot;

void PlayerCharacter::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_id", "p_id"), &PlayerCharacter::set_id);
    ClassDB::bind_method(D_METHOD("get_id"), &PlayerCharacter::get_id);
    ClassDB::add_property("PlayerCharacter", PropertyInfo(Variant::INT, "id"), "set_id", "get_id");

    ClassDB::bind_method(D_METHOD("set_speed", "p_speed"), &PlayerCharacter::set_speed);
    ClassDB::bind_method(D_METHOD("get_speed"), &PlayerCharacter::get_speed);
    ClassDB::add_property("PlayerCharacter", PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");

}

PlayerCharacter::PlayerCharacter() {
    x = y = z = 0;
    map_x = WORLD_CENTER;
    map_y = WORLD_CENTER;

    input_sync = nullptr;
    id = 1;
    move_direction = Vector2i(0, 0);
    speed = 10;
    total_pitch = 0;

	energy=250;
	back_x=0;
	back_y=0;
    health=1000;
    hunger=50;
    thirst=50;
    map_x = WORLD_CENTER;
    map_y = WORLD_CENTER;
    inventory = new InvList("inventory");

	for (int i=0; i < 10; i++)
    {
        hotbar[i]=NULL;
        craftbar[i]=0;
    }
    left_hand = right_hand = nullptr;

}

PlayerCharacter::~PlayerCharacter() {
}


void PlayerCharacter::_ready() {
    if (!Engine::get_singleton()->is_editor_hint()) {
        input_sync = get_node<PlayerInputSync>("InputSynchronizer");
        if (id == get_multiplayer()->get_unique_id()) {
            get_node<Camera3D>("Camera3D")->make_current();
        }
        Input::get_singleton()->set_mouse_mode(Input::MOUSE_MODE_CAPTURED);
    }
}

void PlayerCharacter::_process(double delta) {
    if (get_multiplayer()->is_server()) {
         if (!Engine::get_singleton()->is_editor_hint()) {

            move_direction = input_sync->move_direction;

            Vector3 v = get_velocity();
            if (is_on_floor()) {
                v = Vector3(move_direction.x, 0, move_direction.y);
                v.rotate(Vector3(0, 1, 0), get_rotation().y);
                v.normalize();
                v *= speed;
                v.y = get_velocity().y;
            }
            if (!is_on_floor()) {
                v.y -= 10*delta;
            }
            if (Input::get_singleton()->is_action_just_pressed("ui_accept") && is_on_floor()) {
                v.y = 4.5;
            }
            set_velocity(v);
            move_and_slide();
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
    Ref<InputEventKey> key_ev = event;
    if (key_ev.is_valid() && key_ev->is_pressed()) {
        if (key_ev->get_keycode() == KEY_1)
            craft_axe_blade();
        else if (key_ev->get_keycode() == KEY_2)
            craft_axe_handle();
        else if (key_ev->get_keycode() == KEY_3)
            craft_axe();
        else if (key_ev->get_keycode() == KEY_5)
            UtilityFunctions::print(right_hand->element->get_name());
        else if (key_ev->get_keycode() == KEY_6)
            UtilityFunctions::print(left_hand->element->get_name());

    }
    Ref<InputEventMouseMotion> motion = event;
    if (motion.is_valid()) {
        float yaw = motion->get_relative().x * 0.5;
        float pitch = motion->get_relative().y * 0.5;
        pitch = Math::clamp(pitch, -90 - total_pitch, 90-total_pitch);
        total_pitch += pitch;

        rotate_y(Math::deg_to_rad(-yaw));
        get_node<Camera3D>("Camera3D")->rotate_object_local(Vector3(1, 0, 0), Math::deg_to_rad(-pitch));
        //rotate_object_local(Vector3(1, 0, 0), Math::deg_to_rad(-pitch));

    }
    if (event->is_action_pressed("mine"))
    {
        Ref<InputEventMouseButton> button = event;
        PhysicsDirectSpaceState3D* space_state = get_world_3d()->get_direct_space_state();
        //Ref<PhysicsRayQueryParameters3D> parameters = memnew(PhysicsRayQueryParameters3D);
        Camera3D* camera = get_node<Camera3D>("Camera3D");
        Vector3 from = camera->project_ray_origin(button->get_position());
        Vector3 to = from + camera->project_ray_normal(button->get_position())* 100;
        Ref<PhysicsRayQueryParameters3D> parameters = PhysicsRayQueryParameters3D::create(from, to);
        TypedArray<RID> a;
        a.push_back(get_rid());
        parameters->set_exclude(a);
        Dictionary result = space_state->intersect_ray(parameters);
//        UtilityFunctions::print(result);
        UtilityFunctions::print(result["collider"], result["position"]);
        Node3D* node = Object::cast_to<Node3D>(result["collider"]);
        if (node) {
            Item* item = Object::cast_to<Item>(node);
            if (item) {
                UtilityFunctions::print("foo");
                pick_up(item, &left_hand);
            }

            Terrain* terrain = Object::cast_to<Terrain>(node->get_parent()->get_parent());
            if (terrain)
                terrain->mine(result);

            /*ChunkRenderer* chunk = Object::cast_to<ChunkRenderer>(node->get_parent());
            if (chunk) {
                Vector3 posf = result["position"];
                Vector3 normal = result["normal"];
                posf -= chunk->get_position();
                posf -= normal*0.5;
                Vector3i pos = posf.round();

                world_table[chunk->chunk_z][chunk->chunk_x]->table[pos.z][pos.y][pos.x].tile = TILE_AIR;
                chunk->render_self();
                UtilityFunctions::print(chunk->chunk_x, chunk->chunk_z);
            }*/
        }
    }
    if (event->is_action_pressed("place"))
    {
        Ref<InputEventMouseButton> button = event;
        PhysicsDirectSpaceState3D* space_state = get_world_3d()->get_direct_space_state();
        Camera3D* camera = get_node<Camera3D>("Camera3D");
        Vector3 from = camera->project_ray_origin(button->get_position());
        Vector3 to = from + camera->project_ray_normal(button->get_position())* 100;
        Ref<PhysicsRayQueryParameters3D> parameters = PhysicsRayQueryParameters3D::create(from, to);
        TypedArray<RID> a;
        a.push_back(get_rid());
        parameters->set_exclude(a);
        Dictionary result = space_state->intersect_ray(parameters);
        UtilityFunctions::print(result["collider"], result["position"]);
        Node3D* node = Object::cast_to<Node3D>(result["collider"]);
        if (node) {
            UtilityFunctions::print(node);
            Item* item = Object::cast_to<Item>(node);
            if (item) {
                UtilityFunctions::print("foo");
                pick_up(item, &right_hand);
            }
            Terrain* terrain = Object::cast_to<Terrain>(node->get_parent()->get_parent());
            if (terrain)
                terrain->place(result);
        }
        else {
            UtilityFunctions::print("not node");
        }
    }
}

void PlayerCharacter::set_id(int p_id) {
    id = p_id;
    PlayerInputSync* sync = get_node<PlayerInputSync>("InputSynchronizer");
    if (sync)
        set_multiplayer_authority(id);
}

int PlayerCharacter::get_id() const{
    return id;
}

void PlayerCharacter::set_speed(float p_speed) {
    speed = p_speed;
}

float PlayerCharacter::get_speed() {
    return speed;
}

void PlayerCharacter::pick_up(Item *item, Item** hand) {
    UtilityFunctions::print("picking up");
    if (!item->is_picked_up && *hand == nullptr) {
        UtilityFunctions::print("picked_up");
        item->is_picked_up = true;
        item->set_freeze_enabled(true);
        item->reparent(this);
        //item->set_collision_mask_value(1, false);
        //item->set_collision_mask_value(2, true);
        item->get_node<CollisionShape3D>("Collision")->set_disabled(true);
        if (hand == &right_hand)
            item->set_position(Vector3(0.5, 0, -1));
        else
            item->set_position(Vector3(-0.5, 0, -1));
        *hand = item;
    }
}

void PlayerCharacter::drop() {

}

void PlayerCharacter::craft_axe_blade() {
    if (!right_hand)
        return;
    InventoryElement * el = right_hand->element;
    if (el) {
        //sprintf(status_line, "crafting: axe blade from %s", el->get_name());
        UtilityFunctions::print("crafting: axe blade from %s", el->get_name());
    
        AxeBlade * axe_blade=new AxeBlade(el);
        if (axe_blade->craft()) {
            axe_blade->show();
            axe_blade->set_posittion(0, 0, 0);
            right_hand->queue_free();
            right_hand = nullptr;

            Ref<PackedScene> loaded = ResourceLoader::get_singleton()->load("res://rock.tscn");
            Item* blade = Object::cast_to<Item>(loaded->instantiate());
            get_parent()->add_child(blade);
            blade->set_position(get_position() + Vector3(2, 3, 0));
            blade->element = axe_blade;
            //pick_up(blade, &right_hand);
        } else delete axe_blade;
    }
}

void PlayerCharacter::craft_axe_handle() {
    if (!right_hand)
        return;
    InventoryElement * el = right_hand->element;
    if (el) {
        UtilityFunctions::print("crafting: axe handle from %s", el->get_name());

    
        AxeHandle * axe_handle=new AxeHandle(el);
        if (axe_handle->craft())
        {
            axe_handle->show();
            axe_handle->set_posittion(0,0, 0);
            right_hand->queue_free();
            right_hand = nullptr;

            Ref<PackedScene> loaded = ResourceLoader::get_singleton()->load("res://rock.tscn");
            Item* blade = Object::cast_to<Item>(loaded->instantiate());
            get_parent()->add_child(blade);
            blade->set_position(get_position() + Vector3(2, 3, 0));
            blade->element = axe_handle;
            //pick_up(blade, &right_hand);
        } else delete axe_handle;
    }
}

void PlayerCharacter::craft_axe() {
    if (!right_hand || !left_hand)
        return;
    InventoryElement *el1=right_hand->element, *el2=left_hand->element;
    if (el1 && el2) 
    {
        UtilityFunctions::print("crafting: axe from %s and %s", el1->get_name(), el2->get_name());
    
        Axe * axe=new Axe(el1, el2);
        if (axe->craft())
        {
            axe->show();
            axe->set_posittion(0, 0, 0);
            right_hand->queue_free();
            left_hand->queue_free();
            right_hand = nullptr;
            left_hand = nullptr;

            Ref<PackedScene> loaded = ResourceLoader::get_singleton()->load("res://rock.tscn");
            Item* blade = Object::cast_to<Item>(loaded->instantiate());
            get_parent()->add_child(blade);
            blade->set_position(get_position() + Vector3(2, 3, 0));
            blade->element = axe;
            //pick_up(blade, &right_hand);
        } else delete axe;
    }
}


