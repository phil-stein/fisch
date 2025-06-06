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


void gui_tools_win(ui_context* ctx, ui_rect win_rect, const u32 win_flags)
{
  const char* win_name = "tools";

  if ( nk_begin(ctx, win_name, win_rect, win_flags) ) 
  {
    nk_layout_row_dynamic( ctx, 20, 1 );


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

