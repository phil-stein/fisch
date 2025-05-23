#include "core/templates/shader_template.h"
#include "core/types/shader.h"

// basic
const shader_template_t shader_basic =
  {
    .name  = "basic_shader",
    .vert  = "basic.vert",
    .frag  = "basic.frag",
    .unlit = false,
    .set_uniforms_f = NULL,
  };
// shadow map
const shader_template_t shader_shadow_map =
  {
    .name  = "shadow_map_shader",
    .vert  = "shadow_map.vert",
    .frag  = "empty.frag",
    .unlit = false,
    .set_uniforms_f = NULL,
  };
// deferred
const shader_template_t shader_deferred =
  {
    .name  = "deferred_shader",
    .vert  = "basic.vert",
    .frag  = "deferred.frag",
    .unlit = false,
    .set_uniforms_f = NULL,
  };
const shader_template_t shader_deferred_unlit =
  {
    .name  = "deferred_unlit",
    .vert  = "basic.vert",
    .frag  = "deferred_unlit.frag",
    .unlit = false,
    .set_uniforms_f = NULL,
  };
// skybox
const shader_template_t shader_skybox =
  {
    .name  = "skybox_shader",
    .vert  = "cubemap/cube_map.vert",
    .frag  = "cubemap/cube_map.frag",
    .unlit = false,
    .set_uniforms_f = NULL,
  };
// shadow pass
const shader_template_t shader_shadow_pass =
  {
    .name  = "shadow_pass_shader",
    .vert  = "screen.vert",
    .frag  = "shadow_pass.frag",
    .unlit = false,
    .set_uniforms_f = NULL,
  };
// lighting
const shader_template_t shader_lighting =
  {
    .name  = "lighting_shader",
    .vert  = "screen.vert",
    .frag  = "pbr.frag",
    .unlit = false,
    .set_uniforms_f = NULL,
  };
// post-fx
const shader_template_t shader_post_fx =
  {
    .name  = "post_fx_shader",
    .vert  = "screen.vert",
    .frag  = "post_fx.frag",
    .unlit = false,
    .set_uniforms_f = NULL,
  };
// brdf lut
const shader_template_t shader_brdf_lut =
  {
    .name  = "brdf_lut_shader",
    .vert  = "cubemap/brdf_lut.vert",
    .frag  = "cubemap/brdf_lut.frag",
    .unlit = false,
    .set_uniforms_f = NULL,
  };
// mouse pick
const shader_template_t shader_mouse_pick =
  {
    .name  = "mouse_pick_shader",
    .vert  = "basic.vert",
    .frag  = "mouse_picking.frag",
    .unlit = false,
    .set_uniforms_f = NULL,
  };
// terrain
const shader_template_t shader_terrain =
  {
    .name  = "terrain_shader",
    .vert  = "terrain.vert",
    .frag  = "terrain.frag",
    .unlit = false,
    .set_uniforms_f = NULL,
  };
// fxaa 
const shader_template_t shader_fxaa =
  {
    .name  = "fxaa_shader",
    .vert  = "post_fx/fxFXAA.vert",
    .frag  = "post_fx/fxFXAA.frag",
    .unlit = false,
    .set_uniforms_f = NULL,
  };
const shader_template_t shader_pbr_forward =
  {
    .name  = "pbr",
    .vert  = "basic.vert",
    .frag  = "pbr_forward_02.frag",
    .unlit = false,
    .set_uniforms_f = NULL,
  };



const shader_template_t* shader_template_get(int idx)
{
  TRACE();

  ERR_CHECK(idx < shader_table_len,    "shader template idx too high: %d\n", idx);
  // ERR_CHECK(idx != 0,                  "shader template idx cant be 0, idx: %d\n", idx);
  ERR_CHECK(idx != -1,                 "shader template idx cant be -1 or SHADER_TEMPLATE_NONE, idx: %d\n", idx);
  ERR_CHECK(idx >= SHADER_TEMPLATE_MIN, "shader template idx too low, idx: %d, min: %d\n", idx, SHADER_TEMPLATE_MIN);

  // handle built-in shaders
  switch (idx)
  {
    case SHADER_TEMPLATE_BASIC:
      return &shader_basic;
    case SHADER_TEMPLATE_SHADOW_MAP:
      return &shader_shadow_map;
    case SHADER_TEMPLATE_DEFERRED:
      return &shader_deferred;
    case SHADER_TEMPLATE_DEFERRED_UNLIT:
      return &shader_deferred_unlit;
    case SHADER_TEMPLATE_SKYBOX:
      return &shader_skybox;
    case SHADER_TEMPLATE_SHADOW_PASS:
      return &shader_shadow_pass;
    case SHADER_TEMPLATE_LIGHTING:
      return &shader_lighting;
    case SHADER_TEMPLATE_POST_FX:
      return &shader_post_fx;
    case SHADER_TEMPLATE_BRDF_LUT:
      return &shader_brdf_lut;
    case SHADER_TEMPLATE_MOUSE_PICK:
      return &shader_mouse_pick;
    case SHADER_TEMPLATE_TERRAIN:
      return &shader_terrain;
    case SHADER_TEMPLATE_FXAA:
      return &shader_fxaa;
    case SHADER_TEMPLATE_PBR_FORWARD:
      return &shader_pbr_forward;
  }

  return &shader_table[idx]; 
}

const shader_template_t* shader_template_get_all(int* len)
{
  TRACE();

  *len = shader_table_len;
  return shader_table;
}



