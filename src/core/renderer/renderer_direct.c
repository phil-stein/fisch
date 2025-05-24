#include "core/renderer/renderer_direct.h"
#include "core/renderer/renderer.h"
#include "core/core_data.h"
#include "core/io/assetm.h"
#include "core/types/framebuffer.h"
#include "core/window.h"
#include "core/camera.h"
#include "core/debug/debug_opengl.h"
#include "core/state/state.h"
#include "global/global_types.h"
#include "math/math_mat4.h"
#include "math/math_vec2.h"
#include "math/math_vec3.h"
#include "puzzle_game/material_table.h"


void renderer_direct_init()
{
  TRACE();
}

// @TODO: doesnt work, used in mui.c
void renderer_direct_draw_quad(vec2 cam_pos, f32 cam_zoom, vec2 pos, vec2 size, vec3 color)
{
  TRACE();

  renderer_direct_draw_quad_textured(cam_pos, cam_zoom, pos, size, assetm_get_texture("#internal/blank.png", true), color);
}

void renderer_direct_draw_quad_textured_handle(vec2 cam_pos, f32 cam_zoom, vec2 pos, vec2 size, u32 handle, rgbf tint)
{
  TRACE();


  // ---- mvp ----

  mat4 model;
  mat4_make_model_2d(pos, size, 0.0f, model);

  mat4 view;
  mat4_lookat_2d(cam_pos, cam_zoom, view);

  int w, h;
  window_get_size(&w, &h);
  mat4 proj;
  camera_get_proj_mat(w, h, proj);

  // ---- shader & draw call -----	

  // glViewport( 0, 0, w ,h );

  shader_use(&core_data->basic_shader);
  shader_set_vec3(&core_data->basic_shader, "tint", tint);
  _glActiveTexture(GL_TEXTURE0);
  _glBindTexture(GL_TEXTURE_2D, handle); 
  shader_set_int(&core_data->basic_shader, "tex", 0);

  shader_set_mat4(&core_data->basic_shader, "model", model);
  shader_set_mat4(&core_data->basic_shader, "view", view);
  shader_set_mat4(&core_data->basic_shader, "proj", proj);

  // glBindVertexArray(core_data->quad_vao);
  // glDrawArrays(GL_TRIANGLES, 0, 6);

  mesh_t* m = assetm_get_mesh_by_idx(core_data->quad_mesh);
  glBindVertexArray(m->vao);
  if (m->indexed)
  { _glDrawElements(GL_TRIANGLES, m->indices_count, GL_UNSIGNED_INT, 0); }
  else
  { _glDrawArrays(GL_TRIANGLES, 0, m->verts_count); }

}

void renderer_direct_draw_quad_textured_handle_3d(vec3 pos, vec3 rot, vec2 scl, u32 handle, rgbf tint)
{
  TRACE();

  mat4 model;
  mat4_make_model(pos, rot, VEC3_XYZ(scl[0], scl[1], 1.0f), model);

  mat4 view;
  camera_get_view_mat(view);
  int w, h;
  window_get_size(&w, &h);
  mat4 proj;
  camera_get_proj_mat(w, h, proj);
  
  // ---- shader & draw call -----	

  shader_use(&core_data->basic_shader);
  shader_set_vec3(&core_data->basic_shader, "tint", tint);
  _glActiveTexture(GL_TEXTURE0);
  _glBindTexture(GL_TEXTURE_2D, handle); 
  shader_set_int(&core_data->basic_shader, "tex", 0);

  shader_set_mat4(&core_data->basic_shader, "model", model);
  shader_set_mat4(&core_data->basic_shader, "view", view);
  shader_set_mat4(&core_data->basic_shader, "proj", proj);

  // glBindVertexArray(core_data->quad_vao);
  // glDrawArrays(GL_TRIANGLES, 0, 6);

  mesh_t* m = assetm_get_mesh_by_idx(core_data->quad_mesh);
  glBindVertexArray(m->vao);
  if (m->indexed)
  { _glDrawElements(GL_TRIANGLES, m->indices_count, GL_UNSIGNED_INT, 0); }
  else
  { _glDrawArrays(GL_TRIANGLES, 0, m->verts_count); }
}

