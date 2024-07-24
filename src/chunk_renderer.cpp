#include "chunk_renderer.h"

#include <cstdlib>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/material.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/static_body3d.hpp>
#include "alchemist/elements.h"
#include "godot_cpp/classes/concave_polygon_shape3d.hpp"

#include "terrain.h"
#include "tiles.h"

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
    StaticBody3D* body = memnew(StaticBody3D);
    add_child(body);
    collision = memnew(CollisionShape3D);
    body->add_child(collision);

    set_mesh(array_mesh);
    terrain = get_node<Terrain>("..");
    //Ref<Material> mat = ResourceLoader::get_singleton()->load("res://models/test_material.tres");
    Ref<Material> mat = ResourceLoader::get_singleton()->load("res://models/new_shader_material.tres");
    if (!mat.is_valid()) {
        UtilityFunctions::print("no material");
    }
    set_material_override(mat);

    render_self();
    spawn_objects();
}

void ChunkRenderer::render_self() {

    Ref<ArrayMesh> array_mesh = get_mesh();

    PackedVector3Array verts;
    PackedVector3Array normals;
    PackedVector2Array uvs;
    PackedColorArray colors;

    for (int z = 0; z < CHUNK_SIZE-1; z++)
    {
        for (int y = 0; y < CHUNK_SIZE-1; y++)
        {
            for (int x = 0; x < CHUNK_SIZE-1; x++)
            {
                CubeCorner corners[8] = {
                    { x, y, z, terrain->world_table[chunk_z][chunk_x]->table[z][y][x].tile },
                    { x+1, y, z, terrain->world_table[chunk_z][chunk_x]->table[z][y][x+1].tile },
                    { x+1, y, z+1, terrain->world_table[chunk_z][chunk_x]->table[z+1][y][x+1].tile },
                    { x, y, z+1, terrain->world_table[chunk_z][chunk_x]->table[z+1][y][x].tile },
                    { x, y+1, z, terrain->world_table[chunk_z][chunk_x]->table[z][y+1][x].tile },
                    { x+1, y+1, z, terrain->world_table[chunk_z][chunk_x]->table[z][y+1][x+1].tile },
                    { x+1, y+1, z+1, terrain->world_table[chunk_z][chunk_x]->table[z+1][y+1][x+1].tile },
                    { x, y+1, z+1, terrain->world_table[chunk_z][chunk_x]->table[z+1][y+1][x].tile }
                };
                int weights[8] = {
                    terrain->world_table[chunk_z][chunk_x]->table[z][y][x].weight,
                    terrain->world_table[chunk_z][chunk_x]->table[z][y][x+1].weight,
                    terrain->world_table[chunk_z][chunk_x]->table[z+1][y][x+1].weight,
                    terrain->world_table[chunk_z][chunk_x]->table[z+1][y][x].weight,
                    terrain->world_table[chunk_z][chunk_x]->table[z][y+1][x].weight,
                    terrain->world_table[chunk_z][chunk_x]->table[z][y+1][x+1].weight,
                    terrain->world_table[chunk_z][chunk_x]->table[z+1][y+1][x+1].weight,
                    terrain->world_table[chunk_z][chunk_x]->table[z+1][y+1][x].weight

                };
                marching_cube(corners, weights, verts, normals, colors);
            }
        }
    }

    int x_offset, chunk_offset;
    if (terrain->world_table[chunk_z][chunk_x+1]) {
        cubes_on_z_edge(CHUNK_SIZE-1, chunk_x, verts, normals, colors);
    }
    if (terrain->world_table[chunk_z+1][chunk_x]) {
        cubes_on_x_edge(CHUNK_SIZE-1, chunk_z, verts, normals, colors);
    }

    if (terrain->world_table[chunk_z+1][chunk_x+1] && terrain->world_table[chunk_z+1][chunk_x] && terrain->world_table[chunk_z][chunk_x+1]) {
        cube_on_corner(chunk_x, chunk_z, 1, 1, verts, normals, colors);
    }
  
    if (!(verts.size() > 0))
    {
        UtilityFunctions::print("ERROR NO VERTS!!!");
        return;
    }

    for (int i = 0; i < verts.size(); i+=3)
    {
        uvs.push_back(Vector2(11.4/32.0, 0.4/16.0));
        uvs.push_back(Vector2(11.6/32.0, 0.4/16.0));
        uvs.push_back(Vector2(11.4/32.0, 0.6/16.0));
      /*  uvs.push_back(Vector2(0, 0));
        uvs.push_back(Vector2(1, 0));
        uvs.push_back(Vector2(0, 1));*/
    }
    Array mesh_data;
    mesh_data.resize(Mesh::ARRAY_MAX);
    mesh_data[Mesh::ARRAY_VERTEX] = verts;
    mesh_data[Mesh::ARRAY_NORMAL] = normals;
    mesh_data[Mesh::ARRAY_TEX_UV] = uvs;
    mesh_data[Mesh::ARRAY_COLOR] = colors;
//    UtilityFunctions::print(verts.size(), "abc", colors.size());
    //UtilityFunctions::print(colors);
    array_mesh->clear_surfaces();
    array_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, mesh_data);
