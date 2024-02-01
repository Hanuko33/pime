#ifndef MASTER_NODE_H
#define MASTER_NODE_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/packed_scene.hpp>

namespace godot {

class MasterNode : public Node {
    GDCLASS(MasterNode, Node)

public:
    void _ready() override;
    void start_game();
    void host_game();
    void join_game();
    void change_level(Ref<PackedScene> scene);

    MasterNode();
    ~MasterNode();

protected:
    static void _bind_methods();

};
}

#endif
