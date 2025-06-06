#include "editor/editor_save.h"
#include "core/program.h"
#include "core/types/entity.h"
#include "editor/app.h"
#include "core/core_data.h"
#include "core/state/state.h"
#include "core/io/assetm.h"
#include "core/event_sys.h"
#include "core/io/file_io.h"
#include "core/debug/debug_timer.h"
#include "core/camera.h"
#include "serialization/serialization.h"

#include "stb/stb_ds.h"
#include "tinyfiledialogs/tinyfiledialogs.h"

int selected_id = -1;
int outline_id  = -1;
bool template_browser_minimized;
bool hierarchy_win_minimized;
bool struct_browser_minimized;
bool show_frameb_win;
bool show_debug_win;
bool show_light_hierarchy_win;
bool show_core_data_win;
bool show_assetm_win;
bool show_operation_win;
bool tools_win_minimized;


void editor_save_play_state_callback(play_state_type state)
{
  TRACE();

  if (state == PLAY_STATE_PLAY)
  {
    editor_save_info();
    // app_data->selected_id = -1; // so properties stay opeg
    core_data->outline_id = -1;
  }
  else
  {
    editor_load_info();
  }
  P_PLAY_STATE_TYPE(state);
}

void editor_save_program_quit_callback()
{
  // message box asking to save unsaved changes or to cancel quitting
  if (app_data->unsaved_changes)
  {
    int rtn = -1;
    rtn = tinyfd_messageBox(
        "unsaved changes", // NULL or ""
        "attempting to quit with unsaved changes.\n" 
        "save changes ?", // NULL or "" may contain \n \t
        "yesnocancel", // "ok" "okcancel" "yesno" "yesnocancel"
        "warning", // "info" "warning" "error" "question"
        2); // 0 for cancel/no , 1 for ok/yes , 2 for no in yesnocancel
    P_V(rtn);
    while (rtn < 0) { /* wait */  }
    P_V(rtn);
    if (rtn == 0) // cancel
    { 
      program_stop_quit();
    }
    if (rtn == 1) // yes 
    { 
      app_save();
    }
    // if (rtn == 2)  // no 
  }
}

void editor_save_init()
{
  TRACE();

  event_sys_register_play_state(editor_save_play_state_callback);
  event_sys_register_program_quit(editor_save_program_quit_callback);

  editor_save_load_info_from_file();
}

void editor_save_cleanup()
{
  if (core_data_get_play_state() != PLAY_STATE_PLAY)
  { editor_save_write_info_to_file(); }
}

void editor_save_info()
{
  TRACE();

  selected_id = app_data->selected_id;
  outline_id  = core_data->outline_id;

  template_browser_minimized = app_data->template_browser_minimized;
  hierarchy_win_minimized    = app_data->hierarchy_win_minimized;
  struct_browser_minimized   = app_data->struct_browser_minimized;
  show_frameb_win            = app_data->show_frameb_win;
  show_debug_win             = app_data->show_debug_win;
  show_light_hierarchy_win   = app_data->show_light_hierarchy_win;
  show_core_data_win         = app_data->show_core_data_win;
  show_assetm_win            = app_data->show_assetm_win;
  show_operation_win         = app_data->show_operation_win;
  tools_win_minimized        = app_data->tools_win_minimized;
}
void editor_load_info()
{
  TRACE();

  app_data->selected_id = selected_id;
  core_data->outline_id = outline_id;

  app_data->template_browser_minimized = template_browser_minimized; 
  app_data->hierarchy_win_minimized    = hierarchy_win_minimized;    
  app_data->struct_browser_minimized   = struct_browser_minimized;   
  app_data->show_frameb_win            = show_frameb_win;            
  app_data->show_debug_win             = show_debug_win;             
  app_data->show_light_hierarchy_win   = show_light_hierarchy_win;   
  app_data->show_core_data_win         = show_core_data_win;         
  app_data->show_assetm_win            = show_assetm_win;            
  app_data->show_operation_win         = show_operation_win;         
  app_data->tools_win_minimized        = tools_win_minimized;        

  app_data->template_browser_set_minimized = app_data->template_browser_minimized;
  app_data->hierarchy_win_set_minimized    = app_data->hierarchy_win_minimized;
  app_data->struct_browser_set_minimized   = app_data->struct_browser_minimized;
  app_data->tools_win_set_minimized        = app_data->tools_win_minimized; 
}

