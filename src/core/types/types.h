#ifndef CORE_TYPES_H
#define CORE_TYPES_H

#include "global/global.h"
#include "core/types/texture.h"
#include "core/types/material.h" 
#include "core/types/mesh.h" 
#include "core/types/shader.h" 
#include "core/types/framebuffer.h"
#include "core/types/entity.h"
#include "core/types/cubemap.h"
#include "math/math_inc.h"


typedef struct dir_light_t
{
  vec3  pos;    // position // @TODO: remove this, use other pos for shadows
  vec3  dir;    // direction vector
  rgbf  color;  // color of light
  // rgbf  ambient;
  // rgbf  diffuse;
  float intensity;  // multiplier for lights effect on the world 

  // only used when cast_shadow is true 
  bool cast_shadow; 
  int  shadow_map_x;
  int  shadow_map_y;
  framebuffer_t fb_shadow;
  mat4 view;  // view matrix from lights perspective
  mat4 proj;  // projection matrxix from lights perspective

}dir_light_t;

typedef struct point_light_t
{
  bool is_dead;     // instead of deleting the light from array, its marked dead and overwritten with the next added point_light
  int  id;          // for state_get_point_light(id)
  int  entity_id;   // either -1 or entity id the light is attached to
  
  // f32*  pos_ptr;    // position, points to entity_t.pos
  vec3  offset;     // position offset from its entity 'parent'
  rgbf  color;      // color of light
  float intensity;  // multiplier for lights effect on the world


}point_light_t;

#endif
