#ifndef EDITOR_APP_H
#define EDITOR_APP_H

#include "global/global.h"
#include "core/io/input.h"
#include "core/types/types.h"

// @DOC: the different gizmo types / modes
typedef enum { GIZMO_NONE, GIZMO_TRANSLATE, GIZMO_SCALE, GIZMO_ROTATE } gizmo_type;
typedef enum { GIZMO_SPACE_LOCAL, GIZMO_SPACE_GLOBAL, GIZMO_SPACE_MAX } gizmo_space_type;
#define STR_GIZMO_SPACE_TYPE(_gs) ((_gs) == GIZMO_SPACE_LOCAL  ? "gizmo-local"                        :  \
                                   (_gs) == GIZMO_SPACE_GLOBAL ? "gizmo-global"                       :  \
                                   (_gs) == GIZMO_SPACE_MAX    ? "GIZMO_SPACE_MAX, not a valid space" :  \
                                   "unknown")

// @DOC: the different types / modes for the terrain edit tool
typedef enum terrain_edit_type_t 
{ 
  TERRAIN_EDIT_NONE, 
  TERRAIN_EDIT_SCULPT, 
  TERRAIN_EDIT_LEVEL, 
  TERRAIN_EDIT_SMOOTH, 
  TERRAIN_EDIT_PAINT 

} terrain_edit_type_t;


#define GUI_INFO_STR_MAX 64

// @DOC: houses all publicly accessible data for app (editor)
//       instance of app_data_t is in app.c and accesible via app_data_get()
typedef struct app_data_t
{
  int editor_save_version;

  int selected_id;                        // id of current selected entity, -1 is none 
  
  gizmo_type       gizmo_type;            // current active gizmo type / mode
  gizmo_space_type gizmo_space;           // current space in which gizmos manipulate
  bool switch_gizmos_act;                 // if false hotkeys wont switch gizmo
  f32  gizmo_translate_speed;             // speed at which GIZMO_TRANSLATE moves ents
  f32  gizmo_rotate_speed;                // speed at which GIZMO_ROTATE rotates ents
  f32  gizmo_scale_speed;                 // speed at which GIZMO_SCALE scales ents
  bool gizmo_snapping;                    // if true gizmo movec, rotates, scales in set intervals
  f32  gizmo_translate_snap;              // interval at which entities are moved if gizmo_snapping is true
  f32  gizmo_rotate_snap;                 // interval at which entities are rotatet if gizmo_snapping is true
  f32  gizmo_scale_snap;                  // interval at which entities are scaled if gizmo_snapping is true

  char gui_info_str[GUI_INFO_STR_MAX];    // info text in top bar
  f32  gui_info_t;                        // time remaining for info text in top bar to be displayed

  f32  mouse_sensitivity;                 // speed at which the mouse moves
  bool mouse_over_ui;                     // if true mouse is currently over a ui window

  terrain_edit_type_t terrain_edit_type;  // current active terrain edit tool type / mode 
  f32  terrain_edit_radius;               // radius in which the terrain edit tool has effect
  f32  terrain_edit_strength;             // effect the current terrain edit tool has
  const f32  terrain_edit_scalar;         // multiplier for terrain_edit_strengh
  const f32  terrain_edit_smooth_scalar;  // multiplier for terrain_edit_strengh
  int terrain_edit_paint_material;        // multiplier for terrain_edit_strengh 
  const f32  terrain_edit_paint_scalar;   // multiplier for terrain_edit_strengh 

  // --- gui ---

  bool top_bar_menu_hover;          // mouse over menu in top bar
  bool template_browser_minimized; 
  bool template_browser_set_minimized; 
  bool hierarchy_win_minimized; 
  bool hierarchy_win_set_minimized; 
  bool struct_browser_minimized;
  bool struct_browser_set_minimized;
  
  // bool show_hierarchy_win;
  bool show_frameb_win;
  bool show_debug_win;
  bool show_light_hierarchy_win;
  bool show_core_data_win;
  bool show_assetm_win;
  bool show_operation_win; 

  framebuffer_t fb_preview;         // for redering template browser previews

    
}app_data_t;

