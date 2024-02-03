#include "chunk_renderer.h"

#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/material.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

extern "C" {
#include "../game_gui/world.h"
}

#pragma optimize false
using namespace godot;

void ChunkRenderer::_bind_methods() {

}

ChunkRenderer::ChunkRenderer() {
    chunk_x = -1;
    chunk_z = -1;
}

ChunkRenderer::ChunkRenderer(int p_chunk_x, int p_chunk_z) {
    chunk_x = p_chunk_x;
    chunk_z = p_chunk_z;
}

ChunkRenderer::~ChunkRenderer() {
}

void ChunkRenderer::_ready() {
    Ref<ArrayMesh> array_mesh = memnew(ArrayMesh);
    set_mesh(array_mesh);
    //Ref<Material> mat = ResourceLoader::get_singleton()->load("res://models/new_material.tres");
    Ref<Material> mat = ResourceLoader::get_singleton()->load("res://models/test_material.tres");
    if (!mat.is_valid()) {
        UtilityFunctions::print("gowno");
    }
    set_material_overlay(mat);

    render();
}

void ChunkRenderer::render() {

    Ref<ArrayMesh> array_mesh = get_mesh();

    PackedVector3Array verts;
    PackedVector3Array normals;
    PackedVector2Array uvs;

    for (int z = 0; z < CHUNK_SIZE-1; z++)
    {
        for (int y = 0; y < CHUNK_SIZE-1; y++)
        {
            for (int x = 0; x < CHUNK_SIZE-1; x++)
            {
                int corners[8][4] = {
                    { x, y, z, world_table[chunk_z][chunk_x]->table[z][y][x].tile },
                    { x+1, y, z, world_table[chunk_z][chunk_x]->table[z][y][x+1].tile },
                    { x+1, y, z+1, world_table[chunk_z][chunk_x]->table[z+1][y][x+1].tile },
                    { x, y, z+1, world_table[chunk_z][chunk_x]->table[z+1][y][x].tile },
                    { x, y+1, z, world_table[chunk_z][chunk_x]->table[z][y+1][x].tile },
                    { x+1, y+1, z, world_table[chunk_z][chunk_x]->table[z][y+1][x+1].tile },
                    { x+1, y+1, z+1, world_table[chunk_z][chunk_x]->table[z+1][y+1][x+1].tile },
                    { x, y+1, z+1, world_table[chunk_z][chunk_x]->table[z+1][y+1][x].tile }
                };
                marching_cube(corners, verts, normals);
            }
        }
    }

    int x_offset, chunk_offset;
    if (world_table[chunk_z][chunk_x+1]) {
        cubes_on_z_edge(CHUNK_SIZE-1, chunk_x, verts, normals);
    }
    if (world_table[chunk_z][chunk_x-1]) {
        cubes_on_z_edge(-1, chunk_x-1, verts, normals);
    }
    if (world_table[chunk_z+1][chunk_x]) {
        cubes_on_x_edge(CHUNK_SIZE-1, chunk_z, verts, normals);
    }
    if (world_table[chunk_z-1][chunk_x]) {
        cubes_on_x_edge(-1, chunk_z-1, verts, normals);
    }

    if (world_table[chunk_z+1][chunk_x+1] && world_table[chunk_z+1][chunk_x] && world_table[chunk_z][chunk_x+1]) {
        cube_on_corner(chunk_x, chunk_z, 1, 1, verts, normals);
    }
    if (world_table[chunk_z-1][chunk_x-1] && world_table[chunk_z-1][chunk_x] && world_table[chunk_z][chunk_x-1]) {
        cube_on_corner(chunk_x-1, chunk_z-1, -1, -1, verts, normals);
    }
    if (world_table[chunk_z-1][chunk_x+1] && world_table[chunk_z-1][chunk_x] && world_table[chunk_z][chunk_x+1]) {
        cube_on_corner(chunk_x, chunk_z-1, 1, -1, verts, normals);
    }
    if (world_table[chunk_z+1][chunk_x-1] && world_table[chunk_z+1][chunk_x] && world_table[chunk_z][chunk_x-1]) {
        cube_on_corner(chunk_x-1, chunk_z, -1, 1, verts, normals);
    }
   
    if (!(verts.size() > 0))
    {
        UtilityFunctions::print("ERROR NO VERTS!!!");
        return;
    }

    PackedColorArray colors;

    for (int i = 0; i < verts.size(); i+=3)
    {
      /*  uvs.push_back(Vector2(11.4/32.0, 0.4/16.0));
        uvs.push_back(Vector2(11.6/32.0, 0.4/16.0));
        uvs.push_back(Vector2(11.4/32.0, 0.6/16.0));*/
        uvs.push_back(Vector2(0, 0));
        uvs.push_back(Vector2(1, 0));
        uvs.push_back(Vector2(0, 1));
    }
    Array mesh_data;
    mesh_data.resize(Mesh::ARRAY_MAX);
    mesh_data[Mesh::ARRAY_VERTEX] = verts;
    mesh_data[Mesh::ARRAY_NORMAL] = normals;
    mesh_data[Mesh::ARRAY_TEX_UV] = uvs;
    //Ref<ArrayMesh> array_mesh = get_mesh();
    //array_mesh->clear_surfaces();
    array_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, mesh_data);
    //UtilityFunctions::print("verts", verts);
    //UtilityFunctions::print("data", mesh_data);
    //UtilityFunctions::print("verts", verts);
    //UtilityFunctions::print("normals", normals);
    //array_mesh->lightmap_unwrap(get_global_transform(), 0.4);