void renderer_direct_draw_quad_textured_handle_mat_3d(mat4 model, u32 handle, rgbf tint)
{
  TRACE();

  mat4 view;
  camera_get_view_mat(view);
  int w, h;
  window_get_size(&w, &h);
  mat4 proj;
  camera_get_proj_mat(w, h, proj);
  
  // ---- shader & draw call -----	

  shader_use(&core_data->basic_shader);
  shader_set_vec3(&core_data->basic_shader, "tint", tint);
  _glActiveTexture(GL_TEXTURE0);
  _glBindTexture(GL_TEXTURE_2D, handle); 
  shader_set_int(&core_data->basic_shader, "tex", 0);

  shader_set_mat4(&core_data->basic_shader, "model", model);
  shader_set_mat4(&core_data->basic_shader, "view", view);
  shader_set_mat4(&core_data->basic_shader, "proj", proj);

  // glBindVertexArray(core_data->quad_vao);
  // glDrawArrays(GL_TRIANGLES, 0, 6);

  mesh_t* m = assetm_get_mesh_by_idx(core_data->quad_mesh);
  glBindVertexArray(m->vao);
  if (m->indexed)
  { _glDrawElements(GL_TRIANGLES, m->indices_count, GL_UNSIGNED_INT, 0); }
  else
  { _glDrawArrays(GL_TRIANGLES, 0, m->verts_count); }
}

void renderer_direct_draw_mesh_textured(vec3 pos, vec3 rot, vec3 scale, mesh_t* m, texture_t* tex, vec3 tint)
{
  TRACE();

	// ---- mvp ----
	mat4 model;
  mat4_make_model(pos, rot, scale, model);

	mat4 view;
  // vec3 center;
  // cam_pos[2] = 10;  // tmp, zoom away
  // vec3_add(cam_pos, VEC3_Z(-1), center);
  // mat4_lookat(cam_pos, center, VEC3_Y(1), view);
  camera_get_view_mat(view);
  

	int w, h;
	window_get_size(&w, &h);
	mat4 proj;
  camera_get_proj_mat(w, h, proj);

	// ---- shader & draw call -----	

	shader_use(&core_data->basic_shader);
	_glActiveTexture(GL_TEXTURE0);
	_glBindTexture(GL_TEXTURE_2D, tex->handle); 
	shader_set_int(&core_data->basic_shader, "tex", 0);
	shader_set_vec3(&core_data->basic_shader, "tint", tint);
	
	shader_set_mat4(&core_data->basic_shader, "model", model);
	shader_set_mat4(&core_data->basic_shader, "view", view);
	shader_set_mat4(&core_data->basic_shader, "proj", proj);

	_glBindVertexArray(m->vao);
  if (m->indexed)
  { _glDrawElements(GL_TRIANGLES, m->indices_count, GL_UNSIGNED_INT, 0); }
  else
  { _glDrawArrays(GL_TRIANGLES, 0, m->verts_count); }
	
}

void renderer_direct_draw_mesh_textured_mat(mat4 model, mesh_t* m, texture_t* tex, vec3 tint)
{
  TRACE();

  _glDisable(GL_BLEND);
  _glEnable(GL_CULL_FACE);
  REMOVE_FLAG(core_data->opengl_state, (opengl_state_flag)OPENGL_BLEND);
  core_data->opengl_state |= OPENGL_CULL_FACE;

  // P_OPENGL_STATE_FLAG(core_data->opengl_state);

	// ---- mvp ----
	mat4 view;
  camera_get_view_mat(view);
  

	int w, h;
	window_get_size(&w, &h);
	mat4 proj;
  camera_get_proj_mat(w, h, proj);

	// ---- shader & draw call -----	

	shader_use(&core_data->basic_shader);
	_glActiveTexture(GL_TEXTURE0);
	_glBindTexture(GL_TEXTURE_2D, tex->handle); 
	shader_set_int(&core_data->basic_shader, "tex", 0);
	shader_set_vec3(&core_data->basic_shader, "tint", tint);
	
	shader_set_mat4(&core_data->basic_shader, "model", model);
	shader_set_mat4(&core_data->basic_shader, "view", view);
	shader_set_mat4(&core_data->basic_shader, "proj", proj);

	_glBindVertexArray(m->vao);
  if (m->indexed)
  { _glDrawElements(GL_TRIANGLES, m->indices_count, GL_UNSIGNED_INT, 0); }
  else
  { _glDrawArrays(GL_TRIANGLES, 0, m->verts_count); }
	
}


