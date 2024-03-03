#include "core/camera.h"
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_KEYSTATE_BASED_INPUT
#include "nuklear/nuklear.h"
#include "editor/gui/gui.h"
#include "editor/gui/gui_style.h"
// #include "editor/app.h"
#include "core/core_data.h"
#include "core/window.h"
#include "core/io/assetm.h"


void gui_assetm_win(ui_context* ctx, ui_rect win_rect, const u32 win_flags)
{
  int w, h;
  window_get_size(&w, &h);

  if (nk_begin(ctx, "assetm", win_rect, win_flags)) 
  {
    int texture_arr_len = 0;
    texture_t* texture_arr = assetm_get_texture_arr(&texture_arr_len);
    int mesh_arr_len = 0;
    mesh_t* mesh_arr = assetm_get_mesh_arr(&mesh_arr_len);
    int shader_arr_len = 0;
    shader_t* shader_arr = assetm_get_shader_arr(&shader_arr_len);
    int material_arr_len = 0;
    material_t* material_arr = assetm_get_material_arr(&material_arr_len);

    nk_layout_row_dynamic(ctx, 20, 1);
    if (nk_tree_push(ctx, NK_TREE_TAB, "textures", NK_MINIMIZED))
    {
      for (int i = 0; i < texture_arr_len; ++i)
      {
        #ifdef EDITOR // kinda unnecesasary, but verbose, so, yeah
        // if (nk_tree_push(ctx, NK_TREE_TAB, texture_arr[i].name, NK_MINIMIZED))
        if ( nk_tree_push_hashed(ctx, NK_TREE_TAB, texture_arr[i].name, NK_MINIMIZED, 
                                 texture_arr[i].name, strlen(texture_arr[i].name), __LINE__) )
        #else // EDITOR
        if (nk_tree_push(ctx, NK_TREE_TAB, "texture", NK_MINIMIZED))
        #endif // EDITOR
        {
          ui_rect win_rect_real = nk_window_get_bounds(ctx);
          gui_texture_properties(ctx, win_rect_real, &texture_arr[i]);
          nk_tree_pop(ctx);
        }
      }
      nk_tree_pop(ctx);
    }
    if (nk_tree_push(ctx, NK_TREE_TAB, "meshes", NK_MINIMIZED))
    {
      for (int i = 0; i < mesh_arr_len; ++i)
      {
        #ifdef EDITOR // kinda unnecesasary, but verbose, so, yeah
        if ( nk_tree_push_hashed(ctx, NK_TREE_TAB, mesh_arr[i].name, NK_MINIMIZED, 
                                 mesh_arr[i].name, strlen(mesh_arr[i].name), __LINE__) )
        #else // EDITOR
        if (nk_tree_push(ctx, NK_TREE_TAB, "mesh", NK_MINIMIZED))
        #endif // EDITOR
        {
          ui_rect win_rect_real = nk_window_get_bounds(ctx);
          gui_mesh_properties(ctx, win_rect_real, &mesh_arr[i]);
          nk_tree_pop(ctx);
        }
      }
      nk_tree_pop(ctx);
    }
    if (nk_tree_push(ctx, NK_TREE_TAB, "shaders", NK_MINIMIZED))
    {
      for (int i = 0; i < shader_arr_len; ++i)
      {
        #ifdef EDITOR // kinda unnecesasary, but verbose, so, yeah
        if ( nk_tree_push_hashed(ctx, NK_TREE_TAB, shader_arr[i].name, NK_MINIMIZED, 
                                 shader_arr[i].name, strlen(shader_arr[i].name), __LINE__) )
        #else // EDITOR
        if (nk_tree_push(ctx, NK_TREE_TAB, "shader", NK_MINIMIZED))
        #endif // EDITOR
        {
          ui_rect win_rect_real = nk_window_get_bounds(ctx);
          gui_shader_properties(ctx, win_rect_real, &shader_arr[i]);
          nk_tree_pop(ctx);
        }
      }
      nk_tree_pop(ctx);
    }
    if (nk_tree_push(ctx, NK_TREE_TAB, "materials", NK_MINIMIZED))
    {
      for (int i = 0; i < material_arr_len; ++i)
      {
        #ifdef EDITOR // kinda unnecesasary, but verbose, so, yeah
        if ( nk_tree_push_hashed(ctx, NK_TREE_TAB, material_arr[i].name, NK_MINIMIZED, 
                                 material_arr[i].name, strlen(material_arr[i].name), __LINE__) )
        #else // EDITOR
        if (nk_tree_push(ctx, NK_TREE_TAB, "materials", NK_MINIMIZED))
        #endif // EDITOR
        {
          ui_rect win_rect_real = nk_window_get_bounds(ctx);
          gui_material_properties(ctx, win_rect_real, &material_arr[i]);
          nk_tree_pop(ctx);
        }
      }
      nk_tree_pop(ctx);
    }
  }
  nk_end(ctx);
}

