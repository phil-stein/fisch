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
  bool is_dead;   // required
  f32 alive_t;
}projectile_script_t;

typedef struct
{
  u32 entity_id;  // required
  bool is_dead;   // required
}player_controller_script_t;

typedef struct
{
  u32 entity_id;  // required
  bool is_dead;   // required
}fps_controller_script_t;


SCRIPT_DECL(projectile_script_t);
SCRIPT_DECL(player_controller_script_t);
SCRIPT_DECL(fps_controller_script_t);


void scripts_update();


#ifdef __cplusplus
} // extern c
#endif

#endif  // SCRIPTS_H