void renderer_direct_draw_mesh_preview(vec3 cam_pos, vec3 pos, vec3 rot, vec3 scale, mesh_t* m, texture_t* tex, vec3 tint, texture_t* bg, framebuffer_t* fb)
{
  TRACE();

	framebuffer_bind(fb);
   
  _glViewport(0, 0, fb->width, fb->height);
  _glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
  // -- opengl state --
  _glDisable(GL_DEPTH_TEST);
  _glEnable(GL_CULL_FACE);
  _glCullFace(GL_BACK);
  _glEnable(GL_BLEND); // enable blending of transparent texture
  //set blending function: 1 - source_alpha, e.g. 0.6(60%) transparency -> 1 - 0.6 = 0.4(40%)
  _glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
  _glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
  REMOVE_FLAG(core_data->opengl_state, (opengl_state_flag)OPENGL_DEPTH_TEST);
  core_data->opengl_state |= OPENGL_CULL_FACE;
  core_data->opengl_state |= OPENGL_BLEND;
  core_data->opengl_state |= OPENGL_CULL_FACE_BACK;
  // @TODO: @UNSURE: blend-func
 
  // -- background --
 	
  mat4 model;
  mat4_make_model(VEC3(0), VEC3(0), VEC3(1), model);

	mat4 view;
  vec3 center;
  vec3_add(VEC3_Z(10), VEC3_Z(-1), center);
  mat4_lookat(VEC3_Z(10), center, VEC3_Y(1), view);
  
	mat4 proj;
	float pers = 10;
	m_deg_to_rad(&pers);
	mat4_perspective(pers, ((f32)fb->width / (f32)fb->height), 0.1f, 100.0f, proj);

	shader_use(&core_data->basic_shader);
	shader_set_vec3(&core_data->basic_shader, "tint", VEC3(1));
	_glActiveTexture(GL_TEXTURE0);
	_glBindTexture(GL_TEXTURE_2D, bg->handle); 
	shader_set_int(&core_data->basic_shader, "tex", 0);
	
	shader_set_mat4(&core_data->basic_shader, "model", model);
	shader_set_mat4(&core_data->basic_shader, "view", view);
	shader_set_mat4(&core_data->basic_shader, "proj", proj);

  mesh_t* quad_m = assetm_get_mesh_by_idx(core_data->quad_mesh);
	_glBindVertexArray(quad_m->vao);
  if (quad_m->indexed)
  { _glDrawElements(GL_TRIANGLES, quad_m->indices_count, GL_UNSIGNED_INT, 0); }
  else
  { _glDrawArrays(GL_TRIANGLES, 0, quad_m->verts_count); }

  glEnable(GL_DEPTH_TEST);
  
  // -- mesh --

  mat4_make_model(pos, rot, scale, model);

  vec3_add(cam_pos, VEC3_Z(-1), center);
  mat4_lookat(cam_pos, center, VEC3_Y(1), view);
  
	mat4_perspective(pers, ((f32)fb->width / (f32)fb->height), 0.1f, 100.0f, proj);

	shader_use(&core_data->basic_shader);
	_glActiveTexture(GL_TEXTURE0);
	_glBindTexture(GL_TEXTURE_2D, tex->handle); 
	shader_set_int(&core_data->basic_shader, "tex", 0);
	shader_set_vec3(&core_data->basic_shader, "tint", tint);
	
	shader_set_mat4(&core_data->basic_shader, "model", model);
	shader_set_mat4(&core_data->basic_shader, "view", view);
	shader_set_mat4(&core_data->basic_shader, "proj", proj);

	_glBindVertexArray(m->vao);
  if (m->indexed)
  { _glDrawElements(GL_TRIANGLES, m->indices_count, GL_UNSIGNED_INT, 0); }
  else
  { _glDrawArrays(GL_TRIANGLES, 0, m->verts_count); }

  framebuffer_unbind();
	
}
void renderer_direct_draw_line(vec3 pos0, vec3 pos1, vec3 tint, f32 width)
{
  TRACE();

	// ---- mvp ----
	mat4 model;
  mat4_make_model(VEC3(0), VEC3(0), VEC3(1), model);

	mat4 view;
  camera_get_view_mat(view);
  
	int w, h;
	window_get_size(&w, &h);
	mat4 proj;
  camera_get_proj_mat(w, h, proj);

  _glLineWidth(width);

  // ---- vbo sub data ----

  _glBindBuffer(GL_ARRAY_BUFFER, core_data->line_mesh.vbo);
  _glBufferSubData(GL_ARRAY_BUFFER, 0 * sizeof(f32),               3 * sizeof(f32), pos0);
  _glBufferSubData(GL_ARRAY_BUFFER, FLOATS_PER_VERT * sizeof(f32), 3 * sizeof(f32), pos1);

	// ---- shader & draw call -----	

	shader_use(&core_data->basic_shader);
	_glActiveTexture(GL_TEXTURE0);
	_glBindTexture(GL_TEXTURE_2D, (assetm_get_texture("#internal/blank.png", true))->handle); 
	shader_set_int(&core_data->basic_shader, "tex", 0);
	shader_set_vec3(&core_data->basic_shader, "tint", tint);
	
	shader_set_mat4(&core_data->basic_shader, "model", model);
	shader_set_mat4(&core_data->basic_shader, "view", view);
	shader_set_mat4(&core_data->basic_shader, "proj", proj);

	_glBindVertexArray(core_data->line_mesh.vao);
  _glDrawArrays(GL_LINES, 0, 2);
}
void renderer_direct_draw_triangle(vec3 pos0, vec3 pos1, vec3 pos2, vec3 tint, f32 width)
{
  TRACE();

	// ---- mvp ----
	mat4 model;
  mat4_make_model(VEC3(0), VEC3(0), VEC3(1), model);

	mat4 view;
  camera_get_view_mat(view);
  
	int w, h;
	window_get_size(&w, &h);
	mat4 proj;
  camera_get_proj_mat(w, h, proj);

  _glLineWidth(width);

  // ---- vbo sub data ----

  _glBindBuffer(GL_ARRAY_BUFFER, core_data->triangle_mesh.vbo);
  _glBufferSubData(GL_ARRAY_BUFFER, FLOATS_PER_VERT * sizeof(f32) * 0, 3 * sizeof(f32), pos0);
  _glBufferSubData(GL_ARRAY_BUFFER, FLOATS_PER_VERT * sizeof(f32) * 1, 3 * sizeof(f32), pos1);
  _glBufferSubData(GL_ARRAY_BUFFER, FLOATS_PER_VERT * sizeof(f32) * 2, 3 * sizeof(f32), pos2);
  _glBufferSubData(GL_ARRAY_BUFFER, FLOATS_PER_VERT * sizeof(f32) * 3, 3 * sizeof(f32), pos0);

	// ---- shader & draw call -----	

	shader_use(&core_data->basic_shader);
	_glActiveTexture(GL_TEXTURE0);
	_glBindTexture(GL_TEXTURE_2D, (assetm_get_texture("#internal/blank.png", true))->handle); 
	shader_set_int(&core_data->basic_shader, "tex", 0);
  vec3_mul_f(tint, 0.75f, tint);
	shader_set_vec3(&core_data->basic_shader, "tint", tint);
	
	shader_set_mat4(&core_data->basic_shader, "model", model);
	shader_set_mat4(&core_data->basic_shader, "view", view);
	shader_set_mat4(&core_data->basic_shader, "proj", proj);

	_glBindVertexArray(core_data->triangle_mesh.vao);
  // _glDrawArrays(GL_LINES, 0, 4);
  // _glDrawArrays(GL_LINE_LOOP, 0, 4); 
  _glDrawArrays(GL_TRIANGLES,  0, 4);
  vec3_mul_f(tint, 1.3333333333f, tint);
	shader_set_vec3(&core_data->basic_shader, "tint", tint);
  _glDrawArrays(GL_LINE_STRIP, 0, 4); 
}

