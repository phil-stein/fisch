#include "core/state/state_scripts.h"
#include "core/types/entity.h"
#include "global/global_print.h"
#include "math/math_m.h"
#include "math/math_mat4.h"
#include "math/math_vec3.h"

#include "turn_based/scripts.h"
#include "turn_based/entity_tags.h"
#include "turn_based/entity_table.h"
#include "turn_based/turn_based.h"

#include "core/templates/entity_template.h"
#include "core/core_data.h"
#include "core/io/input.h"
#include "core/io/save_sys/save_sys.h"
#include "core/camera.h"
#include "core/state/state.h"
#include "core/io/assetm.h"
#include "core/debug/debug_draw.h"
#include "core/mui/mui.h"

#include "math/math_inc.h"
#include "phys/phys_ray.h"  // raycasting

#include "stb/stb_ds.h"



static u32 sound_jump_idx    = SOUND_INVALID_IDX;

// --- func-decls ---
static void script_turn_based_cam(entity_t* this);
static void script_turn_based_ui(entity_t* this, controller_script_t* script);

// void SCRIPT_REGISTER_TRIGGER_CALLBACK_FUNC(controller_script_t)  
// {
// }
// void SCRIPT_REGISTER_COLLISION_CALLBACK_FUNC(controller_script_t)
// {
// }

void SCRIPT_INIT(controller_script_t)
{
  // entity_t* this = 
  state_entity_get(script->entity_id);
  
  sound_jump_idx   = audio_load_clip("woosh_01_01.wav", SOUND_SPATIAL);  
}
void SCRIPT_CLEANUP(controller_script_t)
{
  (void)script;
  // audio ...
}
void SCRIPT_UPDATE(controller_script_t)
{
  entity_t* this = state_entity_get(script->entity_id);

  script_turn_based_ui(this, script);

  script_turn_based_cam(this);
}

static void script_turn_based_cam(entity_t* this)
{
  (void)this;
}

static void script_turn_based_ui(entity_t* this, controller_script_t* script)
{
  (void)this;
  (void)script;
}
