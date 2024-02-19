#include "editor/gui/gui.h"
#include "editor/gui/gui_style.h"
#include "editor/app.h"
#include "editor/gizmo.h"
#include "editor/operation.h"
#include "editor/stylesheet.h"
#include "core/core_data.h"
#include "core/window.h"
#include "core/renderer/renderer.h"
#include "core/renderer/renderer_direct.h"
#include "core/renderer/renderer_extra.h"
#include "core/state/state.h"
#include "core/io/input.h"
#include "core/io/assetm.h"
#include "core/io/save_sys/save_sys.h"
#include "core/camera.h"
#include "core/types/types.h"
#include "core/debug/debug_timer.h"
#include "core/debug/debug_draw.h"
#include "core/templates/entity_template.h"

// @NOTE: tmp
#include "phys/phys_world.h"
#include "phys/phys_debug_draw.h"

#include "stb/stb_ds.h"

#include "GLAD/glad.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_KEYSTATE_BASED_INPUT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#include "nuklear/nuklear.h"
#include "nuklear/nuklear_glfw_gl3.h"

#define MAX_VERTEX_BUFFER  512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024


// ---- vars ----
int window_w;
int window_h;

ui_color red = { 255, 0, 0, 255 };

ui_glfw glfw = { 0 };
int width = 0, height = 0;
ui_context* ctx;
ui_colorf bg;

// cant have these two because the windoews cant be resized otherwise NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE
const nk_flags window_flags       = NK_WINDOW_BORDER | NK_WINDOW_TITLE;
const nk_flags window_min_flags   = NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_MINIMIZABLE;
const nk_flags window_float_flags = NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE;
const nk_flags top_bar_flags      = NK_WINDOW_BORDER;

ui_rect top_bar_win_rect;
ui_rect top_bar_win_ratio;
ui_rect prop_win_rect;
ui_rect prop_win_ratio;
ui_rect template_win_rect;
ui_rect template_win_ratio;
ui_rect struct_browser_win_rect;
ui_rect struct_browser_win_ratio;
ui_rect frameb_win_rect;
ui_rect debug_win_rect;
ui_rect light_hierarchy_win_rect;
ui_rect light_hierarchy_win_ratio;
ui_rect core_data_win_rect;
ui_rect operation_win_rect;
ui_rect entity_hierarchy_win_rect;
ui_rect entity_hierarchy_win_ratio;


void gui_init()
{ 
  // ---- nuklear ----
  ctx = nk_glfw3_init(&glfw, core_data->window, NK_GLFW3_INSTALL_CALLBACKS);
  /* Load Fonts: if none of these are loaded a default font will be used  */
  /* Load Cursor: if you uncomment cursor loading please hide the cursor */
  {struct nk_font_atlas* atlas;
    nk_glfw3_font_stash_begin(&glfw, &atlas);

    struct nk_font *droid = nk_font_atlas_add_from_file(atlas, ASSET_PATH"fonts/DroidSans.ttf", 20/*18*//*16*//*14*/, 0);

    nk_glfw3_font_stash_end(&glfw);
    /*nk_style_load_all_cursors(ctx, atlas->cursors);*/

    nk_style_set_font(ctx, &droid->handle);
  }

  app_data->fb_preview.type = FRAMEBUFFER_RGB;
  app_data->fb_preview.is_msaa = false;
  app_data->fb_preview.width  = 512;
  app_data->fb_preview.height = 512;
  app_data->fb_preview.size_divisor = 1;
  framebuffer_create(&app_data->fb_preview);

  // P_NK_COLOR(ctx->style.button.text_active);
  // P_NK_COLOR(ctx->style.button.text_hover); 
  // P_NK_COLOR(ctx->style.button.text_normal);
}

