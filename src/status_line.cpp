#include "status_line.h"
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;
LineEdit* status_line_node;

void status_line(String s)
{
    status_line_node->set_text(s);
}
