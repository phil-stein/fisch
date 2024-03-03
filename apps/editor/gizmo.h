#ifndef GIZMO_H
#define GIZMO_H

#include "global/global.h"
#include "editor/app.h"
#include "core/types/types.h"
#include "core/state/state.h"
#include "math/math_inc.h"
#include "math/math_vec3.h"

// @DOC: get the model matrix, and position for the gizmo
//       used in gizmo.c for drawing the gizmo
//       used in renderer.c for drawing the mouse-pick hitbox
//       model is for calc, display_model for rendering
INLINE void gizmo_get_model_pos(mat4 model, mat4 display_model, vec3 pos)
{
  if ((app_data)->selected_id >= 0)
  {
    entity_t* e = state_entity_get((app_data)->selected_id);
    switch (app_data->gizmo_space)
    {
      case GIZMO_SPACE_GLOBAL:
        mat4_get_pos(e->model, pos);
        mat4_make_model(pos, VEC3(0), VEC3(1), display_model);
        // state_entity_get_global_model_no_rotation(app_data->selected_id, display_model);
        mat4_get_pos(display_model, pos);
        mat4_copy(display_model, model);
        return;
      case GIZMO_SPACE_LOCAL:     // @TODO: this doesnt account for parent influence on rot
        mat4_get_pos(e->model, pos);
        // mat4_make_model(pos, e->rot, VEC3(1), display_model);
        mat4_copy(e->model, display_model);
        mat4_set_scale_vec3(VEC3(1), display_model);
        mat4_copy(display_model, model);
        return;

      default:
        ERR("app_data->gizmo_space is %s, which isnt handled by %s()\n", 
            STR_GIZMO_SPACE_TYPE(app_data->gizmo_space), __func__);
        break;
    }
  }
  else
  { 
    mat4_make_identity(model); 
    mat4_make_identity(display_model); 
    vec3_copy(VEC3(0), pos); 
    // P_ERR("gizmo_get_model_pos() called, but app_data->selected_id is invalid: %d\n", app_data->selected_id);
  }
}
// #define GIZMO_MODEL_POS(_app_data, model, display_model, pos)                                 \x
//     if ((_app_data)->selected_id >= 0)                                                        \x
//     {                                                                                         \x
//       entity_t* e = state_entity_get((_app_data)->selected_id);                               \x
//       mat4_get_pos(e->model, (pos));                                                          \x
//       mat4_make_model((pos), VEC3(0), VEC3(1), display_model);                                \x
//       mat4_copy(display_model, model);                                                        \x
//     }                                                                                         \x
//     else                                                                                      \x
//     { mat4_make_identity(model); mat4_make_identity(display_model); vec3_copy(VEC3(0), pos); }
// 
// // @NOTE: old, not in use
// #define GIZMO_MODEL_POS_OLD(_app_data, model, display_model, pos)                             \x
//     if ((_app_data)->selected_id >= 0)                                                        \x
//     {                                                                                         \x
//       entity_t* e = state_entity_get((_app_data)->selected_id);                               \x
//       mat4_get_pos(e->model, (pos));                                                          \x
//       if ((_app_data)->gizmo_type == GIZMO_TRANSLATE)                                         \x
//       { state_entity_model_no_scale_rotation((_app_data)->selected_id, (model)); }            \x
//       if ((_app_data)->gizmo_type == GIZMO_ROTATE)                                            \x
//       { state_entity_model_no_scale((_app_data)->selected_id, (model)); }                     \x
//       if ((_app_data)->gizmo_type == GIZMO_SCALE)                                             \x
//       { state_entity_model_no_scale((_app_data)->selected_id, (model)); }                     \x
//       mat4_make_model((pos), VEC3(0), VEC3(1), display_model);                                \x
//       mat4_copy(display_model, model);                                                        \x
//     }                                                                                         \x
//     else                                                                                      \x
//     { mat4_make_identity(model); mat4_make_identity(display_model); vec3_copy(VEC3(0), pos); }                                
// 
// @DOC: update the gizmo logic, call once a frame
void gizmo_update();

// @DOC: ends current operation, sets up for next operation
//       also commits an operation to operation.c
void gizmo_end_operation();

// @DOC: calcuate 3d distance between 2 screen-space points projected into and entity's model space
//       p0 & p1: screen-space, in pixel, coordinates
//       pos:     position of gizmo
//       model:   model-space used
//       out:     distance between the projected points
void gizmo_calc_dist_screen_to_model(vec2 p0, vec2 p1, vec3 pos, mat4 model, vec3 out);

#endif