void gui_update()
{ 
  // get window size
  window_get_size(&window_w, &window_h);
  int w = window_w; int h = window_h;
  nk_glfw3_new_frame(&glfw);
  
  // windows ------------------------------------------------------------------------------------------
  
  // less height because the window bar on top and below
  top_bar_win_ratio.w = 1.0f;
  top_bar_win_ratio.h = 0.045f; // pixel size harcoded
  top_bar_win_ratio.x = 0.0f;;
  top_bar_win_ratio.y = 0.0f; 

  top_bar_win_rect = nk_rect(top_bar_win_ratio.x * w, top_bar_win_ratio.y * h, 
                             top_bar_win_ratio.w * w, 45);// top_bar_win_ratio.h * h);
  gui_top_bar_win(ctx, top_bar_win_rect, top_bar_flags);

  // if (!core_data_is_play())
  if (core_data_get_play_state() != PLAY_STATE_PLAY)
  {
    // less height because the window bar on top and below
    template_win_ratio.w = 1.0f - prop_win_ratio.w; 
    template_win_ratio.h = 0.2f; 
    template_win_ratio.x = 0.0f; 
    template_win_ratio.y = app_data->template_browser_minimized ? 1.0f : 0.8f;

    template_win_rect = nk_rect(template_win_ratio.x * w, template_win_ratio.y * h, 
                                template_win_ratio.w * w, template_win_ratio.h * h);
    gui_template_browser_win(ctx, template_win_rect, window_min_flags);
  }
  // less height because the window bar on top and below
  prop_win_ratio.w = 0.25f; 
  prop_win_ratio.h = 1.0f;  
  prop_win_ratio.x = 0.75f; 
  prop_win_ratio.y = 0.0f;  

  bool is_selected = app_data->selected_id >= 0 || 
                     app_data->selected_id <= ID_BUFFER_TERRAIN_0; // -2: terrain
  prop_win_ratio.w *= is_selected ? 1.0f : 0.0f;

  prop_win_rect = nk_rect(prop_win_ratio.x * w, prop_win_ratio.y * h + top_bar_win_rect.h,
                          prop_win_ratio.w * w, prop_win_ratio.h * h);
  gui_properties_win(ctx, prop_win_rect, window_flags, is_selected);
 
  // --- external ---
  

  // if (app_data->show_hierarchy_win)
  { 
    entity_hierarchy_win_ratio.h = 1.0f;
    entity_hierarchy_win_ratio.w = 0.1f;
    entity_hierarchy_win_ratio.x = 0.0f;
    entity_hierarchy_win_ratio.y = 0.0f;
    int h_correct = top_bar_win_rect.h + (template_win_rect.h * !app_data->template_browser_minimized) + (35 * app_data->template_browser_minimized);
    entity_hierarchy_win_rect = nk_rect((entity_hierarchy_win_ratio.x * w), 
                                      (entity_hierarchy_win_ratio.y * h) + top_bar_win_rect.h, 
                                      (entity_hierarchy_win_ratio.w * w), 
                                      (entity_hierarchy_win_ratio.h * h) - h_correct);
    gui_hierarchy_win(ctx, entity_hierarchy_win_rect, window_min_flags);
  }
  if (core_data_get_play_state() != PLAY_STATE_PLAY)
  { 
    struct_browser_win_ratio.h = 1.0f;
    struct_browser_win_ratio.w = 0.1f;
    struct_browser_win_ratio.x = 0.0f;
    struct_browser_win_ratio.y = 0.0f;
    int h_correct = top_bar_win_rect.h + (template_win_rect.h * !app_data->template_browser_minimized) + (35 * app_data->template_browser_minimized);
    struct_browser_win_rect = nk_rect((struct_browser_win_ratio.x * w) + ( (!app_data->hierarchy_win_minimized) * entity_hierarchy_win_rect.w), 
                                      (struct_browser_win_ratio.y * h) + top_bar_win_rect.h + (app_data->hierarchy_win_minimized * 35),   // +35: entity_hierarchy
                                      (struct_browser_win_ratio.w * w), 
                                      (struct_browser_win_ratio.h * h) - h_correct);
    gui_struct_browser_win(ctx, struct_browser_win_rect, window_min_flags); 
  }

  // --- optional ---
 
  if (app_data->show_light_hierarchy_win)
  { 
    // less height because the window bar on top and below
    light_hierarchy_win_ratio.w = 0.1f;
    light_hierarchy_win_ratio.h = 1.0f - template_win_ratio.h;
    light_hierarchy_win_ratio.x = 0.0f;
    light_hierarchy_win_ratio.y = 0.0f + top_bar_win_ratio.h; 

    light_hierarchy_win_rect = nk_rect(light_hierarchy_win_ratio.x * w, light_hierarchy_win_ratio.y * h, 
                                     light_hierarchy_win_ratio.w * w, light_hierarchy_win_ratio.h * h);
    gui_light_hierarchy_win(ctx, light_hierarchy_win_rect, window_float_flags); 
  }

  if (app_data->show_frameb_win)
  { gui_framebuffer_win(); }

   if (app_data->show_debug_win)
  { gui_debug_win(); } 

   if (app_data->show_core_data_win)
  {
    // less height because the window bar on top and below
    const f32 w_ratio = 400.0f  / 1920.0f;
    const f32 h_ratio = 1000.0f / 1020.0f;
    const f32 x_ratio = 0.0f    / 1920.0f;
    const f32 y_ratio = 10.0f   / 1020.0f + top_bar_win_ratio.h;

    core_data_win_rect = nk_rect(x_ratio * w, y_ratio * h, w_ratio * w, h_ratio * h);
    gui_core_data_win(ctx, core_data_win_rect, window_float_flags); 
  } 

  if (app_data->show_operation_win)
  {
    // less height because the window bar on top and below
    const f32 w_ratio = 400.0f  / 1920.0f;
    const f32 h_ratio = 1000.0f / 1020.0f;
    const f32 x_ratio = 0.0f    / 1920.0f;
    const f32 y_ratio = 10.0f   / 1020.0f + top_bar_win_ratio.h;

    operation_win_rect = nk_rect(x_ratio * w, y_ratio * h, w_ratio * w, h_ratio * h);
    gui_operations_win(ctx, operation_win_rect, window_float_flags); 
  } 
 
   // --------------------------------------------------------------------------------------------------

  nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);

  // mouse over gui -----------------------------------------------------------------------------------

  bool over_ui = false;
  over_ui = nk_input_is_mouse_hovering_rect(&ctx->input, top_bar_win_rect)          ? true : over_ui;
  over_ui = nk_input_is_mouse_hovering_rect(&ctx->input, prop_win_rect)             ? true : over_ui;
  over_ui = nk_input_is_mouse_hovering_rect(&ctx->input, template_win_rect)         ? true : over_ui;
  over_ui = nk_input_is_mouse_hovering_rect(&ctx->input, struct_browser_win_rect)   ? true : over_ui;
  over_ui = nk_input_is_mouse_hovering_rect(&ctx->input, entity_hierarchy_win_rect) ? true : over_ui; 
  
  if (app_data->show_light_hierarchy_win) { over_ui = nk_input_is_mouse_hovering_rect(&ctx->input, light_hierarchy_win_rect) ? true : over_ui; }
  if (app_data->show_frameb_win)          { over_ui = nk_input_is_mouse_hovering_rect(&ctx->input, frameb_win_rect)          ? true : over_ui; }
  if (app_data->show_debug_win)           { over_ui = nk_input_is_mouse_hovering_rect(&ctx->input, debug_win_rect)           ? true : over_ui; }
  if (app_data->show_core_data_win)       { over_ui = nk_input_is_mouse_hovering_rect(&ctx->input, core_data_win_rect)       ? true : over_ui; }

  // already hovering or hovewr check from top bar menu's
  over_ui = over_ui || app_data->top_bar_menu_hover;  
  app_data->top_bar_menu_hover = false;

  app_data->mouse_over_ui = over_ui;
}

