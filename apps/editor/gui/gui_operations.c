#include "editor/app.h"
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
#include "editor/operation.h"
#include "core/window.h"
#include "core/state/state.h"


// --- func-decls ---
void gui_operations_entity_translate(ui_context* ctx, operation_t* op);
void gui_operations_entity_add(ui_context* ctx, operation_t* op);
void gui_operations_entity_remove(ui_context* ctx, operation_t* op);
void gui_operations_entity_child_add(ui_context* ctx, operation_t* op);


void gui_operations_win(ui_context* ctx, ui_rect win_rect, const u32 win_flags)
{
  int w, h;
  window_get_size(&w, &h);

  u32 op_arr_len = 0;
  operation_t* op_arr = operation_get_arr(&op_arr_len);
  
  #define TREE_NAME_MAX 64
  char tree_name[TREE_NAME_MAX];

  if (nk_begin(ctx, "operations", win_rect, win_flags)) 
  {  
    nk_layout_row_dynamic(ctx, 20, 1);
    if ( nk_button_label( ctx, "close" ) )
    { app_data->show_operation_win = false; }
    nk_spacing( ctx, 1 );

    for (u32 i = 0; i < op_arr_len; ++i)
    {
      SPRINTF(TREE_NAME_MAX, tree_name, "%.2u | %s", i, operation_type_to_str(op_arr[i].type));

      if (nk_tree_push(ctx, NK_TREE_TAB, tree_name, NK_MAXIMIZED))
      {
        switch (op_arr[i].type)
        {
          case OP_ENTITY_MOVE:
          case OP_ENTITY_ROTATE:
          case OP_ENTITY_SCALE:
            gui_operations_entity_translate(ctx, &op_arr[i]);
            break;
          case OP_ENTITY_ADD:
            gui_operations_entity_add(ctx, &op_arr[i]);
            break;
          case OP_ENTITY_REMOVE:
            gui_operations_entity_remove(ctx, &op_arr[i]);
            break;
          case OP_ENTITY_CHILD_ADD:
            gui_operations_entity_child_add(ctx, &op_arr[i]);
            break;
          default:
            nk_labelf(ctx, NK_TEXT_LEFT, "unknown operation type");
            break;

        }
        nk_tree_pop(ctx);
      }
    }
  }
  nk_end(ctx);
}

void gui_operations_entity_translate(ui_context* ctx, operation_t* op)
{
  f32* vec = op->type == OP_ENTITY_MOVE   ? op->pos : 
             op->type == OP_ENTITY_ROTATE ? op->rot :
             op->type == OP_ENTITY_SCALE  ? op->scl :
             NULL;
  ASSERT(vec != NULL);
  char* name = op->type == OP_ENTITY_MOVE   ? "pos" : 
               op->type == OP_ENTITY_ROTATE ? "rot" :
               op->type == OP_ENTITY_SCALE  ? "scl" :
               NULL;
  ASSERT(name != NULL);
  
  vec3 diff;
  vec3_negate(op->pos, diff);

  bool error  = false;
  /* entity_t* e = */ state_entity_get_err(op->entity_id, &error);
  if (error)
  {
    nk_labelf(ctx, NK_TEXT_LEFT, "entity: %d, not found", op->entity_id);
    return;
  }

  nk_layout_row_dynamic(ctx, 20, 1);

  nk_labelf(ctx, NK_TEXT_LEFT, "entity_id: %d", op->entity_id);
  nk_labelf(ctx, NK_TEXT_LEFT, "%s | x: %.2f, y: %.2f, z: %.2f", name, vec[0], vec[1], vec[2]);
  nk_labelf(ctx, NK_TEXT_LEFT, "diff | x: %.2f, y: %.2f, z: %.2f", diff[0], diff[1], diff[2]);

}

void gui_operations_entity_add(ui_context* ctx, operation_t* op)
{
  nk_layout_row_dynamic(ctx, 20, 1);
  nk_labelf(ctx, NK_TEXT_LEFT, "entity_id: %d", op->entity_id);
}

void gui_operations_entity_remove(ui_context* ctx, operation_t* op)
{
  nk_layout_row_dynamic(ctx, 20, 1);

  nk_labelf(ctx, NK_TEXT_LEFT, "template_idx: %d", op->entity_template_idx);
  nk_labelf(ctx, NK_TEXT_LEFT, "pos | x: %.2f, y: %.2f, z: %.2f", op->pos[0], op->pos[1], op->pos[2]);
  nk_labelf(ctx, NK_TEXT_LEFT, "rot | x: %.2f, y: %.2f, z: %.2f", op->rot[0], op->rot[1], op->rot[2]);
  nk_labelf(ctx, NK_TEXT_LEFT, "scl | x: %.2f, y: %.2f, z: %.2f", op->scl[0], op->scl[1], op->scl[2]);

}

void gui_operations_entity_child_add(ui_context* ctx, operation_t* op)
{
  nk_layout_row_dynamic(ctx, 20, 1);
  nk_labelf(ctx, NK_TEXT_LEFT, "entity_id: %d",       op->entity_id);
  nk_labelf(ctx, NK_TEXT_LEFT, "child_id: %d",        op->child_id);
  nk_labelf(ctx, NK_TEXT_LEFT, "child_parent_id: %d", op->child_parent_id);
}
