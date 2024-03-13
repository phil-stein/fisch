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

// ---- structures ----

#ifdef EDITOR
void save_sys_write_structure_to_file(const char* name, int root_entity_id)
{
  TRACE();

  u8* buffer = NULL;
   
  entity_t* root = state_entity_get(root_entity_id);
  
  u32 len = 0;
  state_entity_get_total_children_len(root, (int*)&len);
  len++; // for root entity
  serialization_serialize_u32(&buffer, len); // structure length, amount of entities
  // P_U32(len);
  
  int* entity_idxs = NULL; 
  int  entity_idxs_pos = 0;
  MALLOC(entity_idxs, len * sizeof(u32));
  
  save_sys_get_structure_idxs(entity_idxs, &entity_idxs_pos, root);

  entity_t _root = *root;
  vec3_copy(VEC3(0), _root.pos);
  save_sys_serialize_structure(&buffer, entity_idxs, len, &_root);

  FREE(entity_idxs);

  SERIALIZATION_P("[serialization] serialized structure");

  char path[ASSET_PATH_MAX +64];
  SPRINTF(ASSET_PATH_MAX + 64, path, "%s%s%s%s", core_data->asset_path, "/structures/", name, ".struct");
  file_io_write(path, (const char*)buffer, (int)arrlen(buffer));

  ARRFREE(buffer);
}
void save_sys_get_structure_idxs(int* arr, int* arr_pos, entity_t* e)
{
  TRACE();

  // PF("-> structure[%d] id: %d\n", (*arr_pos), root->id);
  arr[(*arr_pos)++] = e->id;

  for (int i = 0; i < e->children_len; ++i)
  {
    entity_t* c = state_entity_get(e->children[i]);
    save_sys_get_structure_idxs(arr, arr_pos, c);
  }
}
void save_sys_serialize_structure(u8** buffer, int* idxs, u32 idxs_len, entity_t* e)
{
  TRACE();

  // serialize children len
  //  - each child as index into the structure
  // serialize entity
  // -> recursion

  serialization_serialize_u32(buffer, (u32)e->children_len); // structure length, amount of entities
  for (int j = 0; j < e->children_len; ++j)
  {
    for (int k = 0; k < (int)idxs_len; ++k)
    {
      if (idxs[k] == e->children[j])
      { 
        // PF(" -> entity: %d -> child: %d\n", root->id, idxs[k]);
        serialization_serialize_u32(buffer, (u32)k); 
      }  // serialize index of child into structure 
    }
  }
  entity_t _e = *e;     // copy entity, to not modify original
  _e.children_len =  0;
  _e.parent       = -1;
  save_sys_serialize_entity(buffer, &_e);

  for (int i = 0; i < e->children_len; ++i)
  {
    entity_t* c = state_entity_get(e->children[i]);  
    save_sys_serialize_structure(buffer, idxs, idxs_len, c); // use recursion
  }
}
int save_sys_load_structure_from_file(const char* name)
{
  TRACE();

  char path[ASSET_PATH_MAX +64];
  SPRINTF(ASSET_PATH_MAX + 64, path, "%s%s%s%s", core_data->asset_path, "/structures/", name, ".struct");
  int file_len = 0;
  u8* buffer = (u8*)file_io_read_bytes(path, &file_len);
  u32 offset = 0;

  u32 len = serialization_deserialize_u32(buffer, &offset); // structure length, amount of entities
  
  // child_idxs[i / entity idxs in structure][0] = length of [][X] array
  // child_idxs[i / entity idxs in structure][X] = e.children[X-1] of entity
  #define CHILD_IDXS_MAX_ENTITIES 64 
  ASSERT(len < CHILD_IDXS_MAX_ENTITIES);
  #define CHILD_IDXS_MAX_IDXS_INC_LEN 64
  #define CHILD_IDXS_MAX_IDXS CHILD_IDXS_MAX_IDXS_INC_LEN - 1
  int child_idxs[CHILD_IDXS_MAX_ENTITIES][CHILD_IDXS_MAX_IDXS_INC_LEN];
  int entity_ids[CHILD_IDXS_MAX_ENTITIES]; 

  int rtn_id = -1;
    
  // deserealize entities
  for (int i = 0; i < (int)len; ++i)
  {
    int c_len = (int)serialization_deserialize_u32(buffer, &offset); // amount of child idxs
    ASSERT(c_len < CHILD_IDXS_MAX_IDXS);
    child_idxs[i][0] = c_len;
  
    for (int j = 1; j < c_len +1; ++j) // 0 is len, tsart at 1
    {
      child_idxs[i][j] = (int)serialization_deserialize_u32(buffer, &offset);
    }

    entity_ids[i] = save_sys_deserialize_entity(buffer, &offset);
    if (rtn_id < 0) { rtn_id = entity_ids[i]; }
    // P_U32(entity_ids[i]);
  }
  // parent entities
  for (int i = 0; i < (int)len; ++i)
  {
    int c_len = child_idxs[i][0];

    for (int j = 1; j < c_len +1; ++j) // 0 is len, tsart at 1
    {
      // PF("-> parented: parent: %d, child: %d\n", entity_ids[i], entity_ids[child_idxs[i][j]]);
      state_entity_add_child_id(entity_ids[i], entity_ids[child_idxs[i][j]], false);
    }

  }

  FREE(buffer);

  return rtn_id;
}
#endif
