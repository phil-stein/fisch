#include "core/io/save_sys/save_sys.h"
#include "core/core_data.h"
#include "core/state/state.h"
#include "core/io/assetm.h"
#include "core/io/file_io.h"
#include "core/types/cubemap.h"
#include "core/debug/debug_timer.h"
#include "serialization/serialization.h"
#include "core/camera.h"

#include "stb/stb_ds.h"

u8*  state_buffer = NULL;
char state_buffer_scene_name[SCENE_NAME_MAX] = "";

vec3 state_cam_pos         = { 0, 0, 0 };
// vec3 state_cam_orientation = { 0, 0, 0 };
f32  state_cam_pitch, state_cam_yaw;

// ---- scene ----

#ifdef EDITOR
void save_sys_write_empty_scene_to_file(const char* name)
{  char path[ASSET_PATH_MAX +64];
  SPRINTF(ASSET_PATH_MAX + 64, path, "%s%s", core_data->asset_path, name);
  save_sys_write_empty_scene_to_path(path);
}
void save_sys_write_empty_scene_to_path(const char* path)
{
  TRACE();
  u8* buffer = NULL;

  serialization_serialize_u32(&buffer, SAVE_SYS_VERSION);
   
  // -- cubemap --
  serialization_serialize_f32(&buffer, 1.0f);  // cube_map.intensity
  serialization_serialize_str(&buffer, "#cubemaps/gothic_manor_01_2k.hdr"); // cubemap.name
  
  // serialization_serialize_scene(&buffer);
  serialization_serialize_u32(&buffer, 0); // world_len
  serialization_serialize_u32(&buffer, 0); // dir_lights_len
  serialization_serialize_u32(&buffer, 0); // point_lights_len
  
  SERIALIZATION_P("[serialization] serialized empty scene");

  file_io_write(path, (const char*)buffer, (int)arrlen(buffer));

  ARRFREE(buffer);
}
#endif

void save_sys_write_scene_to_file(const char* name)
{
  char path[ASSET_PATH_MAX +64];
  SPRINTF(ASSET_PATH_MAX + 64, path, "%s%s", core_data->asset_path, name);
  save_sys_write_scene_to_path(path);
}
void save_sys_write_scene_to_path(const char* path)
{
  TRACE();

  u8* buffer = NULL;

  save_sys_serialize_scene(&buffer);
  
  file_io_write(path, (const char*)buffer, (int)arrlen(buffer));

  ARRFREE(buffer);
}

void save_sys_load_scene_from_file_dbg(const char* name, const char* _file, const char* _func, const int _line)
{
  (void)_file; (void)_func; (void)_line;
  TRACE();

  // P_INFO("%s called from\n -> %s, %d\n -> %s\n", __func__, _func, _line, _file);
  
  u32 offset = 0;
  int length = 0;

  PF("save_sys_load_scene_from_file(%s)\n called from -> \"%s\", line: %d\n", name, _file, _line);
  // P_STR(core_data->asset_path);
  char path[ASSET_PATH_MAX +64];
  SPRINTF(ASSET_PATH_MAX +64, path, "%s%s", core_data->asset_path, name);
  u8* buffer = (u8*)file_io_read_bytes(path, &length);
  save_sys_deserialize_scene(buffer, &offset);

  ASSERT(strlen(name) < SCENE_NAME_MAX);
  // strcpy(cur_scene_name, name);
  strcpy(core_data->scene_name, name);

  FREE(buffer);
}
void save_sys_load_scene_from_path_dbg(const char* path, const char* _file, const char* _func, const int _line)
{
  (void)_file; (void)_func; (void)_line;
  TRACE();

  // P_INFO("%s called from\n -> %s, %d\n -> %s\n", __func__, _func, _line, _file);
  
  u32 path_len = (u32)strlen(path);
  // walk back until first \ or /
  char* name = NULL;
  for (int i = (int)path_len -1; i >= 0; --i) 
  {
    if (path[i] == '\\' || path[i] == '/') { break; }
    name = (char*)&path[i];
  }
  P_STR(name);

  u32 offset = 0;
  int length = 0;

  PF("save_sys_load_scene_from_path(%s)\n path: %s\n called from -> \"%s\", line: %d\n", name, path, _file, _line);

  u8* buffer = (u8*)file_io_read_bytes(path, &length);
  save_sys_deserialize_scene(buffer, &offset);

  ASSERT(strlen(name) < SCENE_NAME_MAX);
  // strcpy(cur_scene_name, name);
  strcpy(core_data->scene_name, name);

  FREE(buffer);
}

void save_sys_write_scene_to_current_file()
{
  TRACE();

  save_sys_write_scene_to_file(core_data->scene_name);
}
void save_sys_load_scene_from_current_file()
{
  TRACE();

  save_sys_load_scene_from_file(core_data->scene_name);
}

