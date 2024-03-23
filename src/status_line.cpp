#include "status_line.h"
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;
LineEdit* status_line_node;

void status_line(String s)
{
  /*
   static Engine *engine=Engine::get_singleton();
    Node * node=(Node*)engine->get_singleton("Node3D");
    Node3D *rootNode = getRootNode(anyNode);

    SceneTree *tree = SceneTree::get_singleton();
      Node *rootNode = (Node*) tree->get_root();
*/

    status_line_node->set_text(s);
}
