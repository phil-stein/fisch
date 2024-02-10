#include "core/io/save_sys/save_sys.h"
#include "core/core_data.h"
#include "core/ecs/ecs.h"
#include "core/io/assetm.h"
#include "core/io/file_io.h"
#include "core/types/cubemap.h"
#include "core/debug/debug_timer.h"
#include "serialization/serialization.h"
#include "core/camera.h"

#include "stb/stb_ds.h"

static core_data_t* core_data = NULL;

// ---- terrain ----

void save_sys_write_terrain_to_file(const char* name)
{
  core_data = core_data_get();

  u8* buffer = NULL;

  save_sys_serialize_terrain(&buffer);

  char path[ASSET_PATH_MAX +64];
  SPRINTF(ASSET_PATH_MAX + 64, path, "%s%s", core_data->asset_path, name);
  file_io_write(path, (const char*)buffer, (int)arrlen(buffer));

  ARRFREE(buffer);
}
void save_sys_load_terrain_from_file(const char* name)
{
  core_data = core_data_get();

  u32 offset = 0;
  int length = 0;
  
  char path[ASSET_PATH_MAX +64];
  SPRINTF(ASSET_PATH_MAX + 64, path, "%s%s", core_data->asset_path, name);
  u8* buffer = (u8*)file_io_read_bytes(path, &length);
  
  save_sys_deserialize_terrain(buffer, &offset);

  FREE(buffer);
}
void save_sys_write_terrain_to_current_file()
{
  core_data = core_data_get();
  save_sys_write_terrain_to_file(core_data->terrain_name);
}
void save_sys_load_terrain_from_current_file()
{
  core_data = core_data_get();
  save_sys_load_terrain_from_file(core_data->terrain_name);
}

void save_sys_serialize_terrain(u8** buffer)
{
  core_data = core_data_get();
  
  serialization_serialize_f32(buffer, core_data->terrain_scl);
  serialization_serialize_f32(buffer, core_data->terrain_y_scl);
  serialization_serialize_u32(buffer, core_data->terrain_x_len);
  serialization_serialize_u32(buffer, core_data->terrain_z_len);

  serialization_serialize_u32(buffer, core_data->terrain_layout_len);
  
  for (u32 i = 0; i < core_data->terrain_layout_len; ++i)
  {
    save_sys_serialize_terrain_layout(buffer, &core_data->terrain_layout[i]);
  }
  SERIALIZATION_P("[serialization] serialized terrain");
}
void save_sys_deserialize_terrain(u8* buffer, u32* offset)
{
  core_data = core_data_get();
  
  core_data->terrain_scl   = serialization_deserialize_f32(buffer, offset); 
  core_data->terrain_y_scl = serialization_deserialize_f32(buffer, offset);  
  core_data->terrain_x_len = serialization_deserialize_u32(buffer, offset); 
  core_data->terrain_z_len = serialization_deserialize_u32(buffer, offset); 

  core_data->terrain_layout = NULL;
  core_data->terrain_layout_len = serialization_deserialize_u32(buffer, offset);
  for (u32 i = 0; i < core_data->terrain_layout_len; ++i)
  {
    terrain_layout_t l;
    l.pos[0] = 0; l.pos[1] = 0; // vs doesnt like uninitialized struct l
    arrput(core_data->terrain_layout, l);
    save_sys_deserialize_terrain_layout(buffer, offset, &core_data->terrain_layout[i]);
  }
  SERIALIZATION_P("[serialization] deserialized terrain");
}


void save_sys_serialize_terrain_layout(u8** buffer, terrain_layout_t* l)
{
  core_data = core_data_get();
  
  u32 height_len = TERRAIN_LAYOUT_VERT_INFO_LEN(core_data);
  
  serialization_serialize_ivec2(buffer, l->pos);
  for (u32 i = 0; i < height_len; ++i)
  {
    serialization_serialize_f32(buffer, l->vert_info[i][0]); // height
    serialization_serialize_f32(buffer, l->vert_info[i][1]); // material
  }

}
void save_sys_deserialize_terrain_layout(u8* buffer, u32* offset, terrain_layout_t* l)
{
  core_data = core_data_get();
  
  u32 height_len = TERRAIN_LAYOUT_VERT_INFO_LEN(core_data);
  
  serialization_deserialize_ivec2(buffer, offset, l->pos);
  MALLOC(l->vert_info, height_len * sizeof(vec2));
  for (u32 i = 0; i < height_len; ++i)
  {
    l->vert_info[i][0] = serialization_deserialize_f32(buffer, offset);  // height
    l->vert_info[i][1] = serialization_deserialize_f32(buffer, offset);  // material
  }
}


