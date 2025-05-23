#include "puzzle_game/scripts.h"
#include "core/core_data.h"
#include "core/debug/debug_draw.h"
#include "core/state/state.h"
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


#ifdef EDITOR
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_KEYSTATE_BASED_INPUT
#include "nuklear/nuklear.h"
#endif // EDITOR

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


u8* power_lever_scripts_state_buffer = NULL;

// @NOTE: SCRIPT_INIT(_type) called in SCRIPT_ADD(_type) | what???
// @TODO: make scripts added in save_sys_load_scene() init from __scripts_init__()
//        and entities added afterwards get called in SCRIPT_ADD()
//        by making a second version of SCRIPT_ADD()
void __scripts_init__() {}

void __scripts_update__()
{
  if (!game_data->run_scripts) { return; }

  // // P_INT(player_controller_script_t_arr_len);
  // SCRIPT_RUN_UPDATE(projectile_script_t);
  // SCRIPT_RUN_UPDATE(player_controller_script_t);
  // SCRIPT_RUN_UPDATE(fps_controller_script_t) f;
  // SCRIPT_RUN_UPDATE(enemy_behaviour_script_t);
  SCRIPT_RUN_UPDATE_ALL();
}

void __scripts_cleanup__()
{
  if (core_data_get_play_state() == PLAY_STATE_STOPPED) { scripts_write_save_data(); }

  // SCRIPT_RUN_CLEANUP(projectile_script_t);
  // SCRIPT_RUN_CLEANUP(player_controller_script_t);
  // SCRIPT_RUN_CLEANUP(fps_controller_script_t);
  // SCRIPT_RUN_CLEANUP(enemy_behaviour_script_t);
  SCRIPT_RUN_CLEANUP_ALL();
}
#define LEVER_SCRIPT_SAVE_DATA_VERSION 1
void scripts_save_data_serialize(u8** buffer, power_lever_script_t* lever_scripts, u32 lever_scripts_len )
{
  serialization_serialize_s32(buffer, LEVER_SCRIPT_SAVE_DATA_VERSION);  

  serialization_serialize_u32(buffer, lever_scripts_len);
  for (int i = 0; i < (int)lever_scripts_len; ++i )
  {
    serialization_serialize_s32(buffer, lever_scripts[i].action_type);  

    serialization_serialize_s32(buffer, lever_scripts[i].interactable_id);  
    serialization_serialize_f32(buffer, lever_scripts[i].action_max_t);  
    
    serialization_serialize_vec3(buffer, lever_scripts[i].start);  
    serialization_serialize_vec3(buffer, lever_scripts[i].end);  
  }
}
void scripts_write_save_data()
{
  u32 lever_scripts_len = 0;
  power_lever_script_t* lever_scripts = SCRIPT_GET_ALL(power_lever_script_t, &lever_scripts_len);
  (void)lever_scripts;

  u8* buffer = NULL;

  scripts_save_data_serialize( &buffer, lever_scripts, lever_scripts_len );

  char path[ASSET_PATH_MAX +64];
  SPRINTF(ASSET_PATH_MAX + 64, path, "%s%s", core_data->asset_path, "power_lever.save_data");
  file_io_write(path, (const char*)buffer, (int)arrlen(buffer));

  P_INFO( "wrote power_lever.save_data to file\n" );

  ARRFREE(buffer);
}
void scripts_write_save_data_to_buffer()
{
  u32 lever_scripts_len = 0;
  power_lever_script_t* lever_scripts = SCRIPT_GET_ALL(power_lever_script_t, &lever_scripts_len);
  (void)lever_scripts;

  scripts_save_data_serialize( &power_lever_scripts_state_buffer, lever_scripts, lever_scripts_len );
}
void scripts_save_data_deserialize( u8* buffer, u32* offset, power_lever_script_t* lever_scripts, u32 lever_scripts_len )
{
  int version = serialization_deserialize_s32(buffer, offset);  
  (void)version;

  u32 count = serialization_deserialize_u32(buffer, offset);
  if (count != lever_scripts_len)
  {
    P_V(count);
    P_V(lever_scripts_len);
    ERR("saved amount of lever_scripts and current one not the same");
  }
  for (int i = 0; i < (int)count; ++i)
  {
    lever_scripts[i].action_type     = serialization_deserialize_s32(buffer, offset);  

    lever_scripts[i].interactable_id = serialization_deserialize_s32(buffer, offset);  
    lever_scripts[i].action_max_t    = serialization_deserialize_f32(buffer, offset);  
    
    serialization_deserialize_vec3(buffer, offset, lever_scripts[i].start);  
    serialization_deserialize_vec3(buffer, offset, lever_scripts[i].end);  
  }
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

  if ( buffer != NULL )
  {
    scripts_save_data_deserialize( buffer, &offset, lever_scripts, lever_scripts_len );
    free(buffer);
  }
  else { P_ERR( "failed to load power_lever.save_data" ); }
}
void scripts_load_save_data_from_buffer()
{
  ASSERT( power_lever_scripts_state_buffer != NULL );
  u32 lever_scripts_len = 0;
  power_lever_script_t* lever_scripts = SCRIPT_GET_ALL(power_lever_script_t, &lever_scripts_len);

  u32 offset = 0;

  scripts_save_data_deserialize( power_lever_scripts_state_buffer, &offset, lever_scripts, lever_scripts_len );
}

