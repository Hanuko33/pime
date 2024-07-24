#include "g_plant.h"
#include <godot_cpp/classes/box_mesh.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/convex_polygon_shape3d.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>

using namespace godot;

void GPlant::_bind_methods() {

}

GPlant::GPlant() {
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
}

GPlant::~GPlant() {
}

