#include "terrain.h"
#include "OpenSimplex/OpenSimplex2F.h"
#include "chunk_renderer.h"
#include "notifier.h"
#include "player.h"

#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/rendering_device.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/rd_shader_file.hpp>
#include <godot_cpp/classes/rd_shader_spirv.hpp>
#include <godot_cpp/variant/packed_float32_array.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/classes/rd_uniform.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include "status_line.h"

using namespace godot;

void Terrain::_bind_methods() {
    ClassDB::bind_method(D_METHOD("generate_chunk", "p_x", "p_y"), &Terrain::generate_chunk);
}

/*void Terrain::set_mesh_data(Array a) {
    mesh_data = a;
}

Array Terrain::get_mesh_data() {
    return mesh_data;
}*/

Terrain::Terrain() {
    foo_x = 1;
    foo_z = 0;
    UtilityFunctions::print("terrain construct");
    for (int z = 0; z < WORLD_SIZE; z++) {
        for (int x = 0; x < WORLD_SIZE; x++) {
            chunks[z][x] = nullptr;
        }
    }
}

Terrain::~Terrain() {
}

void Terrain::_ready() {
    if (!Engine::get_singleton()->is_editor_hint()) {

    if (!status_line_node) {
           UtilityFunctions::print("terrain: setting status line node");
        status_line_node = get_node<LineEdit>("/root/Node3D/Player/UI/StatusLine");
    }

    generator();
//        generate_chunk(WORLD_CENTER, WORLD_CENTER);

        //for (int y = 3; y >=0; y--) {
        for (int y = 0; y < 4; y++) {
            for (int x = 3; x >=0; x--) {
            //for (int x = 0; x < 4; x++) {
                generate_chunk(WORLD_CENTER + x, WORLD_CENTER + y);
            }
            //UtilityFunctions::print(y);
            printf("%d\n", y);
        }
   /*        RenderingDevice* rd = RenderingServer::get_singleton()->create_local_rendering_device();
        Ref<RDShaderFile> file = ResourceLoader::get_singleton()->load("res://shader.comp.glsl");
        Ref<RDShaderSPIRV> shader_spirv = file->get_spirv();
        RID shader = rd->shader_create_from_spirv(shader_spirv);
        PackedInt32Array input;
        //PackedFloat32Array input;
        for (int z = 0; z < CHUNK_SIZE; z++)
        {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
        for (int x = 0; x < CHUNK_SIZE; x++)
        {
            input.push_back(world_table[WORLD_CENTER][WORLD_CENTER]->table[z][y][x].tile);
        }}
        }
        PackedByteArray bytes = input.to_byte_array();
        
        Vector<uint8_t> bum;
        int b[10];
        rd->storage_buffer_create(bytes.size(), bytes);
        RID buffer = rd->storage_buffer_create(bytes.size(), bytes);
        RDUniform uniform;
        uniform.set_uniform_type(RenderingDevice::UNIFORM_TYPE_STORAGE_BUFFER);
        uniform.set_binding(0);
        uniform.add_id(buffer);
        Array a;
        a.push_back(&uniform);
        RID uniform_set = rd->uniform_set_create(a, shader, 0);

        RID pipeline = rd->compute_pipeline_create(shader);
        int64_t compute_list = rd->compute_list_begin();
        rd->compute_list_bind_compute_pipeline(compute_list, pipeline);
        rd->compute_list_bind_uniform_set(compute_list, uniform_set, 0);
        rd->compute_list_dispatch(compute_list, 5, 1, 1);
        rd->compute_list_end();

        rd->submit();
        rd->sync();

        PackedByteArray output_bytes = rd->buffer_get_data(buffer);
        PackedInt32Array out = output_bytes.to_int32_array();
        //UtilityFunctions::print("Input2: ", input);
        //UtilityFunctions::print("Output2: ", out);*/
    }
}

void Terrain::_process(double delta) {
    if (!Engine::get_singleton()->is_editor_hint()) {
 /*       if (Input::get_singleton()->is_action_just_pressed("ui_accept")) {
        //for (int y = 3; y >=0; y--) {
        for (int y = 0; y < 4; y++) {
            //for (int x = 3; x >=0; x--) {
            for (int x = 0; x < 4; x++) {
                generate_chunk(WORLD_CENTER + x, WORLD_CENTER + y);
            }
            //UtilityFunctions::print(y);
            printf("%d\n", y);
        }*/
        //for (int i = 0; i < 128; i++) {
 /*       generate_chunk(WORLD_CENTER+foo_x, WORLD_CENTER+foo_z);
        foo_x++;
        if (foo_x >= 127) {
            foo_z++;
            foo_x = 0;
        }*/
        //}
       // }
    }
}

