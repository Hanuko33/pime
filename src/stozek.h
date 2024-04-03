#ifndef STOZEK_H
#define STOZEK_H

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/variant/color.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>

namespace godot {
class Stozek : public MeshInstance3D
{

public:       
    Vector3 top_pos, mid_pos;
    int level;
    Stozek(real_t dx, real_t dy, real_t dz, real_t rot_x, real_t rot_y, real_t rot_z, Color kolor, int l=1, float scale=1.0);
    void rotate(float x, float y, float z);
};
}
#endif // STOZEK_H