void gui_cleanup()
{
  nk_clear(ctx);
}

void gui_framebuffer_win()
{
  int w, h;
  window_get_size(&w, &h);

  // less height because the window bar on top and below
  const f32 w_ratio = 400.0f  / 1920.0f;
  const f32 h_ratio = 1000.0f / 1020.0f;
  const f32 x_ratio = 0.0f    / 1920.0f;
  const f32 y_ratio = 10.0f   / 1020.0f + top_bar_win_ratio.h;

  frameb_win_rect = nk_rect(x_ratio * w, y_ratio * h, w_ratio * w, h_ratio * h);
  if (nk_begin(ctx, "framebuffers", frameb_win_rect, window_float_flags)) 
  {
    int w, h;
    window_get_size(&w, &h);
    float ratio = (float)h / (float)w;
    // const int size = 150; // 190; // 245;
    int size = frameb_win_rect.w - 60;
    
    nk_layout_row_static(ctx, 25, size, 1);
    nk_labelf(ctx, NK_LEFT, "deferred - color");
    nk_layout_row_static(ctx, size * ratio, size, 1);
    nk_image(ctx, nk_image_id(core_data->fb_deferred.buffer));
    nk_layout_row_static(ctx, 25, size, 1);
    nk_labelf(ctx, NK_LEFT, "deferred - material");
    nk_layout_row_static(ctx, size * ratio, size, 1);
    nk_image(ctx, nk_image_id(core_data->fb_deferred.buffer02));
    nk_layout_row_static(ctx, 25, size, 1);
    nk_labelf(ctx, NK_LEFT, "deferred - normal");
    nk_layout_row_static(ctx, size * ratio, size, 1);
    nk_image(ctx, nk_image_id(core_data->fb_deferred.buffer03));
    nk_layout_row_static(ctx, 25, size, 1);
    nk_labelf(ctx, NK_LEFT, "deferred - position");
    nk_layout_row_static(ctx, size * ratio, size, 1);
    nk_image(ctx, nk_image_id(core_data->fb_deferred.buffer04));
    nk_layout_row_static(ctx, 25, size, 1);
    nk_labelf(ctx, NK_LEFT, "shadow");
    nk_layout_row_static(ctx, size * ratio, size, 1);
    nk_image(ctx, nk_image_id(core_data->fb_shadow_pass.buffer));
    nk_layout_row_static(ctx, 25, size, 1);
    nk_labelf(ctx, NK_LEFT, "lighting");
    nk_layout_row_static(ctx, size * ratio, size, 1);
    nk_image(ctx, nk_image_id(core_data->fb_lighting.buffer));
    nk_layout_row_static(ctx, 25, size, 1);
    nk_labelf(ctx, NK_LEFT, "mouse_pick");
    nk_layout_row_static(ctx, size * ratio, size, 1);
    nk_image(ctx, nk_image_id(core_data->fb_mouse_pick.buffer));
    nk_layout_row_static(ctx, 25, size, 1);
    nk_labelf(ctx, NK_LEFT, "outline");
    nk_layout_row_static(ctx, size * ratio, size, 1);
    nk_image(ctx, nk_image_id(core_data->fb_outline.buffer));
  }
  nk_end(ctx);
}
void gui_debug_win()
{
  int w, h;
  window_get_size(&w, &h);

  // less height because the window bar on top and below
  const f32 w_ratio = 400.0f  / 1920.0f;
  const f32 h_ratio = 1000.0f / 1020.0f;
  const f32 x_ratio = 0.0f    / 1920.0f;
  const f32 y_ratio = 10.0f   / 1020.0f + top_bar_win_ratio.h;

  debug_win_rect = nk_rect(x_ratio * w, y_ratio * h, w_ratio * w, h_ratio * h);
  if (nk_begin(ctx, "debug", debug_win_rect, window_float_flags)) 
  {
    
    nk_layout_row_dynamic(ctx, 25, 1);
    nk_labelf(ctx, NK_TEXT_LEFT, "draw_calls_total:         %9u", core_data->draw_calls_total);
    nk_labelf(ctx, NK_TEXT_LEFT, "  draw_calls_shadow:      %6u", core_data->draw_calls_shadow);
    nk_labelf(ctx, NK_TEXT_LEFT, "  draw_calls_deferred:    %6u", core_data->draw_calls_deferred);
    nk_labelf(ctx, NK_TEXT_LEFT, "  draw_calls_screen_quad: %2u", core_data->draw_calls_screen_quad);
    nk_labelf(ctx, NK_TEXT_LEFT, "  draw_call for cubemap:    1");

    
    int idxs_len = 0;
    int** idxs = state_entity_get_template_idxs_arr(&idxs_len);
    for (u32 t = 0; t < idxs_len; ++t)
    {
      nk_labelf(ctx, NK_TEXT_LEFT, "template: %d", t);
      
      u32 len = arrlen(idxs[t]);
      for (u32 i = 0; i < len; ++i)
      {
        nk_labelf(ctx, NK_TEXT_LEFT, "  -> %d", idxs[t][i]);
      }
    }

    // nk_spacing(ctx, 1);

    #ifdef DEBUG_TIMER
    if (nk_tree_push(ctx, NK_TREE_TAB, "static timers", NK_MINIMIZED))
    {
      char buf[128];
      int len = 0;
      timer_t* timers = debug_timer_get_all_static(&len);

      nk_layout_row_dynamic(ctx, 25, 1);
      if (len > 0) // lable for first file name
      {
        sprintf(buf, "[%s]", timers[0].file);
        nk_label(ctx, buf, NK_TEXT_LEFT);
      }
      for (int i = 0; i < len; ++i)
      {
        // @UNCLEAR: im comparing pointers here and it works 
        //           presumably because __FILE__, __FUNCTION__ always points to the same string
        //           not sure if i should use strcmp() anyways, if its more stable yk
        if (i != 0 && (timers[i - 1].file != timers[i].file)) // print file name if it changed
        {
          sprintf(buf, "[%s]", timers[i].file);
          nk_label(ctx, buf, NK_TEXT_LEFT);
        }
        sprintf(buf, "-> | %.2fms | %s | line: %d", timers[i].time, timers[i].name, timers[i].line);
        nk_label(ctx, buf, NK_TEXT_LEFT);
      }
      sprintf(buf, "[frame] -> %.2fms", core_data->delta_t * 1000); // * 1000: concvert to ms
      nk_label(ctx, buf, NK_TEXT_LEFT);
      nk_tree_pop(ctx);
    }
    if (nk_tree_push(ctx, NK_TREE_TAB, "timers", NK_MINIMIZED))
    {
      char buf[128];
      int len = 0;
      timer_t* timers = debug_timer_get_all(&len);

      nk_layout_row_dynamic(ctx, 25, 1);
      if (len > 0) // lable for first file name
      {
        sprintf(buf, "[%s]", timers[0].file);
        nk_label(ctx, buf, NK_TEXT_LEFT);
      }
      for (int i = 0; i < len; ++i)
      {
        // @UNCLEAR: im comparing pointers here and it works 
        //           presumably because __FILE__, __FUNCTION__ always points to the same string
        //           not sure if i should use strcmp() anyways, if its more stable yk
        if (i != 0 && (timers[i - 1].file != timers[i].file)) // print file name if it changed
        {
          sprintf(buf, "[%s]", timers[i].file);
          nk_label(ctx, buf, NK_TEXT_LEFT);
        }
        sprintf(buf, "-> | %.2fms | %s | line: %d", timers[i].time, timers[i].name, timers[i].line);
        nk_label(ctx, buf, NK_TEXT_LEFT);
      }
      sprintf(buf, "[frame] -> %.2fms", core_data->delta_t * 1000); // * 1000: concvert to ms
      nk_label(ctx, buf, NK_TEXT_LEFT);
      nk_tree_pop(ctx);
    }
    #else
    nk_layout_row_dynamic(ctx, 25, 1);
    nk_label(ctx, "timers deactivated", NK_TEXT_LEFT);
    #endif
  }
  nk_end(ctx);
}


