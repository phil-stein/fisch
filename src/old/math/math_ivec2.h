#ifndef MATH_IVEC2_MATH_H
#define MATH_IVEC2_MATH_H

#ifdef __cplusplus
extern "C"
{
#endif


#include "math_inc.h"

typedef int ivec2[2];   // v[0]: x, v[1]: y

// ---- ivec2 creation ----

#define IVEC2_INIT(f)		        { f, f }
#define IVEC2_XY_INIT(x, y) 	  { x, y }
#define IVEC2_X_INIT(x)		      { x, 0 }
#define IVEC2_Y_INIT(y)		      { 0, y }

#define IVEC2(f)			      (ivec2){ f, f }
#define IVEC2_XY(x, y) 		  (ivec2){ x, y }
#define IVEC2_X(x)		      (ivec2){ x, 0 }
#define IVEC2_Y(y)		      (ivec2){ 0, y }


// ---- print util ----
#define P_IVEC2(v) 	    printf("|%s| x: %.2f, y: %.2f\n", #v, v[0], v[1])
#define P_IL_IVEC2(v) 	printf("|%s| x: %.2f, y: %.2f", #v, v[0], v[1])


// ---- ivec2 ----


M_INLINE void ivec2_add(ivec2 a, ivec2 b, ivec2 out)
{
	out[0] = a[0] + b[0];
	out[1] = a[1] + b[1];
}
M_INLINE void ivec2_add_i(ivec2 a, int i, ivec2 out)
{
	out[0] = a[0] + i;
	out[1] = a[1] + i;
}

M_INLINE void ivec2_sub(ivec2 a, ivec2 b, ivec2 out)
{
	out[0] = a[0] - b[0];
	out[1] = a[1] - b[1];
}
M_INLINE void ivec2_sub_i(ivec2 a, int i, ivec2 out)
{
	out[0] = a[0] - i;
	out[1] = a[1] - i;
}

M_INLINE void ivec2_mul(ivec2 a, ivec2 b, ivec2 out)
{
	out[0] = a[0] * b[0];
	out[1] = a[1] * b[1];
}
M_INLINE void ivec2_mul_i(ivec2 a, int i, ivec2 out)
{
	out[0] = a[0] * i;
	out[1] = a[1] * i;
}

M_INLINE void ivec2_div(ivec2 a, ivec2 b, ivec2 out)
{
	out[0] = a[0] / b[0];
	out[1] = a[1] / b[1];
}
M_INLINE void ivec2_div_i(ivec2 a, int i, ivec2 out)
{
	out[0] = a[0] / i;
	out[1] = a[1] / i;
}

M_INLINE int ivec2_dot(ivec2 a, ivec2 b)
{
	return (a[0] * b[0]) + (a[1] * b[1]);
}

M_INLINE float ivec2_magnitude(ivec2 a) // length of vec
{
	return sqrtf((a[0] * a[0]) + (a[1] * a[1]));
}

M_INLINE void ivec2_copy(ivec2 a, ivec2 out)
{
	out[0] = a[0]; 
	out[1] = a[1];
}

M_INLINE void ivec2_negate(ivec2 a, ivec2 out)
{
	out[0] = -a[0]; 
	out[1] = -a[1];
}

M_INLINE void ivec2_normalize(ivec2 a, ivec2 out)
{
	ivec2_copy(a, out);
	float mag = ivec2_magnitude(out);
	ivec2_div_i(out, (int)mag, out);
}

M_INLINE float ivec2_distance(ivec2 a, ivec2 b)
{
	ivec2 d;
	ivec2_sub(a, b, d);
	return ivec2_magnitude(d); 
}

M_INLINE void ivec2_clamp(ivec2 a, ivec2 min, ivec2 max, ivec2 out)
{
	out[0] = CLAMP(a[0], min[0], max[0]);
	out[1] = CLAMP(a[1], min[1], max[1]);
}

M_INLINE void ivec2_clamp_i(ivec2 a, int min, int max, ivec2 out)
{
	out[0] = CLAMP(a[0], min, max);
	out[1] = CLAMP(a[1], min, max);
}

M_INLINE void ivec2_abs(ivec2 a, ivec2 out)
{
	out[0] = abs(a[0]);
	out[1] = abs(a[1]);
}

// ---- conditionals ----

M_INLINE bool ivec2_equal(ivec2 a, ivec2 b)
{
  return a[0] == b[0] && a[1] == b[1];
}
M_INLINE bool ivec2_not_equal(ivec2 a, ivec2 b)
{
  return !ivec2_equal(a, b);
}
M_INLINE bool ivec2_greater(ivec2 a, ivec2 b)
{
  return a[0] > b[0] && a[1] > b[1];
}
M_INLINE bool ivec2_less(ivec2 a, ivec2 b)
{
  return a[0] < b[0] && a[1] < b[1];
}
M_INLINE bool ivec2_greater_eq(ivec2 a, ivec2 b)
{
  return a[0] >= b[0] && a[1] >= b[1];
}
M_INLINE bool ivec2_less_eq(ivec2 a, ivec2 b)
{
  return a[0] <= b[0] && a[1] <= b[1];
}

#ifdef __cplusplus
} // extern C
#endif

#endif
