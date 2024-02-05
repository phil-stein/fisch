#ifndef MATH_MATH_TEST_H
#define MATH_MATH_TEST_H


#ifdef __cplusplus
extern "C"
{
#endif


#include "math_inc.h"

M_INLINE void math_test()
{
  vec3 a = VEC3_INIT(1);
  vec3 b = VEC3_INIT(2);
  vec3 c; 
  vec3_add(a, b, c);
  P_VEC3(c);
  P_VEC3(VEC3_ADD(a, b));
  vec3 init_0 = VEC3_INIT_ADD(a, b);
  P_VEC3(init_0);
  vec3_add_f(a, 10, c);
  P_VEC3(c);
  P_VEC3(VEC3_ADD_F(a, 10));
  vec3 init_1 = VEC3_INIT_ADD_F(a, 10);
  P_VEC3(init_1);
  
  vec3_sub(a, b, c);
  P_VEC3(c);
  P_VEC3(VEC3_SUB(a, b));
  vec3 init_2 = VEC3_INIT_SUB(a, b);
  P_VEC3(init_2);
  vec3_sub_f(a, 10, c);
  P_VEC3(c);
  P_VEC3(VEC3_SUB_F(a, 10));
  vec3 init_3 = VEC3_INIT_SUB_F(a, 10);
  P_VEC3(init_3);


  vec3_mul(a, b, c);
  P_VEC3(c);
  P_VEC3(VEC3_MUL(a, b));
  vec3 init_4 = VEC3_INIT_MUL(a, b);
  P_VEC3(init_4);
  vec3_mul_f(a, 10, c);
  P_VEC3(c);
  P_VEC3(VEC3_MUL_F(a, 10));
  vec3 init_5 = VEC3_INIT_MUL_F(a, 10);
  P_VEC3(init_5);

  vec3_div(a, b, c);
  P_VEC3(c);
  P_VEC3(VEC3_DIV(a, b));
  vec3 init_6 = VEC3_INIT_DIV(a, b);
  P_VEC3(init_6);
  vec3_div_f(a, 10, c);
  P_VEC3(c);
  P_VEC3(VEC3_DIV_F(a, 10));
  vec3 init_7 = VEC3_INIT_DIV_F(a, 10);
  P_VEC3(init_7);
  
  vec3_cross(a, b, c);
  P_VEC3(c);
  P_VEC3(VEC3_CROSS(a, b));
  vec3 init_8 = VEC3_INIT_CROSS(a, b);
  P_VEC3(init_8);
  
  vec3_negate(a, c);
  P_VEC3(c);
  P_VEC3(VEC3_NEGATE(a));
  vec3 init_9 = VEC3_INIT_NEGATE(a);
  P_VEC3(init_9);
  
  // vec3_normalize(a, c);
  // P_VEC3(c);
  // P_VEC3(VEC3_NORMALIZE(a));
  // vec3_normalize(VEC3(0), c);
  // P_VEC3(c);
  // P_VEC3(VEC3_NORMALIZE( VEC3(0) ));
  // vec3 init_10 = VEC3_INIT_NORMALIZE(a);
  // P_VEC3(init_10);

  vec3_min(a, b, c);
  P_VEC3(c);
  P_VEC3(VEC3_MIN(a, b));
  vec3 init_11 = VEC3_INIT_MIN(a, b);
  P_VEC3(init_11);
  vec3_min_f(a, 1.0f, c);
  P_VEC3(c);
  P_VEC3(VEC3_MIN_F(a, 1.0f));
  vec3 init_12 = VEC3_INIT_MIN_F(a, 1.0f);
  P_VEC3(init_12);
  
  vec3_max(a, b, c);
  P_VEC3(c);
  P_VEC3(VEC3_MAX(a, b));
  vec3 init_13 = VEC3_INIT_MAX(a, b);
  P_VEC3(init_13);
  vec3_max_f(a, 1.0f, c);
  P_VEC3(c);
  P_VEC3(VEC3_MAX_F(a, 1.0f));
  vec3 init_14 = VEC3_INIT_MAX_F(a, 1.0f);
  P_VEC3(init_14);
  
  vec3_clamp(a, VEC3(0), VEC3(1), c);
  P_VEC3(c);
  P_VEC3(VEC3_CLAMP(a, VEC3(0), VEC3(1)));
  vec3 init_15 = VEC3_INIT_CLAMP(a, VEC3(0), VEC3(1));
  P_VEC3(init_15);
  vec3_clamp_f(a, 0, 1, c);
  P_VEC3(c);
  P_VEC3(VEC3_CLAMP_F(a, 0, 1));
  vec3 init_16 = VEC3_INIT_CLAMP_F(a, 0, 1);
  P_VEC3(init_16);
 
  vec3_abs(a, c);
  P_VEC3(c);
  P_VEC3(VEC3_ABS(a));
  vec3 init_17 = VEC3_INIT_ABS(a);
  P_VEC3(init_17);

  P_LINE();
  
  P_VEC3(a);
  vec3 init_copy = VEC3_INIT_COPY(a);
  P_VEC3(init_copy);
}

#ifdef __cplusplus
} // extern C
#endif

#endif // MATH_MATH_TESTS_H
