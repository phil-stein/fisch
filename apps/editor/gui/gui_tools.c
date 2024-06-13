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
#include "turn_based/entity_table.h"

#include "core/window.h"
#include "core/io/file_io.h"
#include "core/io/save_sys/save_sys.h"

#define GRID_IDS_MAX (128 * 128)
int grid_ids[GRID_IDS_MAX] = { 0 };
int grid_ids_pos = 0;


void gui_tools_win(ui_context* ctx, ui_rect win_rect, const u32 win_flags)
{
  const char* win_name = "tools";
 
  if ( nk_begin(ctx, win_name, win_rect, win_flags) ) 
  {
    nk_layout_row_dynamic( ctx, 20, 1 );

    static int x_len = 0;
    nk_property_int( ctx, "x: ", 1, &x_len, 1000, 1, 0.1f );
    static int z_len = 0;
    nk_property_int( ctx, "z: ", 1, &z_len, 1000, 1, 0.1f );

    if ( nk_button_label(ctx, "make grid") && (x_len * z_len) < GRID_IDS_MAX )
    {
      grid_ids_pos = 0;
      for (int x = 0; x < x_len; ++x)
      {
        for (int z = 0; z < z_len; ++z)
        {
          grid_ids[grid_ids_pos++] = 
          state_entity_add_from_template( VEC3_XYZ((f32)x *2.0f, 0, (f32)z *2.0f), VEC3(0), VEC3(1), 
                                          ENTITY_TEMPLATE_TILE, true );
        }
      }
    }
    // app_data->selected_id = id;
    
    // operation_t op = OPERATION_T_ENTITY_ADD(id);
    // operation_register(&op);

    if ( nk_button_label(ctx, "clear grid") )
    {
      for (int i = 0; i < grid_ids_pos; ++i)
      {
        state_entity_remove_id(grid_ids[i]);
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

