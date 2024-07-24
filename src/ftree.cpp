#include "ftree.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/cylinder_mesh.hpp>
#include <godot_cpp/classes/static_body3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/box_shape3d.hpp>

using namespace godot;

FTree::FTree()
{
    child=1;
}

void FTree::_bind_methods() {

}

void FTree::_ready()
{
    if (Engine::get_singleton()->is_editor_hint()) {
        set_process_mode(PROCESS_MODE_DISABLED); // disable calling process method    
    }
    else
    {
        Ref<BoxShape3D> box = memnew(BoxShape3D);
        box->set_size(Vector3(1, 1, 1));
        CollisionShape3D* shape = memnew(CollisionShape3D);
        add_child(shape);
        shape->set_shape(box);

        set_process_mode(PROCESS_MODE_INHERIT);
        Stozek *s=memnew(Stozek(0, 0, 0, 0, 0, 0, Color(60.0/255, 50.0/255, 0,0)));
        add_child(s);
    }
}

void FTree::add_branch(Stozek * trunk)
{
    float r=(60 + trunk->level * 2 )/255.0;
    float g=(50 + trunk->level * 2 )/255.0;
    float b=0;//(rand() % 256)/255.0;

    int angle_x=20+rand() % 30;
    int angle_y=rand() % 360;
    int angle_z=20+rand() % 30;

    int branches=3 + rand() % 5;
    Vector3 rot=trunk->get_rotation_degrees();
    for (int i=0; i < branches; i++)
    {
        int signx = 1 - 2 * (rand() % 2);
        int signy = 1 - 2 * (rand() % 2);
        int signz = 1 - 2 * (rand() % 2);

        add_child(memnew(Stozek(trunk->mid_pos.x, trunk->mid_pos.y, trunk->mid_pos.z, rot.x+signx*angle_x, rot.y+signy*angle_y, rot.z+ signz * angle_z,
        Color(r, g, b, 0), trunk->level+1, 1.0/(trunk->level+2))));
    }

/*
    rot=s1->get_rotation_degrees();

    Stozek *s3=memnew(Stozek(s1->mid_pos.x, s1->mid_pos.y, s1->mid_pos.z, rot.x, rot.y, rot.z+45, Color(1, 1, 0,0), 3, 0.25));
    lsystem->add_child(s3);
    Stozek *s4=memnew(Stozek(s1->mid_pos.x, s1->mid_pos.y, s1->mid_pos.z, rot.x, rot.y, rot.z-45, Color(1, 1, 0,0), 3, 0.25));
    lsystem->add_child(s4);

*/
}

void FTree::_process(double p_delta)
{
    static int cnt=0;
    static int sec=0;

    cnt++;

    if (!(cnt % 10))
    {
        sec++;

        if (sec < 10)
        {
           Stozek *trunk=cast_to<Stozek>(get_child(child));
           add_branch(trunk);
           child++;
           set_scale(get_scale() * 1.01);
        }
    }
}


void FTree::_input(const Ref<InputEvent> &event) {
    Ref<InputEventKey> key = event;
    if (!key.is_valid()) return;

    if (!key->is_pressed()) return;

        switch(key->get_keycode())
        {
           default: break;
        }

}
