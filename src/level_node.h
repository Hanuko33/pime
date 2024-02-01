#ifndef LEVEL_NODE_H
#define LEVEL_NODE_H

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot {

class LevelNode : public Node2D {
    GDCLASS(LevelNode, Node2D)

public:
    void _ready() override;
    void _exit_tree() override;
    void add_player(int id);
    void del_player(int id);

    LevelNode();
    ~LevelNode();


protected:
    static void _bind_methods();

};

}

#endif
