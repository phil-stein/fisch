#ifndef SCRIPTS_H
#define SCRIPTS_H

#include "global/global.h"
#include "core/types/types.h"
#include "core/ecs/ecs.h"

#ifdef __cplusplus
extern "C" {
#endif



typedef struct
{
  u32 entity_id;  // required
  char* str; 
}test_script_t;

typedef struct
{
  u32 entity_id;  // required
}player_controller_script_t;

// typedef enum
// {
//   SCRIPT_TEST,
// }script_type;

SCRIPT_DECL(test_script_t);
SCRIPT_DECL(player_controller_script_t);

void scripts_init();
void scripts_update();


#ifdef __cplusplus
} // extern c
#endif

#endif  // SCRIPTS_H
