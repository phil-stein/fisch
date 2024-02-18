#include "core/renderer/renderer_direct.h"
#include "core/text/text_inc.h"
#include "core/types/shader.h"
#include "core/types/texture.h"
#include "core/core_data.h"
#include "core/window.h"
#include "core/io/file_io.h"
#include "core/io/assetm.h"
#include "core/debug/debug_opengl.h"
#include "math/math_inc.h"

// #include "stb/stb_truetype.h" // defined in program.c
#include "stb/stb_image.h"


static core_data_t* core_data = NULL;

shader_t  text_shader;
font_t font;

u32 quad_vao, quad_vbo;

void text_init()
{
  TRACE();
  
  core_data = core_data_get();

  int tex = 0;	// idx into atlas
  
  // const f32 ratio_w  = 380.0f / 256.0f; 
  // const f32 ratio_w2 = 256.0f / 380.0f;
  // const f32 w        = (1.0f / 86.0f);
  // // const f32 ratio_w2 = ((256.0f / 380.0f) * 0.5f) + 0.5f;
  const f32 ratio_w  = 1.0f; 
  const f32 w        = (1.0f / 86.0f);
  
  // const f32 u_max = ( ( w * (f32)(tex +1) ) - (w * 0.5f) ) * ratio_w; 
  // const f32 u_min = ( ( w * (f32)(tex) )    - (w * 0.5f) ) * ratio_w; 
  const f32 u_max = ( ( w * (f32)(tex +1) )) ; // * ratio_w2; 
  const f32 u_min = ( ( w * (f32)(tex -0)    )) ; // * ratio_w2; 
  const f32 u_top = 0.0f; // -4.5f * w;
  const f32 v_max = 1.0f; // 0.95f;
  const f32 v_min = 0.0f;

  const f32 scl   = 1.0f / 4.0f;
  const f32 w_max = 1.0f * scl; // * ratio_w2;
  const f32 h_max = 1.0f * scl * ratio_w;
  const f32 w_min = 0.0f;
  const f32 h_min = 0.0f;
  
  const f32 quad_verts[] = 
  { 
  // positions	  // tex coords
   w_min, h_max,  u_min+u_top, v_max,	 // top-left
   w_min, h_min,  u_min,       v_min,  // bottom-left
   w_max, h_min,  u_max,       v_min,  // bottom-right
  
   w_min, h_max,  u_min+u_top, v_max,	 // top-left
   w_max, h_min,  u_max,       v_min,  // bottom-right 
   w_max, h_max,  u_max+u_top, v_max	 // top-right
  };
  
  // screen quad VAO
  _glGenVertexArrays(1, &quad_vao);
  _glGenBuffers(1, &quad_vbo);
  _glBindVertexArray(quad_vao);
  _glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
  _glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(f32), &quad_verts, GL_STATIC_DRAW); // quad_verts is 24 long
  _glEnableVertexAttribArray(0);
  _glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (void*)0);
  _glEnableVertexAttribArray(1);
  _glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (void*)(2 * sizeof(f32)));

  text_load_font("jetbrains_mono_nl/jetbrains_mono_nl_atlas_02_32.png", &font);

  char vert_path[ASSET_PATH_MAX + 64];
  char frag_path[ASSET_PATH_MAX + 64];
  SPRINTF(ASSET_PATH_MAX + 64, vert_path, "%sshaders/text/text02.vert", core_data->asset_path);
  SPRINTF(ASSET_PATH_MAX + 64, frag_path, "%sshaders/text/text02.frag", core_data->asset_path);
  text_shader = shader_create_from_file(vert_path, frag_path, NULL, "text_shader");
}

