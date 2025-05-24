#ifndef CORE_TYPES_MATERIAL_H
#define CORE_TYPES_MATERIAL_H


#include "global/global.h"
#include "math/math_inc.h"
#include "core/templates/material_template.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define MATERIAL_T_NAME_MAX 128
typedef struct material_t
{
  int template_idx; // idx for material_template_get() // @TODO: 

  int  albedo;    // index for assetm 
  rgbf tint;
  bool translucent;
  
  int normal;    // index for assetm 
  
  int roughness; // index for assetm 
  f32 roughness_f;
  
  int metallic;  // index for assetm 
  f32 metallic_f;
  
  int emissive;  // index for assetm 
  f32 emissive_f;

  int shader; // @TODO: add shaders to assetm, not all, have variable shaders in deferred pass

  f32  tile_scl;    // getts mul with tile
  bool tile_by_scl; // scale uv by ent scl
  vec2 tile;        // tile/scale uv coords
  
  #ifdef EDITOR
  char name[MATERIAL_T_NAME_MAX];  // name, only exists if EDITOR defined
  #endif // EDITOR

} material_t;

// @DOC: load material from template in data/material_template.h
//       m:            the material_template_t with all the properties to make the mat
//       template_idx: the idx for material_template_get() corresponding to m
material_t material_load_from_template(const material_template_t* m, int idx);
// @DOC: make material, sets tile automatic
//       albedo:      idx for texture in assetm
//       normal:      idx for texture in assetm
//       roughness:   idx for texture in assetm
//       metallic:    idx for texture in assetm
//       emissive:    idx for texture in assetm
//       tint:        tint color for material
//       roughness_f: mult for roughness
//       metallic_f:  mult for metalness
//       emissive_f:  mult for emissive 
//       shader:      idx for shader in assetm
// material_t material_make_basic(int albedo, int normal, int roughness, int metallic, rgbf tint, f32 roughness_f, f32 metallic_f, int shader);
material_t material_make_basic(int albedo, int normal, int roughness, int metallic, int emissive, rgbf tint, bool translucent, f32 roughness_f, f32 metallic_f, f32 emissive_f, int shader, const char* name);
// @DOC: make material
//       albedo:       idx for texture in assetm
//       normal:       idx for texture in assetm
//       roughness:    idx for texture in assetm
//       metallic:     idx for texture in assetm
//       emissive:    idx for texture in assetm
//       tint:         tint color for material
//       roughness_f:  mult for roughness
//       metallic_f:   mult for metalness
//       emissive_f:   mult for emissive 
//       shader:       idx for shader in assetm
//       tile_scl:     mult for uv in shader, i.e. tile
//       tile_by_scl:  sets tile_scl to object scl
//       tile:         overall tile, i.e. (2,1) you can see the material 2 times in x but only once on y 
//       template_idx: the idx for material_template_get(), or -1 if not using template
// material_t material_make(int albedo, int normal, int roughness, int metallic, rgbf tint, f32 roughness_f, f32 metallic_f, int shader, f32 tile_scl, bool tile_by_scl, vec2 tile, material_template_type template_idx);
material_t material_make(int albedo, int normal, int roughness, int metallic, int emissive, rgbf tint, bool translucent, f32 roughness_f, f32 metallic_f, f32 emissive_f, int shader, f32 tile_scl, bool tile_by_scl, vec2 tile, int template_idx, const char* name);

#ifdef __cplusplus
} // etxern C
#endif

#endif
