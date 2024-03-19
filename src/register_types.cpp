#include "register_types.h"


#include "player.h"
#include "master_node.h"
#include "level_node.h"
#include "player_input_sync.h"
#include "terrain.h"
#include "chunk_renderer.h"
#include "item.h"
// includes go here

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

void initialize_example_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

    
    ClassDB::register_class<PlayerCharacter>();
    ClassDB::register_class<MasterNode>();
    ClassDB::register_class<LevelNode>();
    ClassDB::register_class<PlayerInputSync>();
    ClassDB::register_class<Terrain>();
    ClassDB::register_class<ChunkRenderer>();
    ClassDB::register_class<Item>();
    //     ClassDB::register_class<class_name>();

}

void uninitialize_example_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT example_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
    godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

    init_obj.register_initializer(initialize_example_module);
    init_obj.register_terminator(uninitialize_example_module);
    init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

    return init_obj.init();
}
}