#define APP_DATA_INIT()                                \
{                                                      \
  .editor_save_version          = 0,                   \
                                                       \
  .selected_id                  = -1,                  \
                                                       \
  .gizmo_type                   = GIZMO_NONE,          \
  .gizmo_space                  = GIZMO_SPACE_LOCAL,   \
  .switch_gizmos_act            = true,                \
  .gizmo_translate_speed        = 1.0f,                \
  .gizmo_rotate_speed           = 20.0f,               \
  .gizmo_scale_speed            = 1.0f,                \
  .gizmo_snapping               = false,               \
  .gizmo_translate_snap         = 1.0f,                \
  .gizmo_rotate_snap            = 15.0f,               \
  .gizmo_scale_snap             = 0.5f,                \
                                                       \
  .gui_info_str                 = "",                  \
  .gui_info_t                   = 0.0f,                \
                                                       \
  .mouse_sensitivity            = 0.125f,              \
  .mouse_over_ui                = false,               \
                                                       \
  .terrain_edit_type            = TERRAIN_EDIT_NONE,   \
  .terrain_edit_radius          = 5.0f,                \
  .terrain_edit_scalar          = 0.01f,               \
  .terrain_edit_smooth_scalar   = 0.5f,                \
  .terrain_edit_strength        = 1.0,                 \
  .terrain_edit_paint_material  = 0,                   \
  .terrain_edit_paint_scalar    = 0.75f,               \
                                                       \
  .top_bar_menu_hover             = false,             \
  .template_browser_minimized     = true,              \
  .template_browser_set_minimized = true,              \
  .hierarchy_win_minimized        = true,              \
  .hierarchy_win_set_minimized    = true,              \
  .struct_browser_minimized       = true,              \
  .struct_browser_set_minimized   = true,              \
                                                       \
  .show_frameb_win            = false,                 \
  .show_debug_win             = false,                 \
  .show_light_hierarchy_win   = false,                 \
  .show_core_data_win         = false,                 \
  .show_assetm_win            = false,                 \
  .show_operation_win         = false,                 \
}
  // .wireframe_act                = false,               \x

// @DOC: extern pointer to app_data_t in editor/app.c
//       available everywhere that includes "editor/app.h"
extern app_data_t* app_data;

#define GUI_INFO_DEFAULT_T  2.0f
#define GUI_INFO_STR_SET_T(_app_data, t, ...)   SPRINTF(GUI_INFO_STR_MAX, (_app_data)->gui_info_str, __VA_ARGS__); \
                                                (_app_data)->gui_info_t = t
#define GUI_INFO_STR_SET(_app_data, ...)        GUI_INFO_STR_SET_T(_app_data, GUI_INFO_DEFAULT_T, __VA_ARGS__)

// -- keymappings --
// some are in program.h

#define KEY_GIZMO_TRANSLATE   KEY_G
#define KEY_GIZMO_SCALE       KEY_S
#define KEY_GIZMO_ROTATE      KEY_R
#define KEY_GIZMO_SNAPPING    KEY_LEFT_CONTROL


#define CAM_SPEED             5
#define CAM_SPEED_SHIFT_MULT  5
#define KEY_MOUSE_MOVE_START  MOUSE_RIGHT
#define KEY_MOVE_FORWARD      KEY_W 
#define KEY_MOVE_BACKWARD     KEY_S
#define KEY_MOVE_LEFT         KEY_A
#define KEY_MOVE_RIGHT        KEY_D 
#define KEY_MOVE_UP           KEY_E 
#define KEY_MOVE_DOWN         KEY_Q 


// @NOTE: moved to core_data->scene_name
// // @TODO: @BUGG: this shouldnt be in app this should be in save_sys
// #define SCENE_FILE_NAME   "test.scene"
// #define TERRAIN_FILE_NAME "test.terrain"

// @DOC: initalize, call before any other calls to app
void app_init();
// @DOC: upate logic called once a frame
void app_update();
// @DOC: cleanup memory, before exiting
void app_cleanup();

void app_entity_removed_callback(int id);


#endif
