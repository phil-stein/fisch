#include "core/templates/shader_template.h"
#include "core/types/shader.h"

// -- set uniform callbacks --

void test_shader_set_uniforms(shader_t* shader, int tex_idx)
{
  shader_set_vec3(shader, "fade_color", VEC3_XYZ(1, 0, 1));
}


const shader_template_t shader_table[] = 
{
  // -- custom --
  {
    .name  = "test_shader",
    .vert  = "basic.vert",
    .frag  = "custom/test.frag", 
    .unlit = false,
    .set_uniforms_f = test_shader_set_uniforms,
  },
};
const int shader_table_len = sizeof(shader_table) / sizeof(shader_table[0]);


