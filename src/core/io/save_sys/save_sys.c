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


// @NOTE: replaced by core_data->scene_name
// #define CUR_SCENE_NAME_MAX SCENE_NAME_MAX 
// char cur_scene_name[CUR_SCENE_NAME_MAX] = "";

// @NOTE: replaced by core_data->save_sys_version
// int cur_version = 0;

static core_data_t* core_data = NULL;


void save_sys_init()
{
  core_data = core_data_get();
}


void save_sys_load_scene_terrain(const char* _scene_name, const char* _terrain_name)
{
  if (_scene_name != NULL)
  {  
    STRCPY(core_data->scene_name,  _scene_name);
    TIMER_FUNC_STATIC(save_sys_load_scene_from_file(core_data->scene_name));
  }
  if (_terrain_name != NULL)
  {
    STRCPY(core_data->terrain_name, _terrain_name);
    TIMER_FUNC_STATIC(save_sys_load_terrain_from_file(core_data->terrain_name));
    TIMER_FUNC_STATIC(terrain_create(25));
  }
}

// ---- complex types ----

void save_sys_serialize_entity(u8** buffer, entity_t* e)
{ 
  if (e->is_dead) { return; }
  serialization_serialize_s32(buffer, e->template_idx);

  serialization_serialize_vec3(buffer, e->pos);  
  serialization_serialize_vec3(buffer, e->rot);   
  serialization_serialize_vec3(buffer, e->scl);  

  serialization_serialize_u8(buffer, e->point_light_idx >= 0 ? 1 : 0);  // if has point light
  if (e->point_light_idx >= 0)
  {
    bool error = false;
    point_light_t* l = ecs_point_light_get(e->point_light_idx, &error); ASSERT(!error);
    save_sys_serialize_point_light(buffer, l);
  }

  serialization_serialize_s32(buffer, e->parent); 
  serialization_serialize_s32(buffer, e->children_len);
  for (u32 i = 0; i < e->children_len; ++i)
  {
    serialization_serialize_s32(buffer, e->children[i]);
  }
}
int save_sys_deserialize_entity(u8* buffer, u32* offset)
{
  int template_idx = serialization_deserialize_s32(buffer, offset);

  vec3 pos, rot, scl;
  serialization_deserialize_vec3(buffer, offset, pos); 
  serialization_deserialize_vec3(buffer, offset, rot); 
  serialization_deserialize_vec3(buffer, offset, scl); 

  int id = ecs_entity_add_from_template(pos, rot, scl, template_idx, false);
  // const entity_template_t* def = entity_template_get(template_idx);
 
  entity_t* e = ecs_entity_get(id);
   
  u8 has_point_light = serialization_deserialize_u8(buffer, offset);  // if has point light
  if (has_point_light)
  {
    // @NOTE: when template has pointlight it gets added in ecs_entity_add_from_template()
    //        and we need to overwrite it to preserve values
    if (e->point_light_idx >= 0)
    { ecs_point_light_remove(e->point_light_idx); }
    e->point_light_idx = save_sys_deserialize_point_light(buffer, offset, id);
  }

  e->parent = serialization_deserialize_s32(buffer, offset);        
  e->children_len = serialization_deserialize_s32(buffer, offset);  
  for (u32 i = 0; i < e->children_len; ++i)
  {
    arrput(e->children, serialization_deserialize_s32(buffer, offset));
  }
  // PF("id: %d, parent: %d, children_len: %d\n", e->id, e->parent, e->children_len);

  return id;
}

void save_sys_serialize_dir_light(u8** buffer, dir_light_t* l)
{ 
  serialization_serialize_vec3(buffer, l->pos);
  serialization_serialize_vec3(buffer, l->dir);
  serialization_serialize_vec3(buffer, l->color);
  
  serialization_serialize_f32(buffer, l->intensity);

  serialization_serialize_u8(buffer, l->cast_shadow);
  serialization_serialize_s32(buffer, l->shadow_map_x);
  serialization_serialize_s32(buffer, l->shadow_map_y);
}
void save_sys_deserialize_dir_light(u8* buffer, u32* offset)
{
  vec3 pos, dir, color;
  serialization_deserialize_vec3(buffer, offset, pos);
  serialization_deserialize_vec3(buffer, offset, dir);
  serialization_deserialize_vec3(buffer, offset, color);
  
  f32 intensity = serialization_deserialize_f32(buffer, offset);

  bool cast_shadow = serialization_deserialize_u8(buffer, offset);
  int shadow_map_x = serialization_deserialize_s32(buffer, offset);
  int shadow_map_y = serialization_deserialize_s32(buffer, offset);

  ecs_dir_light_add(pos, dir, color, intensity, cast_shadow, shadow_map_x, shadow_map_y);
}

void save_sys_serialize_point_light(u8** buffer, point_light_t* l)
{
  serialization_serialize_vec3(buffer, l->offset);
  serialization_serialize_vec3(buffer, l->color);
  serialization_serialize_f32(buffer, l->intensity);
  // entity_id gets set in entity_deserealize
}
int save_sys_deserialize_point_light(u8* buffer, u32* offset, int entity_id)
{
  vec3 _offset, color;
  serialization_deserialize_vec3(buffer, offset, _offset);
  serialization_deserialize_vec3(buffer, offset, color);
  f32 intensity = serialization_deserialize_f32(buffer, offset);

  return ecs_point_light_add(_offset, color, intensity, entity_id);
}


