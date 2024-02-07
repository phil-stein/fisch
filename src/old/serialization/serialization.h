#ifndef SERIALIZATION_SERIALIZATION_H
#define SERIALIZATION_SERIALIZATION_H

#include "global/global.h"
#include "math/math_inc.h"

// @NOTE: serialization for base types
//        check serialization_test() for an example
//        requires std_ds: https://github.com/nothings/stb
//                 global: https://github.com/phil-stein/global
//                 math:   https://github.com/phil-stein/math


// @DOC: used for shoving f32 into byte array
//       f32_wrapper.f_val = 1.23f;
//       f32_wrapper.u_val is now a serializable u32
//       deserialize that u32
//       f32_wrapper.u_val = val;
//       f32_wrapper.f_val is 1.23f again
typedef union f32_wrapper_t
{
  u32 u_val;
  f32 f_val;
}f32_wrapper_t;

// @DOC: test / example of how the serialization works
void serialization_test();

// @DOC: initializing, call this before any other calls to serialization, just checks for type size errors
void serialization_init();

// @DOC: serialize u8, aka. unsigned char
void serialization_serialize_u8(u8** buffer, u8 val);
// @DOC: serialize u32, aka. unsigned int
void serialization_serialize_u32(u8** buffer, u32 val);
// @DOC: serialize s32, aka. int
void serialization_serialize_s32(u8** buffer, int val);
// @DOC: serialize f32, aka. float
void serialization_serialize_f32(u8** buffer, f32 val);
// @DOC: serialize math type vec2, aka. float[2]
void serialization_serialize_vec2(u8** buffer, vec2 val);
// @DOC: serialize math type ivec2, aka. int[2]
void serialization_serialize_ivec2(u8** buffer, ivec2 val);
// @DOC: serialize math type vec3, aka. float[3]
void serialization_serialize_vec3(u8** buffer, vec3 val);
// @DOC: serialize string
void serialization_serialize_str(u8** buffer, char* val);

// @DOC: deserialize u8, aka. unsigned char
u8    serialization_deserialize_u8(u8* buffer, u32* offset);
// @DOC: deserialize u32, aka. unsigned int
u32   serialization_deserialize_u32(u8* buffer, u32* offset);
// @DOC: deserialize s32, aka. int
int   serialization_deserialize_s32(u8* buffer, u32* offset);
// @DOC: deserialize f32, aka. float
f32   serialization_deserialize_f32(u8* buffer, u32* offset);
// @DOC: deserialize math type vec2, aka. float[2], gets put in out
void  serialization_deserialize_vec2(u8* buffer, u32* offset, vec2 out);
// @DOC: deserialize math type ivec2, aka. int[2], gets put in out
void  serialization_deserialize_ivec2(u8* buffer, u32* offset, ivec2 out);
// @DOC: deserialize math type vec3, aka. float[3], gets put in out
void  serialization_deserialize_vec3(u8* buffer, u32* offset, vec3 out);
// @DOC: deserialize string
char* serialization_deserialize_str(u8* buffer, u32* offset);

#endif
