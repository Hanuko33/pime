#include "item.h"
#include "alchemist/elements.h"
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void Item::_bind_methods() {

}

Item::Item() {
    UtilityFunctions::print("item::item");
    element = new Element(base_elements[rand() % BASE_ELEMENTS]);
    element->known = true;
    is_picked_up = false;
}

Item::~Item() {
}

void Item::_physics_process(double delta) {
    //if (!Engine::get_singleton()->is_editor_hint())
    //    set_linear_velocity(get_linear_velocity() + Vector3(0, -10, 0)*delta);
 //   set_linear_velocity(Vector3(0, 0, 0));
 //   Vector3 pos = get_position();
 //   pos.y = 16;
 //   set_position(pos);
//    add_constant_force(Vector3(0, -1, 0));
}
