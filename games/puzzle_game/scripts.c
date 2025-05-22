#include "puzzle_game/scripts.h"
#include "core/debug/debug_draw.h"
#include "core/templates/entity_template.h"
#include "editor/gui/gui.h"
#include "core/io/file_io.h"
#include "global/global_print.h"
#include "puzzle_game/entity_tags.h"
#include "puzzle_game/entity_table.h"

#include "serialization/serialization.h"

// includes bool SCRIPT_REMOVE_FUNC_GENERIC_NAME(u32 uid);
#include "games.h" // needed to include __script_update__(), etc. 

#include "stb/stb_ds.h"
#include "puzzle_game/puzzle_game.h"


#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_KEYSTATE_BASED_INPUT
#include "nuklear/nuklear.h"

// -- register --
SCRIPT_REGISTER(power_lever_script_t, POWER_LEVER_SCRIPT_T_INIT)
SCRIPT_REGISTER(fps_controller_script_t,  FPS_CONTROLLER_SCRIPT_T_INIT)

#if defined(_MSC_VER) & !defined(__clang__)
// -- clear arrays --
SCRIPTS_CLEAR_FUNC_START()
  SCRIPTS_CLEAR_FUNC_SCRIPT(power_lever_script_t);
  SCRIPTS_CLEAR_FUNC_SCRIPT(fps_controller_script_t);
SCRIPTS_CLEAR_FUNC_END()
// -- generic remove func --
SCRIPT_REMOVE_FUNC_GENERIC_START()
  SCRIPT_REMOVE_FUNC_GENERIC_SCRIPT(power_lever_script_t);
  SCRIPT_REMOVE_FUNC_GENERIC_SCRIPT(fps_controller_script_t);
SCRIPT_REMOVE_FUNC_GENERIC_END()
// -- generic get func --
SCRIPT_GET_FUNC_GENERIC_START()
  SCRIPT_GET_FUNC_GENERIC_SCRIPT(power_lever_script_t);
  SCRIPT_GET_FUNC_GENERIC_SCRIPT(fps_controller_script_t);
SCRIPT_GET_FUNC_GENERIC_END()
// -- get type str func --
SCRIPT_GET_TYPE_STR_FUNC_START()
  SCRIPT_GET_TYPE_STR_FUNC_SCRIPT(power_lever_script_t);
  SCRIPT_GET_TYPE_STR_FUNC_SCRIPT(fps_controller_script_t);
SCRIPT_GET_TYPE_STR_FUNC_END()
INLINE void SCRIPT_RUN_UPDATE_ALL()                           
{ 
  SCRIPT_RUN_UPDATE(power_lever_script_t);
  SCRIPT_RUN_UPDATE(fps_controller_script_t);
}                 
                                                              
INLINE void SCRIPT_RUN_CLEANUP_ALL()                          
{
  SCRIPT_RUN_CLEANUP(power_lever_script_t);
  SCRIPT_RUN_CLEANUP(fps_controller_script_t);
} 
SCRIPT_GET_ALL_FUNC(power_lever_script_t);
SCRIPT_GET_ALL_FUNC(fps_controller_script_t);

#else // _MSC_VER && !__clang__
SCRIPT_FUNCS(
  power_lever_script_t,
  fps_controller_script_t,
  )
#endif // _MSC_VER && !__clang__


// @NOTE: SCRIPT_INIT(...) called in SCRIPT_ADD(...) | what???

void __scripts_init__() {}

void __scripts_update__()
{
  if (!game_data->run_scripts) { return; }

  // // P_INT(player_controller_script_t_arr_len);
  // SCRIPT_RUN_UPDATE(projectile_script_t);
  // SCRIPT_RUN_UPDATE(player_controller_script_t);
  // SCRIPT_RUN_UPDATE(fps_controller_script_t);
  // SCRIPT_RUN_UPDATE(enemy_behaviour_script_t);
  SCRIPT_RUN_UPDATE_ALL();
}

void __scripts_cleanup__()
{
  scripts_write_save_data();

  // SCRIPT_RUN_CLEANUP(projectile_script_t);
  // SCRIPT_RUN_CLEANUP(player_controller_script_t);
  // SCRIPT_RUN_CLEANUP(fps_controller_script_t);
  // SCRIPT_RUN_CLEANUP(enemy_behaviour_script_t);
  SCRIPT_RUN_CLEANUP_ALL();
}

