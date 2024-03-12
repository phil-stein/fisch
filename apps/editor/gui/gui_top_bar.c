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
#include "editor/app.h"
#include "core/core_data.h"
#include "core/window.h"
#include "core/io/save_sys/save_sys.h"

#include "tinyfiledialogs/tinyfiledialogs.h"

void gui_top_bar_win(ui_context* ctx, ui_rect win_rect, const u32 win_flags)
{
  int w, h;
  window_get_size(&w, &h);
  
  float padding_x = ctx->style.window.padding.x;
  float padding_y = ctx->style.window.padding.y;
  ctx->style.window.padding.x = 0.0f;
  ctx->style.window.padding.y = 0.0f;

  // // less height because the window bar on top and below
  // top_bar_win_ratio.w = 1.0f;
  // top_bar_win_ratio.h = 0.045f; // pixel size harcoded
  // top_bar_win_ratio.x = 0.0f;;
  // top_bar_win_ratio.y = 0.0f; 

  // win_rect = nk_rect(top_bar_win_ratio.x * w, top_bar_win_ratio.y * h, 
  //                              top_bar_win_ratio.w * w, 35);// top_bar_win_ratio.h * h);
  if (nk_begin(ctx, "top bar", win_rect, win_flags)) 
  {
    // -- menubar --
    nk_menubar_begin(ctx);
    {
      ui_rect bounds; // used for tracking mouse hover for 'app_data->top_bar_menu_hover'
      nk_layout_row_begin(ctx, NK_STATIC, 30, 8);
      nk_layout_row_push(ctx, 50);
      if (nk_menu_begin_label(ctx, "scene", NK_TEXT_LEFT, nk_vec2(135, 200)))
      {
        nk_layout_row_dynamic(ctx, 20, 1);
        
        bounds = nk_widget_bounds(ctx);
        if (nk_menu_item_label(ctx, "save", NK_TEXT_LEFT))
        {
          // save_sys_write_scene_to_file(SCENE_FILE_NAME); 
          save_sys_write_scene_to_current_file();

          #ifdef TERRAIN_ADDON
          // save_sys_write_terrain_to_file(TERRAIN_FILE_NAME); 
          save_sys_write_terrain_to_current_file();
          #endif // TERRAIN_ADDON
        }
        app_data->top_bar_menu_hover = nk_input_is_mouse_hovering_rect(&ctx->input, bounds) ? true : app_data->top_bar_menu_hover;
       
        if (nk_menu_item_label(ctx, "save as", NK_TEXT_LEFT))
        { 
          // P_INFO("'save as' not implemented yet\n"); 
          char* path = tinyfd_saveFileDialog(
              "Save Empty Scene",                       // name
              "C:\\Workspace\\C\\fisch\\_assets\\name", // default path and file, NULL, "" or "C:\\Workspace\\C\\fisch\\_assets\\name" 
              0,                                        // num of file-patterns 
              NULL,                                     // NULL or char const * lFilterPatterns[2]={"*.png","*.jpg"};
              NULL);                                    // NULL or "image files"
          if (path != NULL)
          { 
            P_INFO("path: %s\n", path);
            save_sys_write_scene_to_path(path);
          } else { P_ERR("path not valid in file-dialog for 'save as'\n"); }
        }
        if (nk_menu_item_label(ctx, "load", NK_TEXT_LEFT))
        { 
          // P_INFO("'import' not implemented yet\n"); 
          const char* load_file_fiter_patterns[]    = {"*.scene"};
          const int   load_file_filter_patterns_len = sizeof(load_file_fiter_patterns) / sizeof(load_file_fiter_patterns[0]);
          char* path = tinyfd_openFileDialog(
              "Load Scene",                             // name
              "C:\\Workspace\\C\\fisch\\_assets\\name", // default path and file, NULL, "" or "C:\\Workspace\\C\\fisch\\_assets\\name" 
              load_file_filter_patterns_len,            // num of file-patterns 
              load_file_fiter_patterns,                 // NULL or char const * lFilterPatterns[2]={"*.png","*.jpg"};
              NULL,                                     // NULL or "image files"
              false);                                   // allow multiple select, seperator in path is |

          if (path != NULL)
          {
            // P_INFO("path: %s\n", path);
            save_sys_load_scene_from_path(path);
          } else { P_ERR("path not valid in file-dialog for 'load'\n"); }
        }
        
        if (nk_menu_item_label(ctx, "save new", NK_TEXT_LEFT))
        {
          char* path = tinyfd_saveFileDialog(
              "Save Empty Scene",                               // name
              "C:\\Workspace\\C\\fisch\\_assets\\empty.scene",  // default path and file, NULL, "" or "C:\\Workspace\\C\\fisch\\_assets\\name" 
              0,                                                // num of file-patterns 
              NULL,                                             // NULL or char const * lFilterPatterns[2]={"*.png","*.jpg"};
              NULL);                                            // NULL or "image files"
          if (path != NULL)
          { 
            // P_INFO("path: %s\n", path);
            save_sys_write_empty_scene_to_path(path);
            // save_sys_write_empty_terrain_to_file(TERRAIN_FILE_NAME); 
          } else { P_ERR("path not valid in file-dialog for 'save new'\n"); }
        }
      
        #ifdef TERRAIN_ADDON
        if (nk_menu_item_label(ctx, "terrain save as", NK_TEXT_LEFT))
        { 
          // P_INFO("'save as' not implemented yet\n"); 
          char* path = tinyfd_saveFileDialog(
              "Save Empty Scene",                       // name
              "C:\\Workspace\\C\\fisch\\_assets\\name", // default path and file, NULL, "" or "C:\\Workspace\\C\\fisch\\_assets\\name" 
              0,                                        // num of file-patterns 
              NULL,                                     // NULL or char const * lFilterPatterns[2]={"*.png","*.jpg"};
              NULL);                                    // NULL or "image files"
          if (path != NULL)
          { 
            P_INFO("path: %s\n", path);
            save_sys_write_terrain_to_path(path);
          } else { P_ERR("path not valid in file-dialog for 'save as'\n"); }
        }
        #endif // TERRAIN_ADDON
        
        nk_menu_end(ctx);
      }
      nk_layout_row_push(ctx, 55);
      if (nk_menu_begin_label(ctx, "windows", NK_TEXT_LEFT, nk_vec2(110, 200)))
      {
        nk_layout_row_static(ctx, 20, 90, 1);
        
        // bounds = nk_widget_bounds(ctx);
        // if (nk_menu_item_label(ctx, "hierarchy", NK_TEXT_LEFT))
        // { app_data->show_hierarchy_win = !app_data->show_hierarchy_win; }
        // app_data->top_bar_menu_hover = nk_input_is_mouse_hovering_rect(&ctx->input, bounds) ? true : app_data->top_bar_menu_hover;
        
        bounds = nk_widget_bounds(ctx);
        if (nk_menu_item_label(ctx, "light hierarchy", NK_TEXT_LEFT))
        { app_data->show_light_hierarchy_win = !app_data->show_light_hierarchy_win; }
        app_data->top_bar_menu_hover = nk_input_is_mouse_hovering_rect(&ctx->input, bounds) ? true : app_data->top_bar_menu_hover;
        
        bounds = nk_widget_bounds(ctx);
        if (nk_menu_item_label(ctx, "framebuffers", NK_TEXT_LEFT))
        { app_data->show_frameb_win = !app_data->show_frameb_win; }
        app_data->top_bar_menu_hover = nk_input_is_mouse_hovering_rect(&ctx->input, bounds) ? true : app_data->top_bar_menu_hover;
        
        bounds = nk_widget_bounds(ctx);
        if (nk_menu_item_label(ctx, "debug", NK_TEXT_LEFT))
        { app_data->show_debug_win = !app_data->show_debug_win; }
        app_data->top_bar_menu_hover = nk_input_is_mouse_hovering_rect(&ctx->input, bounds) ? true : app_data->top_bar_menu_hover;
        
        bounds = nk_widget_bounds(ctx);
        if (nk_menu_item_label(ctx, "core_data", NK_TEXT_LEFT))
        { app_data->show_core_data_win = !app_data->show_core_data_win; }
        app_data->top_bar_menu_hover = nk_input_is_mouse_hovering_rect(&ctx->input, bounds) ? true : app_data->top_bar_menu_hover;

        bounds = nk_widget_bounds(ctx);
        if (nk_menu_item_label(ctx, "assetm", NK_TEXT_LEFT))
        { app_data->show_assetm_win = !app_data->show_assetm_win; }
        app_data->top_bar_menu_hover = nk_input_is_mouse_hovering_rect(&ctx->input, bounds) ? true : app_data->top_bar_menu_hover;

        bounds = nk_widget_bounds(ctx);
        if (nk_menu_item_label(ctx, "operations", NK_TEXT_LEFT))
        { app_data->show_operation_win = !app_data->show_operation_win; }
        app_data->top_bar_menu_hover = nk_input_is_mouse_hovering_rect(&ctx->input, bounds) ? true : app_data->top_bar_menu_hover;
        
        nk_menu_end(ctx);
      }
      nk_layout_row_push(ctx, 55);
      if (nk_menu_begin_label(ctx, "control", NK_TEXT_LEFT, nk_vec2(110, 200)))
      {
        nk_layout_row_static(ctx, 20, 90, 1);
        
        bounds = nk_widget_bounds(ctx);
        if (nk_menu_item_label(ctx, "play", NK_TEXT_LEFT))
        { core_data_play();  }// { core_data->phys_act = true; core_data->scripts_act = true; }
        app_data->top_bar_menu_hover = nk_input_is_mouse_hovering_rect(&ctx->input, bounds) ? true : app_data->top_bar_menu_hover;
       
        bounds = nk_widget_bounds(ctx);
        if (nk_menu_item_label(ctx, "pause", NK_TEXT_LEFT))
        { core_data_pause(); } // { core_data->phys_act = false; core_data->scripts_act = false; }
        app_data->top_bar_menu_hover = nk_input_is_mouse_hovering_rect(&ctx->input, bounds) ? true : app_data->top_bar_menu_hover;
        
        bounds = nk_widget_bounds(ctx);
        if (nk_menu_item_label(ctx, "stop", NK_TEXT_LEFT))
        { core_data_stop(); } // { core_data->phys_act = false; core_data->scripts_act = false; }
        app_data->top_bar_menu_hover = nk_input_is_mouse_hovering_rect(&ctx->input, bounds) ? true : app_data->top_bar_menu_hover;
        
        bounds = nk_widget_bounds(ctx);
        if (nk_menu_item_label(ctx, "phys play", NK_TEXT_LEFT)) { core_data_play_phys(); } // { core_data->phys_act = true; }
        app_data->top_bar_menu_hover = nk_input_is_mouse_hovering_rect(&ctx->input, bounds) ? true : app_data->top_bar_menu_hover;
        
        // bounds = nk_widget_bounds(ctx);
        // if (nk_menu_item_label(ctx, "phys pause", NK_TEXT_LEFT))  { core_data->phys_act = false; }
        // app_data->top_bar_menu_hover = nk_input_is_mouse_hovering_rect(&ctx->input, bounds) ? true : app_data->top_bar_menu_hover;
        
        bounds = nk_widget_bounds(ctx);
        if (nk_menu_item_label(ctx, "script play", NK_TEXT_LEFT)) { core_data_play_scripts(); } // { core_data->scripts_act = true; }
        app_data->top_bar_menu_hover = nk_input_is_mouse_hovering_rect(&ctx->input, bounds) ? true : app_data->top_bar_menu_hover;
        
        // bounds = nk_widget_bounds(ctx);
        // if (nk_menu_item_label(ctx, "script pause", NK_TEXT_LEFT))  { core_data->scripts_act = false; }
        // app_data->top_bar_menu_hover = nk_input_is_mouse_hovering_rect(&ctx->input, bounds) ? true : app_data->top_bar_menu_hover;
        nk_menu_end(ctx);
      
      }
      
      // --- play/pause button ---
      // nk_layout_row_push(ctx, 30);
      // enum nk_symbol_type symbol = core_data_get_play_state() == PLAY_STATE_PLAY   ? NK_SYMBOL_RECT_SOLID :
      //                              core_data_get_play_state() == PLAY_STATE_PAUSED ? NK_SYMBOL_CIRCLE_SOLID :
      //                              NK_SYMBOL_TRIANGLE_RIGHT;
      // if (nk_button_symbol(ctx, symbol))
      if (core_data_get_play_state() == PLAY_STATE_PLAY)
      {
        nk_layout_row_push(ctx, 45);
        if (nk_button_label(ctx, "stop"))
        { 
          GUI_INFO_STR_SET(app_data, "stop");
          core_data_stop(); 
        }
        nk_layout_row_push(ctx, 55);
        if (nk_button_label(ctx, "pause"))
        { 
          GUI_INFO_STR_SET(app_data, "paused");
          core_data_pause(); 
        }
      }
      else if (core_data_get_play_state() == PLAY_STATE_PAUSED)
      {
        nk_layout_row_push(ctx, 45);
        if (nk_button_label(ctx, "play"))
        { 
          GUI_INFO_STR_SET(app_data, "play");
          core_data_play(); 
        }
        nk_layout_row_push(ctx, 45);
        if (nk_button_label(ctx, "stop"))
        { 
          GUI_INFO_STR_SET(app_data, "stop");
          core_data_stop(); 
        }
      }
      else // PLAY_STATE_STOPPED
      {
        nk_layout_row_push(ctx, 45);
        if (nk_button_label(ctx, "play"))
        { 
          GUI_INFO_STR_SET(app_data, "play");
          core_data_play(); 
        }
      }
      
      // spacing
      nk_layout_row_push(ctx, 25);
      nk_spacing(ctx, 1);
      
      // --- gizmo-space ---
      nk_layout_row_push(ctx, 155);
      if (nk_button_label(ctx, STR_GIZMO_SPACE_TYPE(app_data->gizmo_space)))
      {
        app_data->gizmo_space++;
        if (app_data->gizmo_space >= GIZMO_SPACE_MAX) { app_data->gizmo_space = 0; }
      }

      // spacing
      nk_layout_row_push(ctx, 25);
      nk_spacing(ctx, 1);
      
      // --- info txt ---
      if (app_data->gui_info_t >= 0.0f)
      {
        nk_layout_row_push(ctx, 250);
        nk_labelf(ctx, NK_TEXT_LEFT, " | %s", app_data->gui_info_str);
        app_data->gui_info_t -= core_data->delta_t;
      }
    }
    nk_menubar_end(ctx);
  }
  nk_end(ctx); 
  
  // reset state
  ctx->style.window.padding.x = padding_x;
  ctx->style.window.padding.y = padding_y;
}
