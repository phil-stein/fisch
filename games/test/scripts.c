#include "test/scripts.h"

#include "stb/stb_ds.h"


SCRIPT_REGISTER(test_script_t);

void scripts_update()
{
  SCRIPT_RUN(test_script_t);
}



void SCRIPT_FUNC(test_script_t)
{
  entity_t* e = ecs_entity_get(script->entity_id);
  // PF("called test comp: %d\n", e->id);
}
