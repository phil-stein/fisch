#ifndef GAME_TEST_SCRIPTS_H
#define GAME_TEST_SCRIPTS_H

#include "global/global.h"
#include "core/types/types.h"
#include "core/state/state.h"
#include "core/state/state_scripts.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
  int  entity_id; // required
  bool is_dead;   // required
  bool is_active; // required
} controller_script_t;

SCRIPT_DECL(controller_script_t);



#ifdef __cplusplus
} // extern c
#endif

#endif  // SCRIPTS_H
