#ifndef MATH_RAND_MATH_H
#define MATH_RAND_MATH_H

#ifdef __cplusplus
extern "C"
{
#endif


#include "math_inc.h"
#include <stdlib.h>   // rand & srand


// #define P_U64(u)   printf("|%s| %llu\n", #u, u)
#define P_U64(u)   printf("|%s| %"PRId64"\n", #u, u)

// @DOC: seed rand, call before other calls to rand
M_INLINE void rand_seed(int seed)
{
	srand(seed);
}
M_INLINE int rand_max()
{
  return RAND_MAX;
}

M_INLINE int rand_int()
{
  return rand();
}
M_INLINE f32 rand_f32()
{
  return (f32)rand() / (f32)RAND_MAX;
}

M_INLINE int rand_int_range(int min, int max)
{
  return (rand_f32() * (max - min)) + min;
}

M_INLINE bool rand_bool()
{
  return rand_int_range(0, 2) == 1;
}


M_INLINE uint64_t rand_u64()
{
  uint64_t g = 0;
  g += rand_int();
  g *= rand_int();
  g *= rand_int();
  g *= rand_int();
  g *= 2;
  return g;
}

#ifdef __cplusplus
} // extern C
#endif

#endif