//    create_trimesh_collision();
//    Ref<Shape3D> shape = array_mesh->create_trimesh_shape();
  //  collision->set_shape(shape);
    collision->set_shape(array_mesh->create_trimesh_shape());
}

void ChunkRenderer::cubes_on_z_edge(int x_offset, int chunk_offset, PackedVector3Array &verts, PackedVector3Array &normals, PackedColorArray& colors) {
    for (int z = 0; z < CHUNK_SIZE-1; z++)
    {
        for (int y = 0; y < CHUNK_SIZE-1; y++)
        {
             CubeCorner corners[8] = {
                { x_offset, y, z, terrain->world_table[chunk_z][chunk_offset]->table[z][y][CHUNK_SIZE-1].tile },
                { x_offset+1, y, z, terrain->world_table[chunk_z][chunk_offset+1]->table[z][y][0].tile },
                { x_offset+1, y, z+1, terrain->world_table[chunk_z][chunk_offset+1]->table[z+1][y][0].tile },
                { x_offset, y, z+1, terrain->world_table[chunk_z][chunk_offset]->table[z+1][y][CHUNK_SIZE-1].tile },
                { x_offset, y+1, z, terrain->world_table[chunk_z][chunk_offset]->table[z][y+1][CHUNK_SIZE-1].tile },
                { x_offset+1, y+1, z, terrain->world_table[chunk_z][chunk_offset+1]->table[z][y+1][0].tile },
                { x_offset+1, y+1, z+1, terrain->world_table[chunk_z][chunk_offset+1]->table[z+1][y+1][0].tile },
                { x_offset, y+1, z+1, terrain->world_table[chunk_z][chunk_offset]->table[z+1][y+1][CHUNK_SIZE-1].tile }
            };
             int weights[8] = {
               terrain->world_table[chunk_z][chunk_offset]->table[z][y][CHUNK_SIZE-1].weight,
                terrain->world_table[chunk_z][chunk_offset+1]->table[z][y][0].weight,
                terrain->world_table[chunk_z][chunk_offset+1]->table[z+1][y][0].weight,
                terrain->world_table[chunk_z][chunk_offset]->table[z+1][y][CHUNK_SIZE-1].weight,
                terrain->world_table[chunk_z][chunk_offset]->table[z][y+1][CHUNK_SIZE-1].weight,
                terrain->world_table[chunk_z][chunk_offset+1]->table[z][y+1][0].weight,
                terrain->world_table[chunk_z][chunk_offset+1]->table[z+1][y+1][0].weight,
                terrain->world_table[chunk_z][chunk_offset]->table[z+1][y+1][CHUNK_SIZE-1].weight
             };
            marching_cube(corners, weights, verts, normals, colors);
        }
    }
}

