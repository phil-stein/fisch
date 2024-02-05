#ifndef DATA_TEST_COMP_H
#define DATA_TEST_COMP_H

#include "global/global.h"
#include "core/types/types.h"
#include "core/state.h"

#ifdef __cplusplus
extern "C"
{
#endif


void entity_init(void* this);
void entity_update(void* this, f32 dt);

#ifdef __cplusplus
} // extern C
#endif

#endif  // DATA_TEST_COMP_H
