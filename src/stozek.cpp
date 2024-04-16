#include "stozek.h"
#include <godot_cpp/classes/standard_material3d.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/cylinder_mesh.hpp>
#include <godot_cpp/classes/surface_tool.hpp>

using namespace godot;

Stozek::Stozek(real_t dx, real_t dy, real_t dz, real_t rot_x, real_t rot_y, real_t rot_z, Color kolor, int l, float scale)
{   
    level=l;
#if 0
    ArrayMesh * mesh = memnew(ArrayMesh);
    SurfaceTool *st = memnew(SurfaceTool);
    st->begin(Mesh::PRIMITIVE_LINES);
    float s=scale/2;

      Vector3 vertices[] = {
          Vector3(-s, -s, -s), Vector3(s, -s, -s),
          Vector3(s, s, -s),   Vector3(-s, s, -s),
          Vector3(-s, -s, s),  Vector3(s, -s, s),
          Vector3(s, s, s),  Vector3(-s, s, s)
      };

      int edges[][2] = {
          {0, 1},  {1, 2}, {2, 3}, {3, 0},
          {4, 5},  {5, 6}, {6, 7}, {7, 4},
          {0, 4},  {1, 5}, {2, 6}, {3, 7}
      };

      for (int i = 0; i < 12; ++i) {
          st->add_vertex(vertices[edges[i][0]]);
          st->add_vertex(vertices[edges[i][1]]);
      }

      st->index();
      mesh->add_surface_from_arrays(Mesh::PRIMITIVE_LINES, st->commit_to_arrays());

      set_mesh(mesh);
#endif
    MeshInstance3D * stozek = memnew(MeshInstance3D);
    add_child(stozek);

    CylinderMesh* cylinder_mesh = memnew(CylinderMesh);

    cylinder_mesh->set_top_radius(0);
    cylinder_mesh->set_bottom_radius(0.01*scale);
    cylinder_mesh->set_height(scale);

  //  UtilityFunctions::print("stozek @ dx=", dx, " dy=", dy, " dz=", dz, " rot_x=", rot_x, " rot_y=", rot_y, " rot_z=", rot_z);
    stozek->set_mesh(cylinder_mesh);
    set_position(get_position() + Vector3(dx, dy, dz));
    stozek->set_position(stozek->get_position() + Vector3(0, cylinder_mesh->get_height()/2, 0));

   // set_rotation_order(EULER_ORDER_XYZ);
    set_rotation_degrees(get_rotation_degrees() + Vector3(rot_x, rot_y, rot_z));
    //set_scale(get_scale() * scale);

    Vector3 pos0=get_position();// + stozek->get_position();

    double height=cylinder_mesh->get_height();
    Basis base = get_basis();
    Vector3 Y_axis = base.get_column(1);
  //  UtilityFunctions::print("base=", base, " base[0]=", base[0], " base[1]=", base[1], " base[2]=", base[2], "Y axis=", Y_axis);

    top_pos=pos0 + (Y_axis * height);
  //  UtilityFunctions::print("   top_pos:", top_pos, " = pos0:", pos0, " + Y_axis:", Y_axis, " * height:", height);

    mid_pos=pos0 + (Y_axis * height/2);
  //  UtilityFunctions::print("   mid_pos:", mid_pos, " = pos0:", pos0, " + Y_axis:", Y_axis, " * height/2:", height/2);

    Ref<StandardMaterial3D> green_material = memnew(StandardMaterial3D);
    green_material->set_albedo(kolor);
    stozek->set_material_override(green_material);

    /*{
       Basis base = get_basis();
       Vector3 B = base[1];
       Vector3 A = {1,0,0};
       A.normalize();
       B.normalize();
       Vector3 rotation_axis = A.cross(B).normalized();
       real_t rotation_angle = -1*acos(A.dot(B))  * 180.0 / Math_PI;
       UtilityFunctions::print("Rotation axis: " + String(rotation_axis * rotation_angle));
       set_rotation_degrees(get_rotation_degrees() + rotation_axis * rotation_angle);
   }*/
    /*
    var cylinder = mesh as CylinderMesh
        var height = cylinder.height
        print (position)
        print(position + basis.y * height/2)*/
}

void Stozek::rotate(float x, float y, float z)
{
      set_rotation_degrees(get_rotation_degrees() + Vector3(x, y, z));
}
