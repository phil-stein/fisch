#ifndef CORE_TYPES_CUBEMAP_H
#define CORE_TYPES_CUBEMAP_H

#include "global/global.h"
// #include "core/types/types.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CUBE_MAP_NAME_MAX 128

// @DOC: @TODO:
typedef struct cubemap_t
{
  bool loaded;
  char name[CUBE_MAP_NAME_MAX];

  u32 environment;  
  u32 irradiance;
  u32 prefilter;
  // u32 brdf_lut;
  f32 intensity; 
}cubemap_t;

void cubemap_free();
cubemap_t cubemap_load_dbg(const char* path, const char* _file, const int _line); 
// @DOC: load cubemap from .hdr file
//       path: path to .hdr file
#define cubemap_load(path)       cubemap_load_dbg(path, __FILE__, __LINE__)

// -- old --

// u32 assetm_create_cubemap_dbg(const char* name, const char* ext, const char* file, const int line);
// @NOTE: no longer in use
// #define assetm_create_cubemap(name, ext)    assetm_create_cubemap_dbg(name, ext, __FILE__, __LINE__)

#ifdef __cplusplus
} // extern C
#endif

#endif
