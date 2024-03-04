#include "editor/editor_save.h"
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


int selected_id = -1;
int outline_id  = -1;

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
void editor_save_init()
{
  TRACE();

  event_sys_register_play_state(editor_save_play_state_callback);

  editor_save_load_info_from_file();
}

void editor_save_cleanup()
{
  editor_save_write_info_to_file();
}

void editor_save_info()
{
  TRACE();

  selected_id = app_data->selected_id;
  outline_id  = core_data->outline_id;
}
void editor_load_info()
{
  TRACE();

  app_data->selected_id = selected_id;
  core_data->outline_id = outline_id;
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

  
  app_data->gizmo_type  = serialization_deserialize_s32(buffer, &offset);
  app_data->gizmo_space = serialization_deserialize_s32(buffer, &offset);

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
  
  FREE(buffer);
}

