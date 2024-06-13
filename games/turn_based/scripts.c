#include "turn_based/scripts.h"
#include "turn_based/entity_tags.h"
#include "turn_based/entity_table.h"

#include "core/templates/entity_template.h"
#include "core/core_data.h"
#include "core/io/input.h"
#include "core/io/save_sys/save_sys.h"
#include "core/camera.h"
#include "core/state/state.h"
#include "core/debug/debug_draw.h"

#include "phys/phys_ray.h"  // raycasting

// includes bool SCRIPT_REMOVE_FUNC_GENERIC_NAME(u32 uid);
#include "games.h" // needed to include __script_update__(), etc. 

#include "stb/stb_ds.h"
#include "turn_based/turn_based.h"


// -- register --
// SCRIPT_REGISTER(controller_script_t, 0)
SCRIPT_REGISTER(tile_script_t, 0)

#if defined(_MSC_VER) & !defined(__clang__)
// -- clear arrays --
SCRIPTS_CLEAR_FUNC_START()
  SCRIPTS_CLEAR_FUNC_SCRIPT(controller_script_t);
SCRIPTS_CLEAR_FUNC_END()
// -- generic remove func --
SCRIPT_REMOVE_FUNC_GENERIC_START()
  SCRIPT_REMOVE_FUNC_GENERIC_SCRIPT(controller_script_t);
SCRIPT_REMOVE_FUNC_GENERIC_END()
// -- generic get func --
SCRIPT_GET_FUNC_GENERIC_START()
  SCRIPT_GET_FUNC_GENERIC_SCRIPT(controller_script_t);
SCRIPT_GET_FUNC_GENERIC_END()
// -- get type str func --
SCRIPT_GET_TYPE_STR_FUNC_START()
  SCRIPT_GET_TYPE_STR_FUNC_SCRIPT(controller_script_t);
SCRIPT_GET_TYPE_STR_FUNC_END()
INLINE void SCRIPT_RUN_UPDATE_ALL()                           
{ 
  SCRIPT_RUN_UPDATE(controller_script_t);
}                 
                                                              
INLINE void SCRIPT_RUN_CLEANUP_ALL()                          
{ 
  SCRIPT_RUN_CLEANUP(controller_script_t);
} 

#else // _MSC_VER && !__clang__
SCRIPT_FUNCS(
  // controller_script_t,
  tile_script_t,
  )
#endif // _MSC_VER && !__clang__



void __scripts_init__() {}

void __scripts_update__()
{
  if (!game_data->run_scripts) { return; }

  SCRIPT_RUN_UPDATE_ALL();
}

void __scripts_cleanup__()
{
  SCRIPT_RUN_CLEANUP_ALL();
}