//    create_convex_collision();
    Ref<MeshConvexDecompositionSettings> settings = memnew(MeshConvexDecompositionSettings);
    settings->set_max_num_vertices_per_convex_hull(1024);
    settings->set_convex_hull_downsampling(16);
    settings->set_max_convex_hulls(CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE);
    settings->set_resolution(CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE);
    //create_multiple_convex_collisions(settings);
    create_trimesh_collision();

}

void ChunkRenderer::cubes_on_z_edge(int x_offset, int chunk_offset, PackedVector3Array &verts, PackedVector3Array &normals) {
    for (int z = 0; z < CHUNK_SIZE-1; z++)
    {
        for (int y = 0; y < CHUNK_SIZE-1; y++)
        {
             int corners[8][4] = {
                { x_offset, y, z, world_table[chunk_z][chunk_offset]->table[z][y][CHUNK_SIZE-1].tile },
                { x_offset+1, y, z, world_table[chunk_z][chunk_offset+1]->table[z][y][0].tile },
                { x_offset+1, y, z+1, world_table[chunk_z][chunk_offset+1]->table[z+1][y][0].tile },
                { x_offset, y, z+1, world_table[chunk_z][chunk_offset]->table[z+1][y][CHUNK_SIZE-1].tile },
                { x_offset, y+1, z, world_table[chunk_z][chunk_offset]->table[z][y+1][CHUNK_SIZE-1].tile },
                { x_offset+1, y+1, z, world_table[chunk_z][chunk_offset+1]->table[z][y+1][0].tile },
                { x_offset+1, y+1, z+1, world_table[chunk_z][chunk_offset+1]->table[z+1][y+1][0].tile },
                { x_offset, y+1, z+1, world_table[chunk_z][chunk_offset]->table[z+1][y+1][CHUNK_SIZE-1].tile }
            };
            marching_cube(corners, verts, normals);
        }
    }
}

void ChunkRenderer::cubes_on_x_edge(int z_offset, int chunk_offset, PackedVector3Array &verts, PackedVector3Array &normals) {
    for (int x = 0; x < CHUNK_SIZE-1; x++)
    {
        for (int y = 0; y < CHUNK_SIZE-1; y++)
        {
          int corners[8][4] = {
                { x, y, z_offset, world_table[chunk_offset][chunk_x]->table[CHUNK_SIZE-1][y][x].tile },
                { x+1, y, z_offset, world_table[chunk_offset][chunk_x]->table[CHUNK_SIZE-1][y][x+1].tile },
                { x+1, y, z_offset+1, world_table[chunk_offset+1][chunk_x]->table[0][y][x+1].tile },
                { x, y, z_offset+1, world_table[chunk_offset+1][chunk_x]->table[0][y][x].tile },
                { x, y+1, z_offset, world_table[chunk_offset][chunk_x]->table[CHUNK_SIZE-1][y+1][x].tile },
                { x+1, y+1, z_offset, world_table[chunk_offset][chunk_x]->table[CHUNK_SIZE-1][y+1][x+1].tile },
                { x+1, y+1, z_offset+1, world_table[chunk_offset+1][chunk_x]->table[0][y+1][x+1].tile },
                { x, y+1, z_offset+1, world_table[chunk_offset+1][chunk_x]->table[0][y+1][x].tile }
            };
            marching_cube(corners, verts, normals);
        }
    }
}

