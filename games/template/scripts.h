#ifndef TEMPLATE_GAME_SCRIPTS_H
#define TEMPLATE_GAME_SCRIPTS_H

#include "global/global.h"
#include "core/types/types.h"
#include "core/state/state.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
  int  entity_id; // required
  bool is_dead;   // required
  f32  alive_t;
} test_script_t;
#define TEST_SCRIPT_T_INIT .alive_t = 2.0f


SCRIPT_DECL(test_script_t);


void scripts_update();
void scripts_cleanup();


#ifdef __cplusplus
} // extern c
#endif

#endif  // SCRIPTS_H
