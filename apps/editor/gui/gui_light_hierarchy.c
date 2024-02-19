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
#include "editor/app.h"
#include "core/core_data.h"
#include "core/window.h"
#include "core/state/state.h"


void gui_light_hierarchy_win(ui_context* ctx, ui_rect win_rect, const u32 win_flags)
{
  // int w, h;
  // window_get_size(&w, &h);
  // // less height because the window bar on top and below
  // light_hierarchy_win_ratio.w = 0.1f;
  // light_hierarchy_win_ratio.h = 1.0f - template_win_ratio.h;
  // light_hierarchy_win_ratio.x = 0.0f;
  // light_hierarchy_win_ratio.y = 0.0f + top_bar_win_ratio.h; 
  // light_hierarchy_win_rect = nk_rect(light_hierarchy_win_ratio.x * w, light_hierarchy_win_ratio.y * h, 
  //                                    light_hierarchy_win_ratio.w * w, light_hierarchy_win_ratio.h * h);
  if (nk_begin(ctx, "light hierarchy", win_rect, win_flags)) 
  {
    nk_layout_row_dynamic(ctx, 30, 1);
    int dl_len = 0;
    dir_light_t* dl   = state_dir_light_get_arr(&dl_len);
    int pl_len = 0;
    int pl_dead_len = 0;
    point_light_t* pl = state_point_light_get_arr(&pl_len, &pl_dead_len);

    const int SEL_LIGHT_NONE  = 0;
    const int SEL_LIGHT_DIR   = 1;
    const int SEL_LIGHT_POINT = 2;

    static int selected_type = 0; // vs is stupid so commented out SEL_LIGHT_NONE;
    static int selected = -1;


    nk_property_float(ctx, "cube map intensity", 0.0f, &core_data->cube_map.intensity, 100.0f, 0.1f, 0.01f);

    if (nk_button_label(ctx, "add dir light")) 
    {
      state_dir_light_add(VEC3(0), VEC3_Y(-1), RGB_F(1, 1, 1), 1, false, 0, 0);
    } 
    if (nk_button_label(ctx, "add point light"))
    {
      vec3 front, pos;
      vec3_copy(core_data->cam.front, front); // camera_get_front(front);
      vec3_copy(core_data->cam.pos,   pos);   // camera_get_pos(pos);
      vec3_mul_f(front, 8.0f, front);
      vec3_add(front, pos, pos);
      state_point_light_add_empty(pos, RGB_F(1.0f, 0.0f, 1.0f), 1.0f);
    }

    if (nk_tree_push(ctx, NK_TREE_TAB, "hierarchy", NK_MAXIMIZED))
    {
      for (int i = 0; i < dl_len; ++i)
      {
        char buf[32];
        sprintf(buf, "dir light: %d", i);
        nk_bool selec = i == selected && selected_type == SEL_LIGHT_DIR;
        nk_selectable_label(ctx, buf, NK_TEXT_LEFT, &selec);
        if (selec) { selected = i; selected_type = SEL_LIGHT_DIR; }     // select
        if (!selec && i == selected && selected_type == SEL_LIGHT_DIR) 
        { selected = -1; selected_type = SEL_LIGHT_NONE; }              // deselect
      }
      for (int i = 0; i < pl_len; ++i)
      {
        if (pl->is_dead) { continue; }
        char buf[32];
        sprintf(buf, "point light: %d", i);
        nk_bool selec = i == selected && selected_type == SEL_LIGHT_POINT;
        nk_selectable_label(ctx, buf, NK_TEXT_LEFT, &selec);
        // if (selec) { selected = i; selected_type = SEL_LIGHT_POINT; debug_draw_sphere_register(pl[i].pos, 0.35f, pl[i].color); } // select
        if (!selec && i == selected && selected_type == SEL_LIGHT_POINT) 
        { selected = -1; selected_type = SEL_LIGHT_NONE; }                                                                       // deselect
      }
      nk_tree_pop(ctx);
    }

    if (selected >= 0 && selected_type == SEL_LIGHT_DIR && nk_tree_push(ctx, NK_TREE_TAB, "properties", NK_MAXIMIZED))
    {
      dir_light_t* l = &dl[selected];
      nk_layout_row_dynamic(ctx, 30, 1);

      if (nk_button_label(ctx, "remove")) 
      {
        state_dir_light_remove(selected);
        selected = -1;
      }
      
      nk_label(ctx, "direction", NK_TEXT_LEFT);
      nk_property_float(ctx, "d.x", -2048.0f, &l->dir[0], 2048.0f, 0.1f, 0.01f);
      nk_property_float(ctx, "d.y", -2048.0f, &l->dir[1], 2048.0f, 0.1f, 0.01f);
      nk_property_float(ctx, "d.z", -2048.0f, &l->dir[2], 2048.0f, 0.1f, 0.01f);
      
      gui_color_selector(l->color);
      
      nk_property_float(ctx, "intens.", -1.0f, &l->intensity, 2048.0f, 0.1f, 0.01f);

      if (selected == 0)
      {
        // @NOTE: need to pass a nk_bool
        nk_bool cast_shadow = l->cast_shadow;
        nk_checkbox_label(ctx, "cast shadows", &cast_shadow);
        l->cast_shadow = cast_shadow;

        nk_labelf(ctx, NK_TEXT_LEFT, "shadowmap");
        nk_labelf(ctx, NK_TEXT_LEFT, "x: %d, y: %d", l->shadow_map_x, l->shadow_map_y);
      }
      
      nk_tree_pop(ctx);
    }
    else if (selected >= 0 && selected_type == SEL_LIGHT_POINT && nk_tree_push(ctx, NK_TREE_TAB, "properties", NK_MAXIMIZED))
    {
      point_light_t* l = &pl[selected];
      nk_layout_row_dynamic(ctx, 30, 1);

      if (nk_button_label(ctx, "remove")) 
      {
        state_point_light_remove(selected);
        selected = -1;
      }

      nk_label(ctx, "position offset", NK_TEXT_LEFT);
      nk_property_float(ctx, "p.x", -2048.0f, &l->offset[0], 2048.0f, 0.1f, 0.01f);
      nk_property_float(ctx, "p.y", -2048.0f, &l->offset[1], 2048.0f, 0.1f, 0.01f);
      nk_property_float(ctx, "p.z", -2048.0f, &l->offset[2], 2048.0f, 0.1f, 0.01f);
      // nk_labelf(ctx, NK_LEFT, "p.x %.2f", (*l->pos_ptr)[0]);
      // nk_labelf(ctx, NK_LEFT, "p.y %.2f", (*l->pos_ptr)[1]);
      // nk_labelf(ctx, NK_LEFT, "p.z %.2f", (*l->pos_ptr)[2]);
      
      gui_color_selector(l->color);
      
      nk_property_float(ctx, "intens.", -1.0f, &l->intensity, 2048.0f, 0.1f, 0.01f);
      
      nk_tree_pop(ctx);
    }
  }
  nk_end(ctx); 
}