void ChunkRenderer::cube_on_corner(int x, int z, int dx, int dz, PackedVector3Array &verts, PackedVector3Array &normals) {
    volatile int foo =x;
    volatile int boo = z;
    int pos_x = (dx == -1) ? -1 : CHUNK_SIZE-1;
    int pos_z = (dz == -1) ? -1 : CHUNK_SIZE-1;
    int tile_z;
    int tile_x;
    for (int y = 0; y < CHUNK_SIZE-1; y++)
    {
      int corners[8][4] = {
            { pos_x, y, pos_z, world_table[z][x]->table[CHUNK_SIZE-1][y][CHUNK_SIZE-1].tile },
            { pos_x+1, y, pos_z, world_table[z][x+1]->table[CHUNK_SIZE-1][y][0].tile },
            { pos_x+1, y, pos_z+1, world_table[z + 1][x + 1]->table[0][y][0].tile },
            { pos_x, y, pos_z+1, world_table[z+1][x]->table[0][y][CHUNK_SIZE-1].tile },
            { pos_x, y+1, pos_z, world_table[z][x]->table[CHUNK_SIZE-1][y+1][CHUNK_SIZE-1].tile },
            { pos_x+1, y+1, pos_z, world_table[z][x+1]->table[CHUNK_SIZE-1][y+1][0].tile },
            { pos_x+1, y+1, pos_z+1, world_table[z + 1][x+1]->table[0][y+1][0].tile },
            { pos_x, y+1, pos_z+1, world_table[z+1][x]->table[0][y+1][CHUNK_SIZE-1].tile }
        };
        marching_cube(corners, verts, normals);
    }

}

void ChunkRenderer::marching_cube(int (&corners)[8][4], PackedVector3Array &verts, PackedVector3Array &normals) {
    int cubeIndex = 0;
    for (int i = 0; i < 8; i++) {
        corners[i][0] += (chunk_x - WORLD_CENTER) * CHUNK_SIZE;
        //corners[i][1] *= 10;
        corners[i][2] += (chunk_z - WORLD_CENTER) * CHUNK_SIZE;
        if (corners[i][3] == 0) cubeIndex |= (1 << i);
    }
    int numIndices = lengths[cubeIndex];
    int offset = offsets[cubeIndex];

    for (int i = 0; i < numIndices; i += 3) {
        int v0 = lut[offset+i];
        int v1 = lut[offset+1+i];
        int v2 = lut[offset+2+i];

        int a0 = cornerIndexAFromEdge[v0];
        int b0 = cornerIndexBFromEdge[v0];

        int a1 = cornerIndexAFromEdge[v1];
        int b1 = cornerIndexBFromEdge[v1];

        int a2 = cornerIndexAFromEdge[v2];
        int b2 = cornerIndexBFromEdge[v2];

        Vector3 a = (Vector3(corners[a0][0], corners[a0][1], corners[a0][2]) + Vector3(corners[b0][0], corners[b0][1], corners[b0][2])) * 0.5;
        Vector3 b = (Vector3(corners[a1][0], corners[a1][1], corners[a1][2]) + Vector3(corners[b1][0], corners[b1][1], corners[b1][2])) * 0.5;
        Vector3 c = (Vector3(corners[a2][0], corners[a2][1], corners[a2][2]) + Vector3(corners[b2][0], corners[b2][1], corners[b2][2])) * 0.5;

        Vector3 ab = b - a;
        Vector3 ac = c - a;
        Vector3 norm = -(ab.cross(ac).normalized());

        verts.push_back(a);
        verts.push_back(b);
        verts.push_back(c);
        normals.push_back(norm);
        normals.push_back(norm);
        normals.push_back(norm);
    }

}
