#include "test/shader_table.h"
#include "core/templates/shader_template.h"
#include "core/types/shader.h"

// uniform callbacks
#include "core/core_data.h"
#include <math.h>

// -- set uniform callbacks --

// blinks on red channel
void test_shader_set_uniforms(shader_t* shader, int tex_idx)
{
  core_data_t* core_data = core_data_get();
  const f32 BLINK_SPEED = 3.0f;
  shader_set_vec3(shader, "fade_color", VEC3_XYZ(sin(core_data->total_t * BLINK_SPEED), 0, 1));
}

/*
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
*/

shader_template_t shader_table[SHADER_TEMPLATE_MAX];
const int shader_table_len = SHADER_TEMPLATE_MAX;

void shader_table_init()
{
  // -- custom --
  shader_table[SHADER_TEMPLATE_TEST] = (shader_template_t)
  {
    .name  = "test_shader",
    .vert  = "basic.vert",
    .frag  = "custom/test.frag", 
    .unlit = false,
    .set_uniforms_f = test_shader_set_uniforms,
  };
}

