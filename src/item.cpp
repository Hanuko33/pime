#include "item.h"
#include "alchemist/elements.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/box_mesh.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/convex_polygon_shape3d.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>

using namespace godot;

void Item::_bind_methods() {

}

Item::Item() : Item(base_elements[rand() % BASE_ELEMENTS]) {}

Item::Item(BaseElement* base_element) : Item(base_element, Vector3(1, 1, 1)) { }
 
Item::Item(BaseElement* base_element, Vector3 size) {
    UtilityFunctions::print("item::item");
    element = new Element(base_element);
    element->known = true;
    is_picked_up = false;

    MeshInstance3D* mesh = memnew(MeshInstance3D);
    BoxMesh* box = memnew(BoxMesh);
    box->set_size(Vector3(size));
    mesh->set_mesh(box);
    //Ref<StandardMaterial3D> mat = memnew(StandardMaterial3D);
    //mat->set_albedo(base_element->color);
    mesh->get_mesh()->surface_set_material(0, base_element->godot_material);
    
    add_child(mesh);
    CollisionShape3D* shape = memnew(CollisionShape3D);
    shape->set_shape(mesh->get_mesh()->create_convex_shape());
    shape->set_name("Collision");
    add_child(shape);
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