void renderer_direct_draw_entity_pbr( entity_t* e )
{
  int texture_arr_len = 0;
  texture_t* texture_arr = assetm_get_texture_arr(&texture_arr_len);
  int material_arr_len = 0;
  material_t* material_arr = assetm_get_material_arr(&material_arr_len);
  int mesh_arr_len = 0;
  mesh_t* mesh_arr = assetm_get_mesh_arr(&mesh_arr_len);

  int dir_lights_len = 0;
  dir_light_t* dir_lights = state_dir_light_get_arr(&dir_lights_len);
  int point_lights_len = 0;
  int point_lights_dead_len = 0;
  point_light_t* point_lights = state_point_light_get_arr(&point_lights_len, &point_lights_dead_len);

  material_t* mat = &material_arr[e->mat]; // [m]
  // material_t* mat = assetm_get_material(MATERIAL_TEMPLATE_BRICK); // [m]

  // @TODO: do this outside the for-loop
  shader_t* mat_shader = &core_data->pbr_forward_shader;
  // if (mat->shader >= 0) { mat_shader = assetm_get_shader_by_idx(mat->shader); }

  shader_use(mat_shader);

  rgbf tint;
  vec3_mul(mat->tint,  e->tint, tint);
  shader_set_vec3(mat_shader, "tint", tint);

  int tex_idx = 0;
  shader_set_int(mat_shader, "albedo_map", tex_idx);
  _glActiveTexture(GL_TEXTURE0 + (GLenum)tex_idx); tex_idx++;
  _glBindTexture(GL_TEXTURE_2D, texture_arr[mat->albedo].handle); 

  shader_set_int(mat_shader, "normal_map", tex_idx);
  _glActiveTexture(GL_TEXTURE0 + (GLenum)tex_idx); tex_idx++;
  _glBindTexture(GL_TEXTURE_2D, texture_arr[mat->normal].handle); 

  shader_set_int(mat_shader, "roughness_map", tex_idx);
  _glActiveTexture(GL_TEXTURE0 + (GLenum)tex_idx); tex_idx++;
  _glBindTexture(GL_TEXTURE_2D, texture_arr[mat->roughness].handle); 

  shader_set_int(mat_shader, "metallic_map", tex_idx);
  _glActiveTexture(GL_TEXTURE0 + (GLenum)tex_idx); tex_idx++;
  _glBindTexture(GL_TEXTURE_2D, texture_arr[mat->metallic].handle);

  shader_set_int(mat_shader, "emissive_map", tex_idx);
  _glActiveTexture(GL_TEXTURE0 + (GLenum)tex_idx); tex_idx++;
  _glBindTexture(GL_TEXTURE_2D, texture_arr[mat->emissive].handle);

  _glActiveTexture(GL_TEXTURE0 + (GLenum)tex_idx);
  _glBindTexture(GL_TEXTURE_CUBE_MAP, core_data->cube_map.irradiance);
  shader_set_int(mat_shader, "irradiance_map", tex_idx);
  tex_idx++;
  _glActiveTexture(GL_TEXTURE0 + (GLenum)tex_idx);
  _glBindTexture(GL_TEXTURE_CUBE_MAP, core_data->cube_map.prefilter);
  shader_set_int(mat_shader, "prefilter_map", tex_idx);
  tex_idx++;
  _glActiveTexture(GL_TEXTURE0 + (GLenum)tex_idx);
  _glBindTexture(GL_TEXTURE_2D, core_data->brdf_lut);
  shader_set_int(mat_shader, "brdf_lut", tex_idx);
  tex_idx++;

  ERR_CHECK(tex_idx <= 31, "bound GL_TEXTURE%d, max: 31\n", tex_idx);

  shader_set_float(mat_shader, "roughness_f", mat->roughness_f);
  shader_set_float(mat_shader, "metallic_f", mat->metallic_f);
  shader_set_float(mat_shader, "emissive_f", mat->emissive_f);


  shader_set_vec3(mat_shader, "view_pos", core_data->cam.pos); // cam_pos
  shader_set_float(mat_shader, "cube_map_intensity", core_data->cube_map.intensity);

  // lights ----------------------------------------------
#define BUF_SIZE2 28
  char buffer[BUF_SIZE2];
  // int  shadow_idx = 0;
  shader_set_int(mat_shader, "dir_lights_len", dir_lights_len);
  for (int i = 0; i < dir_lights_len; ++i)
  {
    dir_light_t* light = &dir_lights[i];

    int idx = i; //  - disabled_lights;
    SPRINTF(BUF_SIZE2, buffer, "dir_lights[%d].direction", idx);
    shader_set_vec3(mat_shader, buffer, light->dir);

    vec3 color;
    vec3_mul_f(light->color, light->intensity, color);
    SPRINTF(BUF_SIZE2, buffer, "dir_lights[%d].color", idx);
    shader_set_vec3(mat_shader, buffer, color);
  }
  shader_set_int(mat_shader, "point_lights_len", point_lights_len - point_lights_dead_len);
  ERR_CHECK(point_lights_len - point_lights_dead_len <= 8, "can only have 8 point lights, bc lighting_shader\n");
  int point_lights_disabled = 0;
  for (int i = 0; i < point_lights_len; ++i)
  {
    point_light_t* light = &point_lights[i];
    if (light->is_dead) { point_lights_disabled++; continue; }
    entity_t* pl = state_entity_get(light->entity_id);
    vec3 pl_pos;
    mat4_get_pos(pl->model, pl_pos);
    vec3 l_pos; vec3_add(pl_pos, light->offset, l_pos);

    int idx = i - point_lights_disabled;
    SPRINTF(BUF_SIZE2, buffer, "point_lights[%d].pos", idx);
    shader_set_vec3(mat_shader, buffer, l_pos); // light->pos_ptr);
    SPRINTF(BUF_SIZE2, buffer, "point_lights[%d].color", idx);
    shader_set_vec3(mat_shader, buffer, light->color);
    SPRINTF(BUF_SIZE2, buffer, "point_lights[%d].intensity", idx);
    shader_set_float(mat_shader, buffer, light->intensity);
  }
  #undef BUF_SIZE2

  vec2 tile; 
  vec2_copy(mat->tile, tile);
  if (mat->tile_by_scl) 
  { 
    f32 uv_scl = ( e->scl[0] + e->scl[1] + e->scl[2] ) / 3;
    vec2_mul_f(tile, uv_scl, tile); 
  }
  vec2_mul_f(tile, mat->tile_scl, tile);
  shader_set_vec2(mat_shader, "uv_tile", tile);

  mat4 view, proj;
  camera_get_view_mat(view);
  int w, h;
  window_get_size( &w, &h );
  camera_get_proj_mat(w, h, proj);

  shader_set_mat4(mat_shader, "model", e->model);  // model gets updated in shadow map
  shader_set_mat4(mat_shader, "view", view);
  shader_set_mat4(mat_shader, "proj", proj);

  if (mat_shader->set_uniforms_f != NULL) { mat_shader->set_uniforms_f(mat_shader, tex_idx); }

  // mesh_t* mesh = assetm_get_mesh_by_idx(e->mesh); // [m]
  mesh_t* mesh = &mesh_arr[e->mesh]; // [m]

  DRAW_MESH(mesh);
  core_data->draw_calls_total++;
  core_data->draw_calls_forward++;
}

