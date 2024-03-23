#ifndef STATUS_LINE_H
#define STATUS_LINE_H
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/classes/line_edit.hpp>

//namespace godot {
void status_line(godot::String s);
extern godot::LineEdit* status_line_node;
//}

#endif // STATUS_LINE_H
