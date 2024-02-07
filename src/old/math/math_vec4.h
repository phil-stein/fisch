#ifndef MATH_VEC4_MATH_H
#define MATH_VEC4_MATH_H

#ifdef __cplusplus
extern "C"
{
#endif



#include "math_inc.h"

typedef float	vec4[4];	// v[0]: x, v[1]: y, v[2]: z, v[3]: w

typedef int ivec4[4];	  // v[0]: x, v[1]: y, v[2]: z, v[3]: w


// ---- vec4 creation ----

#define VEC4_INIT(f)		      { f, f, f, f }
#define VEC4_XYZW_INIT(x, y, z, w)    { x, y, z, w }
#define VEC4_X_INIT(x)		      { x, 0, 0, 0 }
#define VEC4_Y_INIT(y)		      { 0, y, 0, 0 }
#define VEC4_Z_INIT(z)		      { 0, 0, z, 0 }
#define VEC4_W_INIT(w)		      { 0, 0, 0, w }

#define VEC4(f)			(vec4){ f, f, f, f }
#define VEC4_XYZW(x, y, z, w)	(vec4){ x, y, z, w }
#define VEC4_X(x)		(vec4){ x, 0, 0, 0 }
#define VEC4_Y(y)		(vec4){ 0, y, 0, 0 }
#define VEC4_Z(z)		(vec4){ 0, 0, z, 0 }
#define VEC4_W(w)		(vec4){ 0, 0, 0, w }

// ---- print utils ----
#define P_VEC4(v) 	printf("|%s| x: %.2f, y: %.2f, z: %.2f, w: %.f\n", #v, v[0], v[1], v[2], v[3])
#define P_IL_VEC4(v) 	printf("|%s| x: %.2f, y: %.2f, z: %.2f, w: %.f", #v, v[0], v[1], v[2], v[3])


// ---- vec4 ----

M_INLINE void vec4_add(vec4 a, vec4 b, vec4 out)
{
	out[0] = a[0] + b[0];
	out[1] = a[1] + b[1];
	out[2] = a[2] + b[2];
	out[3] = a[3] + b[3];
}
M_INLINE void vec4_add_f(vec4 a, float f, vec4 out)
{
	out[0] = a[0] + f;
	out[1] = a[1] + f;
	out[2] = a[2] + f;
	out[3] = a[3] + f;
}

M_INLINE void vec4_sub(vec4 a, vec4 b, vec4 out)
{
	out[0] = a[0] - b[0];
	out[1] = a[1] - b[1];
	out[2] = a[2] - b[2];
	out[3] = a[3] - b[3];
}
M_INLINE void vec4_sub_f(vec4 a, float f, vec4 out)
{
	out[0] = a[0] - f;
	out[1] = a[1] - f;
	out[2] = a[2] - f;
	out[3] = a[3] - f;
}

M_INLINE void vec4_mul(vec4 a, vec3 b, vec4 out)
{
	out[0] = a[0] * b[0];
	out[1] = a[1] * b[1];
	out[2] = a[2] * b[2];
	out[3] = a[3] * b[4];
}
M_INLINE void vec4_mul_f(vec4 a, float f, vec4 out)
{
	out[0] = a[0] * f;
	out[1] = a[1] * f;
	out[2] = a[2] * f;
	out[3] = a[3] * f;
}

M_INLINE float vec4_dot(vec4 a, vec4 b)
{
	return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]) + (a[3] * b[3]);
}

M_INLINE float vec4_magnitude(vec4 a) // length of vec
{
	return sqrtf((a[0] * a[0]) + (a[1] * a[1]) + (a[2] * a[2]) + (a[3] * a[3]));
}

M_INLINE void vec4_copy(vec4 a, vec4 out)
{
	out[0] = a[0];
	out[1] = a[1];
	out[2] = a[2];
	out[3] = a[3];
}

M_INLINE void vec4_negate(vec4 a, vec4 out)
{
	out[0] = -a[0];
	out[1] = -a[1];
	out[2] = -a[2];
	out[3] = -a[3];
}

M_INLINE float vec4_distance(vec4 a, vec4 b)
{
	vec4 d;
	vec4_sub(a, b, d);
	return vec4_magnitude(d); 
}

M_INLINE void vec4_clamp(vec4 a, vec4 min, vec4 max, vec4 out)
{
	out[0] = CLAMP(a[0], min[0], max[0]);
	out[1] = CLAMP(a[1], min[1], max[1]);
	out[2] = CLAMP(a[2], min[2], max[2]);
	out[3] = CLAMP(a[3], min[3], max[3]);
}

M_INLINE void vec4_clamp_f(vec4 a, vec4 min, vec4 max, vec4 out)
{
	out[0] = CLAMP(a[0], min[0], max[0]);
	out[1] = CLAMP(a[1], min[1], max[1]);
	out[2] = CLAMP(a[2], min[2], max[2]);
	out[3] = CLAMP(a[3], min[3], max[3]);
}

M_INLINE void vec4_abs(vec4 a, vec4 out)
{
	out[0] = fabsf(a[0]);
	out[1] = fabsf(a[1]);
	out[2] = fabsf(a[2]);
	out[3] = fabsf(a[3]);
}


// ---- interpolation ----

M_INLINE void vec4_lerp(vec4 start, vec4 end, f32 percentage, vec4 out)
{
  out[0] = m_lerp(start[0], end[0], percentage);
  out[1] = m_lerp(start[1], end[1], percentage);
  out[2] = m_lerp(start[2], end[2], percentage);
  out[3] = m_lerp(start[3], end[3], percentage);
}
M_INLINE void vec4_lerp_f(f32 start, f32 end, f32 percentage, vec4 out)
{
  out[0] = m_lerp(start, end, percentage);
  out[1] = m_lerp(start, end, percentage);
  out[2] = m_lerp(start, end, percentage);
  out[3] = m_lerp(start, end, percentage);
}


// ---- conditionals ----

M_INLINE bool vec4_equal(vec4 a, vec4 b)
{
  return a[0] == b[0] && a[1] == b[1] && 
         a[2] == b[2] && a[3] == b[3];
}
M_INLINE bool vec4_not_equal(vec4 a, vec4 b)
{
  return !vec4_equal(a, b);
}
M_INLINE bool vec4_greater(vec4 a, vec4 b)
{
  return a[0] > b[0] && a[1] > b[1] && 
         a[2] > b[2] && a[3] > b[3];
}
M_INLINE bool vec4_less(vec4 a, vec4 b)
{
  return a[0] < b[0] && a[1] < b[1] && 
         a[2] < b[2] && a[3] < b[3];
}
M_INLINE bool vec4_greater_eq(vec4 a, vec4 b)
{
  return a[0] >= b[0] && a[1] >= b[1] && 
         a[2] >= b[2] && a[3] >= b[3];
}
M_INLINE bool vec4_less_eq(vec4 a, vec4 b)
{
  return a[0] <= b[0] && a[1] <= b[1] && 
         a[2] >= b[2] && a[3] <= b[3];
}

#ifdef __cplusplus
} // extern C
#endif

#endif