void Terrain::generate_chunk(int chunk_x, int chunk_z) {
    //UtilityFunctions::print("generate");

    load_chunk(chunk_x, chunk_z);

    ChunkRenderer *chunk = memnew(ChunkRenderer(chunk_x, chunk_z));
    chunks[chunk_z][chunk_x] = chunk;
    add_child(chunk);
    chunk->set_position(Vector3i((chunk_x - WORLD_CENTER)* CHUNK_SIZE, 0, (chunk_z - WORLD_CENTER) * CHUNK_SIZE));
    if (chunks[chunk_z-1][chunk_x]) {
        chunks[chunk_z-1][chunk_x]->render_self();
        if (chunks[chunk_z][chunk_x-1]) {
            chunks[chunk_z][chunk_x-1]->render_self();
            chunks[chunk_z-1][chunk_x-1]->render_self();
        }
    } else if (chunks[chunk_z][chunk_x-1]) {
        chunks[chunk_z][chunk_x-1]->render_self();
    }
}

void Terrain::mine(Vector3 in_pos, Vector3 in_norm) {
    Vector3 posf = in_pos;
    Vector3 normal = in_norm;
    posf -= normal*0.5;

    Vector3i pos = posf.round();
    pos.x += WORLD_CENTER*CHUNK_SIZE;
    pos.z += WORLD_CENTER*CHUNK_SIZE;
    ChunkRenderer* chunk = chunks[pos.z/CHUNK_SIZE][pos.x/CHUNK_SIZE];
    int chunk_z = pos.z/CHUNK_SIZE;
    int chunk_x = pos.x/CHUNK_SIZE;
    int z_pos = pos.z%CHUNK_SIZE;
    int x_pos = pos.x%CHUNK_SIZE;
    world_table[chunk->chunk_z][chunk->chunk_x]->table[z_pos][pos.y][x_pos].tile = TILE_AIR;
    chunk->render_self();
    if (z_pos == 0 && chunks[chunk_z-1][chunk_x]) {
        chunks[chunk_z-1][chunk_x]->render_self();
    }
    if (x_pos == 0 && chunks[chunk_z][chunk_x-1]) {
        chunks[chunk_z][chunk_x-1]->render_self();
    } 
    if (x_pos == 0 && z_pos == 0 && chunks[chunk_z-1][chunk_x-1]) {
        chunks[chunk_z-1][chunk_x-1]->render_self();
    }

    UtilityFunctions::print(chunk->chunk_x, chunk->chunk_z);
}

void Terrain::place(Vector3 in_pos, Vector3 in_norm) {
    Vector3 posf = in_pos;
    Vector3 normal = in_norm;
    posf += normal*0.5;

    Vector3i pos = posf.round();
    pos.x += WORLD_CENTER*CHUNK_SIZE;
    pos.z += WORLD_CENTER*CHUNK_SIZE;
    int chunk_z = pos.z/CHUNK_SIZE;
    int chunk_x = pos.x/CHUNK_SIZE;
    ChunkRenderer* chunk = chunks[chunk_z][chunk_x];
    int z_pos = pos.z%CHUNK_SIZE;
    int x_pos = pos.x%CHUNK_SIZE;
    world_table[chunk->chunk_z][chunk->chunk_x]->table[z_pos][pos.y][x_pos].tile = TILE_DIRT;
    chunk->render_self();
    if (z_pos == 0 && chunks[chunk_z-1][chunk_x]) {
        chunks[chunk_z-1][chunk_x]->render_self();
    }
    if (x_pos == 0 && chunks[chunk_z][chunk_x-1]) {
        chunks[chunk_z][chunk_x-1]->render_self();
    } 
    if (x_pos == 0 && z_pos == 0 && chunks[chunk_z-1][chunk_x-1]) {
        chunks[chunk_z-1][chunk_x-1]->render_self();
    }

    UtilityFunctions::print("chunk x=", chunk->chunk_x, " chunk z=",chunk->chunk_z);
}

void Terrain::generator()
{
    //OpenSimplex2F(time(NULL), &simplex_context);
    RandomNumberGenerator rng;
    OpenSimplex2F(rng.randi(), &simplex_context);

    load_chunk(WORLD_CENTER, WORLD_CENTER);
}

int Terrain::height_at(int chunk_x, int chunk_y, int x, int y)
{
    return 1 + 
        (OpenSimplex2F_noise2(simplex_context, 
                (chunk_x * CHUNK_SIZE + x) / WORLD_SCALE, 
                (chunk_y * CHUNK_SIZE + y) / WORLD_SCALE) + 1) *
        (CHUNK_SIZE - 2) / 2.0;
}

