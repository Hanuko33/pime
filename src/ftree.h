#ifndef FTREE_H
#define FTREE_H
#include <godot_cpp/classes/static_body3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include "stozek.h"

namespace godot {
class FTree : public StaticBody3D
{
    GDCLASS(FTree, StaticBody3D);
public:
    FTree();
    int child;
    void _ready() override;
    void _process(double p_delta) override;
    void _input(const Ref<InputEvent> &event) override;
    void add_branch(Stozek * trunk);
protected:
    static void _bind_methods();

};
}

#endif // FTREE_H