#ifdef EDITOR
void editor_entity_ui_callback( ui_context* ctx, entity_t* e )
{
  if ( e->template_idx != ENTITY_TEMPLATE_POWER_LEVER_02 ) { return; }

  power_lever_script_t* script = SCRIPT_ENTITY_GET( power_lever_script_t, e );

  if (script != NULL && nk_tree_push(ctx, NK_TREE_TAB, "power_lever_script_t", NK_MINIMIZED))
  {
    #define ACTION_TYPES_LEN 2
    char action_types[ACTION_TYPES_LEN][18] = { "LEVER_ACTION_MOVE", "LEVER_ACTION_" };
    int selected = 0;
    selected = script->action_type;
    if (nk_combo_begin_label(ctx, action_types[selected], nk_vec2(nk_widget_width(ctx), 200))) 
    {
      nk_layout_row_dynamic(ctx, 35, 1);
      for (int i = 0; i < ACTION_TYPES_LEN; ++i)
      { 
        if (nk_combo_item_label(ctx, action_types[i], NK_TEXT_LEFT))
        { selected = i; }
      }
      nk_combo_end(ctx);
    }
    // if ( selected != (int)script->action_type )
    // {
    // }

    nk_labelf( ctx, NK_LEFT, "activated: %s", script->activated ? "true" : "false" );
    nk_bool act = (nk_bool)script->activated;
    nk_checkbox_label( ctx, "activated", &act );
    if ( act != script->activated ) { power_lever_script_t_set_activated( script, (bool)act ); }

    nk_property_int(ctx, "interactable_id", 0, &script->interactable_id, 2048, 0, 0.01f);
    nk_property_float(ctx, "action_max_t", -2048.0f, &script->action_max_t, 2048.0f, 0.1f, 0.01f);

    if ( script->action_type == LEVER_ACTION_MOVE )
    {
      if ( nk_button_label(ctx, "set start to entity pos") ) { vec3_copy(e->pos, script->start); }
      if ( nk_button_label(ctx, "set start to interactable pos") ) 
      { 
        bool err = false;
        entity_t* interactable = state_entity_get_err(script->interactable_id, &err);
        if (!err) { vec3_copy(interactable->pos, script->start); }
      }
      nk_layout_row_dynamic(ctx, 30, 3);
      nk_property_float(ctx, "start.x", -2048.0f, &script->start[0], 2048.0f, 0.1f, 0.01f);
      nk_property_float(ctx, "start.y", -2048.0f, &script->start[1], 2048.0f, 0.1f, 0.01f);
      nk_property_float(ctx, "start.z", -2048.0f, &script->start[2], 2048.0f, 0.1f, 0.01f);

      nk_layout_row_dynamic(ctx, 30, 1);
      if ( nk_button_label(ctx, "set end to entity pos") ) { vec3_copy(e->pos, script->end); }
      if ( nk_button_label(ctx, "set end to interactable pos") ) 
      { 
        bool err = false;
        entity_t* interactable = state_entity_get_err(script->interactable_id, &err);
        if (!err) { vec3_copy(interactable->pos, script->end); }
      }
      nk_layout_row_dynamic(ctx, 30, 3);
      nk_property_float(ctx, "end.x", -2048.0f, &script->end[0], 2048.0f, 0.1f, 0.01f);
      nk_property_float(ctx, "end.y", -2048.0f, &script->end[1], 2048.0f, 0.1f, 0.01f);
      nk_property_float(ctx, "end.z", -2048.0f, &script->end[2], 2048.0f, 0.1f, 0.01f);

      debug_draw_sphere( script->start, 0.5f, RGBF(0, 1, 0) );
      debug_draw_sphere( script->end, 0.5f, RGBF(1, 0, 0) );
      debug_draw_line( script->start, script->end, RGBF(1, 1, 1) );
    }

    nk_tree_pop(ctx);
  }

}
#endif // EDITOR

