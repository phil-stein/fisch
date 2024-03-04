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


void gui_hierarchy_win(ui_context* ctx, ui_rect win_rect, const u32 win_flags)
{
  // int w, h;
  // window_get_size(&w, &h);
  // // less height because the window bar on top and below
  // hierarchy_win_ratio.w = 0.1f;
  // hierarchy_win_ratio.h = 1.0f - template_win_ratio.h;
  // hierarchy_win_ratio.x = 0.0f;
  // hierarchy_win_ratio.y = 0.0f + top_bar_win_ratio.h; 
  // hierarchy_win_rect = nk_rect(hierarchy_win_ratio.x * w, hierarchy_win_ratio.y * h, 
  //                              hierarchy_win_ratio.w * w, hierarchy_win_ratio.h * h);
  // if (nk_begin(ctx, "entity hierarchy", hierarchy_win_rect, window_float_flags)) 

  app_data->hierarchy_win_minimized = nk_window_is_collapsed(ctx, "entity hierarchy") ? 1 : 0;

  if (nk_begin(ctx, "entity hierarchy", win_rect, win_flags)) 
  {
    nk_layout_row_dynamic(ctx, 30, 1);
    int e_len = 0;
    int e_dead_len = 0;
    entity_t* e = state_entity_get_arr(&e_len, &e_dead_len);
    // int selected_id = app_get_selected_id();
    for (int i = 0; i < e_len; ++i)
    {
      int offs = 0;
      if (e[i].parent <= -1)
      { gui_hierarchy_display_entity_and_children(ctx, &e[i], &offs, &win_rect); }
    }
  }
  nk_end(ctx); 
  
  // collapse / minimize by default  
  // static bool first_frame = true;
  // if (first_frame)
  if (app_data->hierarchy_win_set_minimized)
  {
    nk_window_collapse(ctx, "entity hierarchy", NK_MINIMIZED);
    // first_frame = false;
    app_data->hierarchy_win_set_minimized = false;
  }
}
void gui_hierarchy_display_entity_and_children(ui_context* ctx, entity_t* e, int* offs, ui_rect* win_rect)
{
  // bool err = false;
  // entity_t* e = state_entity_get(id, &err);
  // assert(err);
  if (!e->is_dead)
  {
    nk_layout_row_begin(ctx, NK_STATIC, 30, *offs == 0 ? 1 : 2);
    
    const float offs_step = 10.0f;
    if (*offs > 0)
    { nk_layout_row_push(ctx, *offs * offs_step); nk_spacing(ctx, 1); } 
    
    nk_layout_row_push(ctx, win_rect->w - (*offs * offs_step));
    char buf[32];
    sprintf(buf, "%d", e->id);
    nk_bool selec = e->id == app_data->selected_id; 
    nk_bool selec_old = selec;
    nk_selectable_label(ctx, buf, NK_TEXT_LEFT, &selec);
    if (!selec_old && selec)
    { app_data->selected_id = e->id; }

    *offs += 1;
    // PF("offs: %d, id: %d\n", *offs, e->id);
    for (int i = 0; i < e->children_len; ++i)
    {
      ERR_CHECK(e->id != e->children[i], "id: %d, children_len: %d, child[%d]: %d\n", e->id, e->children_len, i, e->children[i]);
      entity_t* c = state_entity_get(e->children[i]);
      gui_hierarchy_display_entity_and_children(ctx, c, offs, win_rect); 
    }
  }   
}