void scripts_write_save_data()
{
  u32 lever_scripts_len = 0;
  power_lever_script_t* lever_scripts = SCRIPT_GET_ALL(power_lever_script_t, &lever_scripts_len);
  (void)lever_scripts;
  P_V(lever_scripts_len);

  u8* buffer = NULL;

  serialization_serialize_u32(&buffer, lever_scripts_len);
  for (int i = 0; i < (int)lever_scripts_len; ++i )
  {
    serialization_serialize_s32(&buffer, lever_scripts[i].interactable_id);  
    serialization_serialize_f32(&buffer, lever_scripts[i].action_max_t);  
    
    serialization_serialize_vec3(&buffer, lever_scripts[i].start);  
    serialization_serialize_vec3(&buffer, lever_scripts[i].end);  
  }

  char path[ASSET_PATH_MAX +64];
  SPRINTF(ASSET_PATH_MAX + 64, path, "%s%s", core_data->asset_path, "power_lever.save_data");
  file_io_write(path, (const char*)buffer, (int)arrlen(buffer));
}
void scripts_load_save_data()
{
  u32 lever_scripts_len = 0;
  power_lever_script_t* lever_scripts = SCRIPT_GET_ALL(power_lever_script_t, &lever_scripts_len);

  u32 offset = 0;
  int length = 0;

  char path[ASSET_PATH_MAX +64];
  SPRINTF(ASSET_PATH_MAX +64, path, "%s%s", core_data->asset_path, "power_lever.save_data");
  u8* buffer = (u8*)file_io_read_bytes(path, &length);

  u32 count = serialization_deserialize_u32(buffer, &offset);
  if (count != lever_scripts_len)
  {
    P_V(count);
    P_V(lever_scripts_len);
    ERR("saved amount of lever_scripts and current one not the same");
  }
  for (int i = 0; i < (int)count; ++i)
  {
    lever_scripts[i].interactable_id = serialization_deserialize_s32(buffer, &offset);  
    lever_scripts[i].action_max_t    = serialization_deserialize_f32(buffer, &offset);  
    
    serialization_deserialize_vec3(buffer, &offset, lever_scripts[i].start);  
    serialization_deserialize_vec3(buffer, &offset, lever_scripts[i].end);  
  }

}

void editor_entity_ui_callback( ui_context* ctx, entity_t* e )
{
  if ( e->template_idx != ENTITY_TEMPLATE_POWER_LEVER_02 ) { return; }

  power_lever_script_t* script = SCRIPT_ENTITY_GET( power_lever_script_t, e );

  if (script != NULL && nk_tree_push(ctx, NK_TREE_TAB, "power_lever_script_t", NK_MINIMIZED))
  {
    nk_labelf( ctx, NK_LEFT, "activated: %s", script->activated ? "true" : "false" );
    nk_bool act = (nk_bool)script->activated;
    nk_checkbox_label( ctx, "activated", &act );
    if ( act != script->activated ) { power_lever_script_t_set_activated( script, (bool)act ); }

    nk_property_int(ctx, "interactable_id", 0, &script->interactable_id, 2048, 0, 0.01f);
    nk_property_float(ctx, "action_max_t", -2048.0f, &script->action_max_t, 2048.0f, 0.1f, 0.01f);

    nk_layout_row_dynamic(ctx, 30, 3);
    nk_property_float(ctx, "start.x", -2048.0f, &script->start[0], 2048.0f, 0.1f, 0.01f);
    nk_property_float(ctx, "start.y", -2048.0f, &script->start[1], 2048.0f, 0.1f, 0.01f);
    nk_property_float(ctx, "start.z", -2048.0f, &script->start[2], 2048.0f, 0.1f, 0.01f);

    nk_property_float(ctx, "end.x", -2048.0f, &script->end[0], 2048.0f, 0.1f, 0.01f);
    nk_property_float(ctx, "end.y", -2048.0f, &script->end[1], 2048.0f, 0.1f, 0.01f);
    nk_property_float(ctx, "end.z", -2048.0f, &script->end[2], 2048.0f, 0.1f, 0.01f);

    debug_draw_sphere( script->start, 0.5f, RGBF(0, 1, 0) );
    debug_draw_sphere( script->end, 0.5f, RGBF(1, 0, 0) );
    debug_draw_line( script->start, script->end, RGBF(1, 1, 1) );

    nk_tree_pop(ctx);
  }

}