void gui_texture_properties(ui_context* ctx, ui_rect win_rect, texture_t* t)
{
  nk_layout_row_dynamic(ctx, 20, 1);
  nk_labelf(ctx, NK_LEFT, "loaded: %s", STR_BOOL(t->loaded));
  if (!t->loaded) { return; }

  #ifdef EDITOR // kinda unnecesasary, but verbose, so, yeah
  nk_labelf(ctx, NK_LEFT, "name:   %s", t->name);
  #endif // EDITOR

  nk_labelf(ctx, NK_LEFT, "handle: %u", t->handle);
  nk_labelf(ctx, NK_LEFT, "width:  %d", t->width);
  nk_labelf(ctx, NK_LEFT, "height: %d", t->height);
  nk_labelf(ctx, NK_LEFT, "channel_nr: %d", t->channel_nr);
    
  const int size = win_rect.w - 75;
  nk_layout_row_static(ctx, size, size, 1);
  nk_image(ctx, nk_image_id(t->handle));

}
void gui_mesh_properties(ui_context* ctx, ui_rect win_rect, mesh_t* m)
{
  nk_layout_row_dynamic(ctx, 20, 1);
  nk_labelf(ctx, NK_LEFT, "loaded: %s", STR_BOOL(m->loaded));
  if (!m->loaded) { return; }
  
  nk_labelf(ctx, NK_LEFT, "indexed: %s", STR_BOOL(m->indexed));

  #ifdef EDITOR // kinda unnecesasary, but verbose, so, yeah
  nk_labelf(ctx, NK_LEFT, "name:   %s", m->name);
  #endif // EDITOR

  nk_labelf(ctx, NK_LEFT, "floats_per_vert: %d", m->floats_per_vert);
  nk_labelf(ctx, NK_LEFT, "verts_count:     %d", m->verts_count);
  nk_labelf(ctx, NK_LEFT, "indices_count:   %d", m->indices_count);
  
  nk_labelf(ctx, NK_LEFT, "vao: %u", m->vao);
  nk_labelf(ctx, NK_LEFT, "vbo: %u", m->vbo);
  nk_labelf(ctx, NK_LEFT, "ebo: %u", m->ebo);
}

void gui_shader_properties(ui_context* ctx, ui_rect win_rect, shader_t* s)
{
  nk_layout_row_dynamic(ctx, 20, 1);
  // nk_labelf(ctx, NK_TEXT_LEFT, "%s:", name);
  nk_labelf(ctx, NK_TEXT_LEFT, "has_error: %s", STR_BOOL(s->has_error));
  nk_labelf(ctx, NK_TEXT_LEFT, "handle: %d", s->handle);
  #ifdef EDITOR // kinda unnecesasary, but verbose, so, yeah
  nk_labelf(ctx, NK_LEFT, "name:           %s", s->name);
  if (s->set_uniforms_f)
  { nk_labelf(ctx, NK_LEFT, "set_uniforms_f: %s", s->set_uniforms_f_name); }
  #endif // EDITOR
}

void gui_material_properties(ui_context* ctx, ui_rect win_rect, material_t* m)
{
  nk_layout_row_dynamic(ctx, 20, 1);
  #ifdef EDITOR // kinda unnecesasary, but verbose, so, yeah
  nk_labelf(ctx, NK_LEFT, "name: %s", m->name);
  #endif // EDITOR

  nk_labelf(ctx, NK_LEFT, "template_idx: %d", m->template_idx);
  nk_labelf(ctx, NK_LEFT, "shader:       %d", m->shader);
  
  nk_layout_row_dynamic(ctx, 20, 2);
  nk_labelf(ctx, NK_LEFT, "albedo:    %d", m->albedo);
  nk_labelf(ctx, NK_LEFT, "normal:    %d", m->normal);
  nk_labelf(ctx, NK_LEFT, "roughness: %d", m->roughness);
  nk_labelf(ctx, NK_LEFT, "metallic:  %d", m->metallic);
  nk_labelf(ctx, NK_LEFT, "emissive:  %d", m->emissive);
  
  const int size = (win_rect.w *0.5f) - 75;
  nk_layout_row_static(ctx, size, size, 2);
  nk_image(ctx, nk_image_id(assetm_get_texture_by_idx(m->albedo)->handle));
  nk_image(ctx, nk_image_id(assetm_get_texture_by_idx(m->normal)->handle));
  nk_image(ctx, nk_image_id(assetm_get_texture_by_idx(m->roughness)->handle));
  nk_image(ctx, nk_image_id(assetm_get_texture_by_idx(m->metallic)->handle));
  nk_image(ctx, nk_image_id(assetm_get_texture_by_idx(m->emissive)->handle));
}