void text_load_font(const char* name, font_t* _font)
{
  TRACE();
  
  STRCPY(_font->name, name);

  // char path[ASSET_PATH_MAX + FONT_NAME_MAX];
  // SPRINTF(ASSET_PATH_MAX + FONT_NAME_MAX, path, "%stextures/%s", core_data->asset_path, name);
  // P_STR(path);

  // _font->atlas = texture_create_from_path(path, true);
  // u8* pixels;
  int width, height;
  int channel_num = 0;
  void*  buf = NULL;
  size_t buf_len = 0;
  // texture_load_pixels(path, &pixels, &width, &height, &channel_num, true);
  char path[ASSET_PATH_MAX + FONT_NAME_MAX + 12];
  int len = 0;
  SPRINTF(ASSET_PATH_MAX + FONT_NAME_MAX + 12, path, "%stextures/%s", core_data->asset_path, name);
  P_STR(path);
  
  // -- load pixels --
  buf = (void*)file_io_read_len(path, &len);
  buf_len = len;
  ERR_CHECK(buf != NULL && buf_len > 0, "texture '%s' requested in %s(), doesn't exist in the asset folder.\n", name, __func__);
  stbi_set_flip_vertically_on_load(true);
  u8* pixels = stbi_load_from_memory(buf, buf_len, &width, &height, &channel_num, 0);
  ERR_CHECK(channel_num == 1, "texture %s in %s(), wasnt single channel\n", name, __func__);
  // u32 handle = texture_create_from_pixels(pixels, width, height, channel_num, false);
  
  int gl_internal_format = GL_R8;
  int gl_format = GL_RED;

  u32 handle = 0;
  _glGenTextures(1, &handle);
  _glBindTexture(GL_TEXTURE_2D, handle);
  // no interpolation
  _glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  _glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  _glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  _glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // _glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  _glTexImage2D(GL_TEXTURE_2D, 0, gl_internal_format, width, height, 0, gl_format, GL_UNSIGNED_BYTE, pixels);
  // _glGenerateMipmap(GL_TEXTURE_2D);
  _glBindTexture(GL_TEXTURE_2D, 0);
    
  FREE(buf);
  FREE(pixels);

  // texture_t tex;
  _font->atlas.handle     = handle;
  _font->atlas.width      = width;
  _font->atlas.height     = height;
  _font->atlas.channel_nr = channel_num;
  P_INT(width);
  P_INT(height);
  P_INT(channel_num);
  
  // _font->atlas = assetm_get_texture_idx(name, false);
  // _font->atlas = assetm_get_texture_idx("stylized_planks/stylized_planks_albedo.jpg", false);
}

void text_draw_char()
{
  TRACE();
  
  font_t* _font = &font;
  // renderer_direct_draw_quad_textured(VEC2(0), 10, VEC2(0), VEC2(1), &_font->atlas, RGB_F(1, 1, 1));
  
  // ---- mvp ----

  mat4 model;
  mat4_make_model_2d(VEC2(0), VEC2_XY(1, 1), 0.0f, model);

  mat4 view;
  mat4_lookat_2d(VEC2(0), 10, view);

  int w, h;
  window_get_size(&w, &h);
  mat4 proj;
  camera_get_proj_mat(w, h, proj);

  // _glClear(GL_COLOR_BUFFER_BIT);
  _glDisable(GL_DEPTH_TEST);

  // ---- shader & draw call -----	

  shader_use(&text_shader);
  shader_set_vec3(&text_shader, "tint", RGB_F(1, 1, 1));
  _glActiveTexture(GL_TEXTURE0);
  // _glBindTexture(GL_TEXTURE_2D, assetm_get_texture_by_idx(_font->atlas)->handle); 
  _glBindTexture(GL_TEXTURE_2D, _font->atlas.handle); 
  shader_set_int(&text_shader, "tex", 0);

  // shader_set_mat4(&text_shader, "model", model);
  // shader_set_mat4(&text_shader, "view", view);
  shader_set_mat4(&text_shader, "proj", proj);
  shader_set_float(&text_shader, "scl", 0.5f);
  shader_set_vec2(&text_shader,  "pos", VEC2_XY(0.0f, -0.3f));

  // glBindVertexArray(core_data->quad_vao);
  // glDrawArrays(GL_TRIANGLES, 0, 6);

  // mesh_t* m = assetm_get_mesh_by_idx(core_data->quad_mesh);
  // glBindVertexArray(m->vao);
  // if (m->indexed)
  // { _glDrawElements(GL_TRIANGLES, m->indices_count, GL_UNSIGNED_INT, 0); }
  // else
  // { _glDrawArrays(GL_TRIANGLES, 0, m->verts_count); }
  _glBindVertexArray(quad_vao);
  _glDrawArrays(GL_TRIANGLES, 0, 6);
  
  _glEnable(GL_DEPTH_TEST);
}

void text_draw_str(font_t* _font)
{
  TRACE();

}