void save_sys_write_scene_to_state_buffer_dbg(const char* _file, const int _line)
{
  TRACE();

  PF("save_sys_write_scene_to_state_buffer()\n  -> called from: %s, line: %d\n", _file, _line);

  u8* buffer = NULL;

  save_sys_serialize_scene(&buffer);

  REALLOC(state_buffer, (size_t)arrlen(buffer) * sizeof(u8));
  memcpy(state_buffer, buffer, (size_t)arrlen(buffer) * sizeof(u8));

  // strcpy(state_buffer_scene_name, cur_scene_name);
  strcpy(state_buffer_scene_name, core_data->scene_name);

  ARRFREE(buffer);

  vec3_copy(core_data->cam.pos,   state_cam_pos);
  // vec3_copy(core_data->cam.front, state_cam_orientation);
  state_cam_pitch = core_data->cam.pitch_rad; 
  state_cam_yaw   = core_data->cam.yaw_rad;
  
  (void)_file; (void)_line;
}

void save_sys_load_scene_from_state_buffer_dbg(const char* _file, const int _line)
{
  TRACE();

  PF("save_sys_load_scene_from_state_buffer()\n  -> called from: %s, line: %d\n", _file, _line);
  
  u32 offset = 0;
  
  save_sys_deserialize_scene(state_buffer, &offset);

  // strcpy(cur_scene_name, state_buffer_scene_name);
  strcpy(core_data->scene_name, state_buffer_scene_name);

  vec3_copy(state_cam_pos, core_data->cam.pos);
  camera_set_pitch_yaw(state_cam_pitch, state_cam_yaw);
  
  (void)_file; (void)_line;
}

void save_sys_serialize_scene(u8** buffer)
{
  TRACE();
  
  // -- version --

  serialization_serialize_u32(buffer, SAVE_SYS_VERSION);
   
  // -- cubemap --
  serialization_serialize_f32(buffer, core_data->cube_map.intensity);
  serialization_serialize_str(buffer, core_data->cube_map.name); 
  
  // -- entities --

  int world_len = 0;
  int world_dead_len = 0;
  entity_t* world = state_entity_get_arr(&world_len, &world_dead_len);

  serialization_serialize_u32(buffer, (u32)(world_len - world_dead_len));
  P_INT(world_len - world_dead_len);

  for (int i = 0; i < world_len; ++i)
  {
    if (world[i].is_dead) { continue; }
    save_sys_serialize_entity(buffer, &world[i]);
  }

  // -- dir lights --
  
  int dir_lights_len = 0;
  dir_light_t* dir_lights = state_dir_light_get_arr(&dir_lights_len);
   
  serialization_serialize_u32(buffer, (u32)dir_lights_len);

  for (int i = 0; i < dir_lights_len; ++i)
  {
    save_sys_serialize_dir_light(buffer, &dir_lights[i]);
  } 
  
  // @NOTE: now get serialized with entity
  // -- point lights --

  // int point_lights_len = 0;
  // point_light_t* point_lights = state_point_light_get_arr(&point_lights_len);
  //  
  // serialization_serialize_u32(buffer, point_lights_len);

  // for (u32 i = 0; i < point_lights_len; ++i)
  // {
  //   save_sys_serialize_point_light(buffer, &point_lights[i]);
  // } 
  
  SERIALIZATION_P("[serialization] serialized scene");
}
void save_sys_deserialize_scene_dbg(u8* buffer, u32* offset, const char* _file, const char* _func, const int _line)
{
  (void)_file; (void)_func; (void)_line;
  TRACE();
  
  // P_INFO("%s called from\n -> %s, %d\n -> %s\n", __func__, _func, _line, _file);

  // clear pre-existing scene
  state_clear_scene();
 
  // -- version -- 
  
  core_data->save_sys_version = (int)serialization_deserialize_u32(buffer, offset);
  PF("| serialization version: %d\n", core_data->save_sys_version);

  // -- cubemap --
  f32 intensity = serialization_deserialize_f32(buffer, offset);
  char* cube_map_name = serialization_deserialize_str(buffer, offset);
  TIMER_FUNC_STATIC(core_data->cube_map = cubemap_load(cube_map_name));
  core_data->cube_map.intensity = intensity;

  // -- entities --

  u32 world_len = serialization_deserialize_u32(buffer, offset);
  P_U32(world_len);

  for (u32 i = 0; i < world_len; ++i)
  {
    save_sys_deserialize_entity(buffer, offset);
  }
  
  // -- dir lights --
  
  u32 dir_lights_len = serialization_deserialize_u32(buffer, offset);
  P_U32(dir_lights_len);

  for (u32 i = 0; i < dir_lights_len; ++i)
  {
    save_sys_deserialize_dir_light(buffer, offset);
  }

  // @NOTE: now get serialized with entities
  // -- point lights --
  
  // @BUGG: previous pointlight entites get serialized & deserialized
  //        then deserilaize_pointlight adds new entities

  // u32 point_lights_len = serialization_deserialize_u32(buffer, offset);
  // P_U32(point_lights_len);

  // for (u32 i = 0; i < point_lights_len; ++i)
  // {
  //   save_sys_deserialize_point_light(buffer, offset);
  // } 
  
  SERIALIZATION_P("[serialization] deserialized scene");
}