void Terrain::generate_chunk2(chunk *chunk, int chunk_x, int chunk_y)  
{
    for (int z = 0; z < CHUNK_SIZE; z++) 
    {
        for (int x = 0; x < CHUNK_SIZE; x++) 
        {
            int height = height_at(chunk_x, chunk_y, x, z);
            //printf("%3d ", height);
            for (int y = 0; y < CHUNK_SIZE; y++)
            {
                chunk->table[z][y][x].tile = (y < height-1) ? TILE_DIRT : (y == height-1 ? TILE_GRASS : TILE_AIR);
                chunk->table[z][y][x].weight = 4 + rand() % 9;
            }
        }
        //printf("\n");
    }

    for (int i = 0; i < 128; i++)
    {
        struct object *o = (struct object *)malloc(sizeof(struct object));
        o->type = OBJECT_NULL;
        o->x = rand() % 16;
        o->z = rand() % 16;
        o->y = height_at(chunk_x, chunk_y, o->x, o->z);

        chunk->objects[i] = o;
    }
    for (int i = 0; i < 19; i++)
    {
        struct object *o = (struct object *)malloc(sizeof(struct object));
        o->type = OBJECT_TREE;
        o->x = rand() % 16;
        o->z = rand() % 16;
        o->y = height_at(chunk_x, chunk_y, o->x, o->z);

        chunk->objects[i] = o;
    }
    for (int i = 0; i < 4; i++)
    {
        int b = rand() % BASE_ELEMENTS;
        Element *o = new Element(&base_elements[b]);
        int x = rand() % 16;
        int z = rand() % 16;

        o->set_posittion(x, 0, x);
        o->set_posittion(x, height_at(chunk_x, chunk_y, x, z), z);

        chunk->items[i] = o;
    }

    /*enum biomes random_biome = (enum biomes) (rand() % 4);
    chunk->biome = random_biome;

    switch (random_biome)
    {
        case BIOME_FOREST: create_biome_forest(chunk); break;
        case BIOME_DESERT: create_biome_desert(chunk); break;
        case BIOME_SWEET_TREE: create_biome_sweet_tree(chunk); break;
        case BIOME_LAKE: create_biome_lake(chunk); break;
    }*/
}

char Terrain::load_chunk(int x, int y)
{
    if (x >= 0 && x < WORLD_SIZE && y >= 0 && y < WORLD_SIZE) 
    {
        if (world_table[y][x] == NULL) 
        {
            chunk* c = (chunk*) calloc(1, sizeof(chunk));
//            printf("load %d %d\n", x, y);
            generate_chunk2(c, x, y);
            world_table[y][x] = c;
            notify_load_chunk(x, y);
        }
        return 1;
    }
    return 0;
}

InventoryElement** Terrain::get_item_at(int chunk_x, int chunk_y, int x, int y, int z)
{
    // TODO: change items array to list
   
    for (int i = 0; i < 128; i++)
    {
        InventoryElement *el = world_table[chunk_y][chunk_x]->items[i];
        if (el)
        {
            int el_x, el_y, el_z;
            el->get_posittion(&el_x, &el_y, &el_z);

            if (el_x == x && el_y == y && el_z == z )
            {
                return &world_table[chunk_y][chunk_x]->items[i];
            }
        }
    }
    return NULL;
}

InventoryElement** Terrain::get_item_at_ppos(PlayerCharacter * player)
{
    return get_item_at(player->map_x, player->map_y, player->x, player->y, player->z);
}

void Terrain::set_item_at(InventoryElement *item, int chunk_x, int chunk_y, int x, int y, int z) 
{
    for (int i = 0; i < 128; i++)
    {
        if (!world_table[chunk_y][chunk_x]->items[i])
        {
            world_table[chunk_y][chunk_x]->items[i] = item;
            break;
        }
    }
}

void Terrain::set_item_at_ppos(InventoryElement *item, PlayerCharacter *player) 
{
    set_item_at(item, player->map_x, player->map_y, player->x, player->y, player->z);
}

enum game_tiles Terrain::get_tile_at(int chunk_x, int chunk_y, int x, int y, int z)
{
    return world_table[chunk_y][chunk_x]->table[z][y][x].tile;
}

enum game_tiles Terrain::get_tile_at_ppos(PlayerCharacter *player)
{
    return get_tile_at(player->map_x, player->map_y, player->x, player->y, player->z);
}