// -- gui elems --

void gui_color_selector(rgbf color)
{ 
  ui_colorf c = { color[0], color[1], color[2] };

  // complex color combobox
  if (nk_combo_begin_color(ctx, nk_rgb_cf(c), nk_vec2(200, 400)))
  {
    enum color_mode { COL_RGB, COL_HSV };
    static int col_mode = COL_RGB;

    nk_layout_row_dynamic(ctx, 120, 1);
    c = nk_color_picker(ctx, c, NK_RGB);

    nk_layout_row_dynamic(ctx, 25, 2);
    col_mode = nk_option_label(ctx, "RGB", col_mode == COL_RGB) ? COL_RGB : col_mode;
    col_mode = nk_option_label(ctx, "HSV", col_mode == COL_HSV) ? COL_HSV : col_mode;

    nk_layout_row_dynamic(ctx, 25, 1);
    if (col_mode == COL_RGB) {
      c.r = nk_propertyf(ctx, "#R:", 0, c.r, 1.0f, 0.01f, 0.005f);
      c.g = nk_propertyf(ctx, "#G:", 0, c.g, 1.0f, 0.01f, 0.005f);
      c.b = nk_propertyf(ctx, "#B:", 0, c.b, 1.0f, 0.01f, 0.005f);
    }
    else {
      float hsva[4];
      nk_colorf_hsva_fv(hsva, c);
      hsva[0] = nk_propertyf(ctx, "#H:", 0, hsva[0], 1.0f, 0.01f, 0.05f);
      hsva[1] = nk_propertyf(ctx, "#S:", 0, hsva[1], 1.0f, 0.01f, 0.05f);
      hsva[2] = nk_propertyf(ctx, "#V:", 0, hsva[2], 1.0f, 0.01f, 0.05f);
      c = nk_hsva_colorfv(hsva);
    }
    nk_combo_end(ctx);
    
    color[0] = c.r;
    color[1] = c.g;
    color[2] = c.b;

  }
}

