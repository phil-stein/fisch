#include "core/state/state.h"
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
#include "editor/operation.h"

#include "core/window.h"
#include "core/io/file_io.h"
#include "core/io/save_sys/save_sys.h"

// @TODO: factor the contents of tools window out into seperate functions inside each game
#include "turn_based/entity_table.h"
#include "turn_based/turn_based.h"


void gui_tools_win(ui_context* ctx, ui_rect win_rect, const u32 win_flags)
{
  const char* win_name = "tools";
 
  if ( nk_begin(ctx, win_name, win_rect, win_flags) ) 
  {
    nk_layout_row_dynamic( ctx, 20, 1 );

    static int x_len = 10;
    nk_property_int( ctx, "x: ", 1, &x_len, GRID_IDS_AXIS_MAX, 1, 0.1f );
    static int z_len = 10;
    nk_property_int( ctx, "z: ", 1, &z_len, GRID_IDS_AXIS_MAX, 1, 0.1f );

    static nk_bool center_grid = true;
    nk_checkbox_label(ctx, "center grid", &center_grid);

    if ( nk_button_label(ctx, "make grid") && (x_len * z_len) < GRID_IDS_MAX )
    {
      const f32 x_center_offs = ((f32)x_len * 2.0f * 0.5f);
      const f32 z_center_offs = ((f32)z_len * 2.0f * 0.5f);
      game_data->grid_ids_pos = 0;
      for (int x = 0; x < x_len; ++x)
      {
        for (int z = 0; z < z_len; ++z)
        {
          // game_data->grid_ids[game_data->grid_ids_pos++] = 
          state_entity_add_from_template( 
              center_grid ? 
                VEC3_XYZ( ((f32)x *2.0f) - x_center_offs, 0, ((f32)z *2.0f) - z_center_offs ) : 
                VEC3_XYZ( (f32)x *2.0f, 0, (f32)z *2.0f ), 
              VEC3(0), VEC3(1), 
              ENTITY_TEMPLATE_TILE, true );
        }
      }
    }
    // app_data->selected_id = id;
    
    // operation_t op = OPERATION_T_ENTITY_ADD(id);
    // operation_register(&op);

    if ( nk_button_label(ctx, "delete grid") )
    {
      for (int i = 0; i < game_data->grid_ids_pos; ++i)
      {
        state_entity_remove_id(game_data->grid_ids[i]);
      }
    }
    
    if ( nk_button_label(ctx, "delete all tiles") )
    {
      int len, dead_len;
      entity_t* arr = state_entity_get_arr(&len, &dead_len);
      for ( int i = 0; i < len; ++i )
      {
        if ( arr[i].template_idx == ENTITY_TEMPLATE_TILE )
        { state_entity_remove_id(arr[i].id); }
      }
    }

  }
  nk_end( ctx );

  // collapse / minimize by default  
  // static bool first_frame = true;
  // if (first_frame)
  if ( app_data->tools_win_set_minimized )
  {
    nk_window_collapse(ctx, win_name, NK_MINIMIZED);
    // first_frame = false;
    app_data->tools_win_set_minimized = false;
  }

  app_data->tools_win_minimized = nk_window_is_collapsed(ctx, win_name) ? 1 : 0;
}