void ChunkRenderer::cubes_on_x_edge(int z_offset, int chunk_offset, PackedVector3Array &verts, PackedVector3Array &normals, PackedColorArray& colors) {
    for (int x = 0; x < CHUNK_SIZE-1; x++)
    {
        for (int y = 0; y < CHUNK_SIZE-1; y++)
        {
            CubeCorner corners[8] = {
                { x, y, z_offset, terrain->world_table[chunk_offset][chunk_x]->table[CHUNK_SIZE-1][y][x].tile },
                { x+1, y, z_offset, terrain->world_table[chunk_offset][chunk_x]->table[CHUNK_SIZE-1][y][x+1].tile },
                { x+1, y, z_offset+1, terrain->world_table[chunk_offset+1][chunk_x]->table[0][y][x+1].tile },
                { x, y, z_offset+1, terrain->world_table[chunk_offset+1][chunk_x]->table[0][y][x].tile },
                { x, y+1, z_offset, terrain->world_table[chunk_offset][chunk_x]->table[CHUNK_SIZE-1][y+1][x].tile },
                { x+1, y+1, z_offset, terrain->world_table[chunk_offset][chunk_x]->table[CHUNK_SIZE-1][y+1][x+1].tile },
                { x+1, y+1, z_offset+1, terrain->world_table[chunk_offset+1][chunk_x]->table[0][y+1][x+1].tile },
                { x, y+1, z_offset+1, terrain->world_table[chunk_offset+1][chunk_x]->table[0][y+1][x].tile }
            };
            int weights[8] = {
                terrain->world_table[chunk_offset][chunk_x]->table[CHUNK_SIZE-1][y][x].weight,
                terrain->world_table[chunk_offset][chunk_x]->table[CHUNK_SIZE-1][y][x+1].weight,
                terrain->world_table[chunk_offset+1][chunk_x]->table[0][y][x+1].weight,
                terrain->world_table[chunk_offset+1][chunk_x]->table[0][y][x].weight,
                terrain->world_table[chunk_offset][chunk_x]->table[CHUNK_SIZE-1][y+1][x].weight,
                terrain->world_table[chunk_offset][chunk_x]->table[CHUNK_SIZE-1][y+1][x+1].weight,
                terrain->world_table[chunk_offset+1][chunk_x]->table[0][y+1][x+1].weight,
                terrain->world_table[chunk_offset+1][chunk_x]->table[0][y+1][x].weight
            };
            marching_cube(corners, weights, verts, normals, colors);
        }
    }
}

void ChunkRenderer::cube_on_corner(int x, int z, int dx, int dz, PackedVector3Array &verts, PackedVector3Array &normals, PackedColorArray& colors) {
    volatile int foo =x;
    volatile int boo = z;
    int pos_x = (dx == -1) ? -1 : CHUNK_SIZE-1;
    int pos_z = (dz == -1) ? -1 : CHUNK_SIZE-1;
    int tile_z;
    int tile_x;
    for (int y = 0; y < CHUNK_SIZE-1; y++)
    {
      CubeCorner corners[8] = {
            { pos_x, y, pos_z, terrain->world_table[z][x]->table[CHUNK_SIZE-1][y][CHUNK_SIZE-1].tile },
            { pos_x+1, y, pos_z, terrain->world_table[z][x+1]->table[CHUNK_SIZE-1][y][0].tile },
            { pos_x+1, y, pos_z+1, terrain->world_table[z + 1][x + 1]->table[0][y][0].tile },
            { pos_x, y, pos_z+1, terrain->world_table[z+1][x]->table[0][y][CHUNK_SIZE-1].tile },
            { pos_x, y+1, pos_z, terrain->world_table[z][x]->table[CHUNK_SIZE-1][y+1][CHUNK_SIZE-1].tile },
            { pos_x+1, y+1, pos_z, terrain->world_table[z][x+1]->table[CHUNK_SIZE-1][y+1][0].tile },
            { pos_x+1, y+1, pos_z+1, terrain->world_table[z + 1][x+1]->table[0][y+1][0].tile },
            { pos_x, y+1, pos_z+1, terrain->world_table[z+1][x]->table[0][y+1][CHUNK_SIZE-1].tile }
        };
      int weights[8] = {
          terrain->world_table[z][x]->table[CHUNK_SIZE-1][y][CHUNK_SIZE-1].weight,
            terrain->world_table[z][x+1]->table[CHUNK_SIZE-1][y][0].weight,
            terrain->world_table[z + 1][x + 1]->table[0][y][0].weight,
            terrain->world_table[z+1][x]->table[0][y][CHUNK_SIZE-1].weight,
            terrain->world_table[z][x]->table[CHUNK_SIZE-1][y+1][CHUNK_SIZE-1].weight,
            terrain->world_table[z][x+1]->table[CHUNK_SIZE-1][y+1][0].weight,
            terrain->world_table[z + 1][x+1]->table[0][y+1][0].weight,
            terrain->world_table[z+1][x]->table[0][y+1][CHUNK_SIZE-1].weight
      };
        marching_cube(corners, weights, verts, normals, colors);
    }

}

