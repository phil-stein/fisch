# -- serialization --
simple c serialization library for base types and my math libraries types

using:      [stb_image & stb_ds](https://github.com/nothings/stb)<br>
and my own: [math-library](https://github.com/phil-stein/math) & [global-defines](https://github.com/phil-stein/global)

```c
void serialization_test()
{
  u8* buffer = NULL;
 
  u8  b0 = 128;
  serialization_serialize_u8(&buffer, b0);
  u32 u0 = 1234;
  serialization_serialize_u32(&buffer, u0);
  int s0 = -1234;
  serialization_serialize_s32(&buffer, s0);
  float f0 = 123.45678f;
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
  
  arrfree(buffer);
}
```
