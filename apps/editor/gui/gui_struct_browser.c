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

#include "core/io/file_io.h"
#include "core/io/save_sys/save_sys.h"


static ui_context* ctx      = NULL;

void gui_struct_browser_display_struct(const char* path, const char* name)
{ 
  (void)path;
  ASSERT(ctx != NULL);
  
  char name_no_ext[64];
  strcpy(name_no_ext, name);
  name_no_ext[strlen(name) - strlen(".struct")] = '\0'; // castrate extension
  if(nk_button_label(ctx, name_no_ext))
  {
    int id = save_sys_load_structure_from_file(name_no_ext);
    app_data->selected_id = id;
  }
}

void gui_struct_browser_win(ui_context* _ctx, ui_rect win_rect, const u32 win_flags)
{
  ctx = _ctx;
 
  const char* win_name = "struct browser";
 
  if (nk_begin(ctx, win_name, win_rect, win_flags)) 
  {
    nk_layout_row_dynamic(ctx, 20, 1);

    file_io_search_dir_for_file_type(ASSET_PATH"structures", ".struct", gui_struct_browser_display_struct);

  }
  nk_end(ctx);

  // collapse / minimize by default  
  // static bool first_frame = true;
  // if (first_frame)
  if (app_data->struct_browser_set_minimized)
  {
    nk_window_collapse(ctx, win_name, NK_MINIMIZED);
    // first_frame = false;
    app_data->struct_browser_set_minimized = false;
  }

  app_data->struct_browser_minimized = nk_window_is_collapsed(ctx, win_name) ? 1 : 0;
}

