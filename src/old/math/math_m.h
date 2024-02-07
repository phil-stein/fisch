#ifndef MATH_M_MATH_H
#define MATH_M_MATH_H

#ifdef __cplusplus
extern "C"
{
#endif


#include "math_inc.h"

#ifndef M_PI
	#define M_PI 	3.14159265358979323846264338327950288
#endif
#ifndef M_PI_F
	#define M_PI_F	(float)M_PI
#endif

// if less than b than b else a
#ifndef MIN
	#define MIN(a, b)	((a) < (b) ? (a) : (b))
#endif

// if more than b than b else a
#ifndef MAX
	#define MAX(a, b)	((a) > (b) ? (a) : (b))
#endif

// if less than min, min, if more than max, else a
#ifndef CLAMP
	#define CLAMP(a, _min, _max)	MAX(_min, MIN(_max, a))
#endif

#define INT_ABS(v)      abs(v)
#define FLOAT_ABS(v)    fabsf(v)
#define DOUBLE_ABS(v)   absf(v)
#define INT_SQRT(v)     sqrtl(v)
#define FLOAT_SQRT(v)   sqrtf(v)
#define DOUBLE_SQRT(v)  sqrt(v)
#define F32_ABS(v)      FLOAT_ABS(v)
#define F64_ABS(v)      DOUBLE_ABS(v)
#define F32_SQRT(v)     FLOAT_SQRT(v)
#define F64_SQRT(v)     DOUBLE_SQRT(v)

// @DOC: convert from degree to radians
M_INLINE void m_deg_to_rad(float* deg)
{
	*deg = *deg * M_PI_F / 180.0f;
}

// @DOC: convert from radians to degree
M_INLINE void m_rad_to_deg(float* rad)
{
	*rad = *rad * 180.0f / M_PI_F;
}

// taken from: http://www.corykoseck.com/2018/08/29/programming-in-c-lerp/
// @DOC: linear interpolation, float between start and end, given percentage
M_INLINE float m_lerp(float start, float end, float percentage)
{
  return CLAMP(start + percentage * (end - start), end, start);
}
// @TODO: slerp

#ifdef __cplusplus
} // extern C
#endif

#endif
