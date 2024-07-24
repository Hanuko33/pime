#include "bush.h"
#include "item.h"

using namespace godot;

void Bush::_bind_methods() {

}

Bush::Bush() {
}

Bush::Bush(BaseElement* fruit_type): fruit_type(fruit_type) {
}

Bush::~Bush() {
}

Item* Bush::collect() {
    Item* fruit = memnew(Item(fruit_type));
    ((Element*)(fruit->element))->mass = rand() % 20;
    add_child(fruit);
    
    return fruit;
}