Vector3 interpolate_verts(Vector3 v1, Vector3 v2, float w1, float w2) {
    return v1 + (w2 / (w1+w2)) * (v2-v1);
    //return (v1+v2) * 0.5;
}
Vector3 inter(Vector3 v1, Vector3 v2) {
    return (v1+v2) * 0.5;
}

void ChunkRenderer::marching_cube(CubeCorner (&corners)[8], int (&weights)[8], PackedVector3Array &verts, PackedVector3Array &normals, PackedColorArray &colors) {
    int cubeIndex = 0;
    for (int i = 0; i < 8; i++) {
        //corners[i][0] += (chunk_x - WORLD_CENTER) * CHUNK_SIZE;
        //corners[i][1] *= 10;
        //corners[i][2] += (chunk_z - WORLD_CENTER) * CHUNK_SIZE;
        if (corners[i].tile == nullptr) cubeIndex |= (1 << i);
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

        Vector3 a = interpolate_verts(Vector3(corners[a0].x, corners[a0].y, corners[a0].z), Vector3(corners[b0].x, corners[b0].y, corners[b0].z), weights[a0], weights[b0]);
        Vector3 b = interpolate_verts(Vector3(corners[a1].x, corners[a1].y, corners[a1].z), Vector3(corners[b1].x, corners[b1].y, corners[b1].z), weights[a1], weights[b1]);
        Vector3 c = interpolate_verts(Vector3(corners[a2].x, corners[a2].y, corners[a2].z), Vector3(corners[b2].x, corners[b2].y, corners[b2].z), weights[a2], weights[b2]);

        Vector3 ab = b - a;
        Vector3 ac = c - a;
        Vector3 norm = -(ab.cross(ac).normalized());

        // find nearest non air point
        Vector3 center = (a+b+c) / 3.0;
        int color_corner = -1;
        float dist = -1;
        for (int j = 0; j < 8; j++) {
            if (color_corner == -1) {
                if (corners[j].tile) {
                    color_corner = j;
                    dist = center.distance_squared_to(Vector3(corners[j].x, corners[j].y, corners[j].z));
                }
            } else if (corners[j].tile && center.distance_squared_to(Vector3(corners[j].x, corners[j].y, corners[j].z)) < dist) {
                color_corner = j;
                dist = center.distance_squared_to(Vector3(corners[j].x, corners[j].y, corners[j].z));
            }
        }
        if (color_corner == -1)
            abort();

        verts.push_back(a);
        verts.push_back(b);
        verts.push_back(c);
        normals.push_back(norm);
        normals.push_back(norm);
        normals.push_back(norm);
        //colors.push_back(Color(corners[common][3]*20, 0, 0));
        //colors.push_back(Color(corners[common][3]*20, 0, 0));
        //colors.push_back(Color(corners[common][3]*20, 0, 0));
        volatile int x, y, z, cx, cz;
        if (corners[color_corner].x < CHUNK_SIZE) {
            x = corners[color_corner].x;
            cx = chunk_x;
        } else {
            x = 0;
            cx =  chunk_x+1;
        }
        if (corners[color_corner].y < CHUNK_SIZE) {
            y = corners[color_corner].y;
        } else {
            y = 0;
        }
        if (corners[color_corner].z < CHUNK_SIZE) {
            z = corners[color_corner].z;
            cz = chunk_z;
        } else {
            z = 0;
            cz =  chunk_z+1;
        }
        BaseElement* el = terrain->world_table[cz][cx]->table[z][y][x].tile;

        //printf("id: %d %d %d %d %d\n", id, color_corner, corners[color_corner].z, corners[color_corner].y, corners[color_corner].x);
        Color color = el->color;
        colors.push_back(color);
        colors.push_back(color);
        colors.push_back(color);
        //colors.push_back(Color(corners[color_corner][3]/9.0, 0, 0));
    }

}

void ChunkRenderer::spawn_objects() {
    // TODO: replace table with lists
    for (int i = 0; i < 10; i++) {
        struct object * o = terrain->world_table[chunk_z][chunk_x]->objects[i];
        if (o)
        switch (o->type) {
            case OBJECT_TREE:
            {
              /*  Ref<PackedScene> packed_tree = ResourceLoader::get_singleton()->load("res://tree.tscn");
                Node3D* tree = (Node3D*)packed_tree->instantiate();
                add_child(tree);
                tree->set_position(Vector3(o->x, o->y, o->z));
                break;*/
            }
            default:
                break;
        }
    }
}
