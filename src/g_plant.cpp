#include "g_plant.h"
#include "alchemist/elements.h"
#include <cstring>
#include <godot_cpp/classes/box_mesh.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/convex_polygon_shape3d.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>
#include "item.h"

using namespace godot;

void GPlant::_bind_methods() {

}

GPlant::GPlant() {
}

GPlant::GPlant(BaseElement * base, BaseElement * seed)
{
    MeshInstance3D* mesh = memnew(MeshInstance3D);
    Ref<BoxMesh> box = memnew(BoxMesh);
    
    box->set_size(Vector3(1,1,1));
    mesh->set_mesh(box);

    add_child(mesh);

    Ref<StandardMaterial3D> mat = memnew(StandardMaterial3D);
    mat->set_albedo(Color(0,2550000,0));
    mesh->get_mesh()->surface_set_material(0, mat);

    CollisionShape3D* shape = memnew(CollisionShape3D);

    shape->set_shape(mesh->get_mesh()->create_convex_shape());

    add_child(shape);

    this->base = base;
    seed->edible=nullptr;
    this->seed = seed;
}

void GPlant::collect() {
    Item* it = memnew(Item(base));
    get_parent()->add_child(it);
    it->set_position(get_position());

    Item* s = memnew(Item(seed));
    get_parent()->add_child(s);
    s->set_position(get_position()+Vector3(0,1,0));

    queue_free();
}


GPlant::~GPlant() {
}