void editor_save_write_info_to_file()
{
  TRACE();

  u8* buffer = NULL;
  
  serialization_serialize_s32(&buffer, EDITOR_SAVE_VERSION);
  
  serialization_serialize_f32(&buffer,  core_data->cam.pitch_rad);
  serialization_serialize_f32(&buffer,  core_data->cam.yaw_rad);
  serialization_serialize_vec3(&buffer, core_data->cam.pos);
  
  serialization_serialize_s32(&buffer, core_data->outline_id);
  serialization_serialize_s32(&buffer, app_data->selected_id);

  
  serialization_serialize_s32(&buffer, app_data->gizmo_type);
  serialization_serialize_s32(&buffer, app_data->gizmo_space);

  serialization_serialize_u8(&buffer, app_data->template_browser_minimized);
  serialization_serialize_u8(&buffer, app_data->hierarchy_win_minimized);
  serialization_serialize_u8(&buffer, app_data->struct_browser_minimized);
  serialization_serialize_u8(&buffer, app_data->show_frameb_win);
  serialization_serialize_u8(&buffer, app_data->show_debug_win);
  serialization_serialize_u8(&buffer, app_data->show_light_hierarchy_win);
  serialization_serialize_u8(&buffer, app_data->show_core_data_win);
  serialization_serialize_u8(&buffer, app_data->show_assetm_win);
  serialization_serialize_u8(&buffer, app_data->show_operation_win);
  // version 2
  serialization_serialize_u8(&buffer, app_data->tools_win_minimized );
  P_V( app_data->tools_win_minimized );
  
  // char path[ASSET_PATH_MAX +64];
  // SPRINTF(ASSET_PATH_MAX + 64, path, "%s%s%s%s", core_data->asset_path, "/editor/", EDITOR_SAVE_NAME, EDITOR_SAVE_EXTENSION);
  char* path = EDITOR_SAVE_NAME;
  file_io_write(path, (const char*)buffer, (int)arrlen(buffer));

  ARRFREE(buffer);
}

void editor_save_load_info_from_file()
{
  TRACE();

  u32 offset = 0;
  int length = 0;

  char* path = EDITOR_SAVE_NAME;
  u8* buffer = (u8*)file_io_read_bytes(path, &length);
  if (buffer == NULL) { P_ERR("%s() couldnt find %s, stopping\n", __func__, path); return; }

  app_data->editor_save_version = serialization_deserialize_s32(buffer, &offset);
  
  f32 pitch = serialization_deserialize_f32(buffer, &offset);
  f32 yaw   = serialization_deserialize_f32(buffer, &offset);
  camera_set_pitch_yaw(pitch, yaw);
  serialization_deserialize_vec3(buffer, &offset, core_data->cam.pos);
  
  core_data->outline_id = serialization_deserialize_s32(buffer, &offset);
  app_data->selected_id = serialization_deserialize_s32(buffer, &offset);
  // checking if id's are valid, f.e. if selected entity wasnt saved
  bool error = false;
  if (app_data->selected_id >= 0)
  {
    state_entity_get_err(app_data->selected_id, &error);
    if (error) { app_data->selected_id = -1; P_INFO("error because app_data->selected_id wasnt valid\n"); }
  }
  if (core_data->outline_id >= 0)
  {
    state_entity_get_err(core_data->outline_id, &error);
    if (error) { core_data->outline_id = -1; P_INFO("error because core_data->outline_id wasnt valid\n"); }
  }

  
  // @TODO: removed this cause causes opengl error in editor_render_mouse_pick()
  /* app_data->gizmo_type  = */ serialization_deserialize_s32(buffer, &offset);
  /* app_data->gizmo_space = */ serialization_deserialize_s32(buffer, &offset);

  app_data->template_browser_minimized = serialization_deserialize_u8(buffer, &offset);
  app_data->hierarchy_win_minimized    = serialization_deserialize_u8(buffer, &offset);
  app_data->struct_browser_minimized   = serialization_deserialize_u8(buffer, &offset);
  app_data->show_frameb_win            = serialization_deserialize_u8(buffer, &offset);
  app_data->show_debug_win             = serialization_deserialize_u8(buffer, &offset);
  app_data->show_light_hierarchy_win   = serialization_deserialize_u8(buffer, &offset);
  app_data->show_core_data_win         = serialization_deserialize_u8(buffer, &offset);
  app_data->show_assetm_win            = serialization_deserialize_u8(buffer, &offset);
  app_data->show_operation_win         = serialization_deserialize_u8(buffer, &offset);
  app_data->template_browser_set_minimized = app_data->template_browser_minimized;
  app_data->hierarchy_win_set_minimized    = app_data->hierarchy_win_minimized;
  app_data->struct_browser_set_minimized   = app_data->struct_browser_minimized;
  if (app_data->editor_save_version >= 2 )
  {
    app_data->tools_win_minimized     = serialization_deserialize_u8(buffer, &offset);
    app_data->tools_win_set_minimized = app_data->tools_win_minimized; 
    P_V( app_data->tools_win_minimized );
  }
  else { app_data->tools_win_set_minimized = true; P_ERR( "editor save version not 2" );  }
  
  FREE(buffer);
}

