#include "serialization/serialization.h"

#include "stb/stb_ds.h"


#define STR_BUF_MAX 256
char str_buf[STR_BUF_MAX] = "";


void serialization_test()
{
  u8* buffer = NULL;
 
  u8  b0 = 128;
  serialization_serialize_u8(&buffer, b0);
  u32 u0 = 1234;
  serialization_serialize_u32(&buffer, u0);
  int s0 = -1234;
  serialization_serialize_s32(&buffer, s0);
  f32 f0 = 123.45678f;
  serialization_serialize_f32(&buffer, f0);
  vec2 v2_0 = { 123.45f, 678.9f };
  serialization_serialize_vec2(&buffer, v2_0);
  vec3 v3_0 = { 123.45f, 678.9f, 1234567.89f };
  serialization_serialize_vec3(&buffer, v3_0);
  char str0[] = "hello, world! this is a test :)";
  serialization_serialize_str(&buffer, str0);

  P_INT((int)arrlen(buffer));

  u32 offset = 0;

  u8 b0d = serialization_deserialize_u8(buffer, &offset);
  P_INT(b0d); ASSERT(b0d == b0);
  u32 u0d = serialization_deserialize_u32(buffer, &offset);
  P_U32(u0d); ASSERT(u0d == u0);
  int s0d = serialization_deserialize_s32(buffer, &offset);
  P_INT(s0d); ASSERT(s0d == s0);
  f32 f0d = serialization_deserialize_f32(buffer, &offset);
  P_F32(f0d); ASSERT(f0d == f0);
  vec2 v2_0d; serialization_deserialize_vec2(buffer, &offset, v2_0d);
  P_VEC2(v2_0d); ASSERT(vec2_equal(v2_0d, v2_0)); 
  vec3 v3_0d; serialization_deserialize_vec3(buffer, &offset, v3_0d);
  P_VEC3(v3_0d); ASSERT(vec2_equal(v3_0d, v3_0)); 
  P_STR(serialization_deserialize_str(buffer, &offset));
  

  ARRFREE(buffer);

}

void serialization_init()
{
  ERR_CHECK(sizeof(int) == 4, "serialization requires int to be 4 bytes in size");
  ERR_CHECK(sizeof(u32) == 4, "serialization requires u32 / unsigned int to be 4 bytes in size");
  ERR_CHECK(sizeof(f32) == 4, "serialization requires f32 / float to be 4 bytes in size");
}


// ---- base types ----

void serialization_serialize_u8(u8** buffer, u8 val)
{
  arrput(*buffer, (u8)(val));
}
u8 serialization_deserialize_u8(u8* buffer, u32* offset)
{
  u8 rtn = buffer[*offset];
  *offset += 1;
  return rtn;
}

void serialization_serialize_u32(u8** buffer, u32 val)
{
  arrput(*buffer, (u8)(val >> 24));
  arrput(*buffer, (u8)(val >> 16));
  arrput(*buffer, (u8)(val >> 8));
  arrput(*buffer, (u8)(val));
}
u32 serialization_deserialize_u32(u8* buffer, u32* offset)
{
  u8* data = buffer + *offset;
  u32 rtn  = ((u32)data[3]) + ((u32)data[2] << 8) + ((u32)data[1] << 16) + ((u32)data[0] << 24);
  *offset += 4;
  return rtn;
}

void serialization_serialize_s32(u8** buffer, int val)
{
  arrput(*buffer, val >> 24);
  arrput(*buffer, val >> 16);
  arrput(*buffer, val >> 8);
  arrput(*buffer, val); 
}
int serialization_deserialize_s32(u8* buffer, u32* offset)
{
  u8* data = buffer + *offset;
  int rtn  = ((int)data[3]) + ((int)data[2] << 8) + ((int)data[1] << 16) + ((int)data[0] << 24);
  
  *offset += 4;
  return rtn;
}

void serialization_serialize_f32(u8** buffer, f32 val)
{
  f32_wrapper_t value;
  value.f_val = val;
  arrput(*buffer, value.u_val >> 24);
  arrput(*buffer, value.u_val >> 16);
  arrput(*buffer, value.u_val >> 8);
  arrput(*buffer, value.u_val);  
}
f32 serialization_deserialize_f32(u8* buffer, u32* offset)
{
  u8* data = buffer + *offset;
  f32_wrapper_t value;
  value.u_val = ((u32)data[3]) + ((u32)data[2] << 8) + ((u32)data[1] << 16) + ((u32)data[0] << 24);
  *offset += 4;
  return value.f_val;
}

void serialization_serialize_vec2(u8** buffer, vec2 val)
{
  serialization_serialize_f32(buffer, val[0]);
  serialization_serialize_f32(buffer, val[1]);
}
void serialization_deserialize_vec2(u8* buffer, u32* offset, vec2 out)
{
  out[0] = serialization_deserialize_f32(buffer, offset);
  out[1] = serialization_deserialize_f32(buffer, offset);
}
void serialization_serialize_ivec2(u8** buffer, ivec2 val)
{
  serialization_serialize_s32(buffer, val[0]);
  serialization_serialize_s32(buffer, val[1]);
}
void serialization_deserialize_ivec2(u8* buffer, u32* offset, ivec2 out)
{
  out[0] = serialization_deserialize_s32(buffer, offset);
  out[1] = serialization_deserialize_s32(buffer, offset);
}

void serialization_serialize_vec3(u8** buffer, vec3 val)
{
  serialization_serialize_f32(buffer, val[0]);
  serialization_serialize_f32(buffer, val[1]);
  serialization_serialize_f32(buffer, val[2]);
}
void serialization_deserialize_vec3(u8* buffer, u32* offset, vec3 out)
{
  out[0] = serialization_deserialize_f32(buffer, offset);
  out[1] = serialization_deserialize_f32(buffer, offset);
  out[2] = serialization_deserialize_f32(buffer, offset);
}

void serialization_serialize_str(u8** buffer, char* val)
{
  u32 len = strlen(val);
  // P_U32(len);
  serialization_serialize_u32(buffer, len);

  for (int i = 0; i < len; ++i)
  {
    serialization_serialize_u8(buffer, val[i]); 
  }
}
// return needs to be copies as it gets overwritten next call
char* serialization_deserialize_str(u8* buffer, u32* offset)
{
  u32 len = serialization_deserialize_u32(buffer, offset);
  // P_U32(len);
  for (int i = 0; i < len; ++i)
  {
    str_buf[i] = serialization_deserialize_u8(buffer, offset); 
  }
  str_buf[len] = '\0';
  return str_buf;
}

