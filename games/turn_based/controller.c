#include "core/types/entity.h"
#include "math/math_color.h"
#include "math/math_vec3.h"
#include "core/renderer/renderer_extra.h"

#include "turn_based/controller.h"
#include "turn_based/entity_table.h"
#include "turn_based/turn_based.h"

#include "core/core_data.h"
#include "core/io/input.h"
#include "core/camera.h"
#include "core/state/state.h"
#include "core/debug/debug_draw.h"
#include "core/mui/mui.h"

#include "stb/stb_ds.h"



// static u32 sound_jump_idx    = SOUND_INVALID_IDX;

#define TILES_LAYERS_MAX 2
#define TILES_AXIS_MAX   10   // max tiles per axis, i.e. x or z
#define TILES_MAX        TILES_AXIS_MAX * TILES_AXIS_MAX
int tiles[TILES_LAYERS_MAX][TILES_MAX] /* = { { 0, 1 }, { -1; } } */ ; // contains all entity id's for all tiles based on which layer they are in or <= -1

#define CHAR_IDS_MAX 3
static int char_ids[CHAR_IDS_MAX] = { -1 };  // entity_ids of controllable characters
static int selected_char_id = -1;
static int selected_tile_id = -1;

static const vec3 hover_col    = { 1.0f, 0.0f, 0.0f };
static const vec3 selected_col = { 0.0f, 1.0f, 1.0f };

// --- func-decls ---
static void controller_camera_controller();
static void controller_ui();

// void SCRIPT_REGISTER_TRIGGER_CALLBACK_FUNC(controller_script_t)  
// {
// }
// void SCRIPT_REGISTER_COLLISION_CALLBACK_FUNC(controller_script_t)
// {
// }

void controller_init()
{
  // -- cam position --
  vec3_copy(VEC3_XYZ(-7.85f, 18.80f, 7.90f), core_data->cam.pos);
  camera_set_pitch_yaw(-0.89f, 11.85f);

  // sound_jump_idx   = audio_load_clip("woosh_01_01.wav", SOUND_SPATIAL);  
 
  // spawn characters
  char_ids[0] = state_entity_add_from_template( VEC3_XYZ(-2, 1, -10), VEC3(0), VEC3(1), ENTITY_TEMPLATE_ROBOT_CHARACTER_06, false );
  char_ids[1] = state_entity_add_from_template( VEC3_XYZ( 0, 1, -10), VEC3(0), VEC3(1), ENTITY_TEMPLATE_ROBOT_CHARACTER_06, false );
  char_ids[2] = state_entity_add_from_template( VEC3_XYZ( 2, 1, -10), VEC3(0), VEC3(1), ENTITY_TEMPLATE_ROBOT_CHARACTER_06, false );

  // spawn tiles
  const bool center_grid = true;
  const f32 x_center_offs = ((f32)TILES_AXIS_MAX * 2.0f * 0.5f);
  const f32 z_center_offs = ((f32)TILES_AXIS_MAX * 2.0f * 0.5f);
  for ( int x = 0; x < TILES_AXIS_MAX; ++x )
  {
    for ( int z = 0; z < TILES_AXIS_MAX; ++z )
    {
      tiles[0][x + z] = 
      state_entity_add_from_template( 
          center_grid ? 
            VEC3_XYZ( ((f32)x *2.0f) - x_center_offs, 0, ((f32)z *2.0f) - z_center_offs ) : 
            VEC3_XYZ( (f32)x *2.0f, 0, (f32)z *2.0f ), 
          VEC3(0), VEC3(1), 
          ENTITY_TEMPLATE_TILE, true );
      
    }
  }
}
void controller_update()
{
  controller_ui();

  controller_camera_controller();

  int mouse_pick_id = renderer_extra_mouse_position_mouse_pick_id();

  // -- hovering char -- 
  for (int i = 0; i < CHAR_IDS_MAX; ++i)
  {
    entity_t* e = state_entity_get( char_ids[i] );

    if ( input_get_mouse_pressed( MOUSE_LEFT ) )
    {
      selected_char_id = -1;
    }

    if ( selected_char_id == e->id )
    {
      vec3_copy( (f32*)selected_col, e->tint );
    }
    else if ( mouse_pick_id == e->id )
    {
      vec3_copy( (f32*)hover_col, e->tint );
      if ( input_get_mouse_down( MOUSE_LEFT ) )
      {
        selected_char_id = e->id;
      }
    }
    else { vec3_copy( VEC3(1.0f), e->tint ); }
  }

  // -- hovering tile -- 
  for (int i = 0; i < game_data->grid_ids_pos; ++i)
  {
    entity_t* tile = state_entity_get( game_data->grid_ids[i] );

    if ( mouse_pick_id  == tile->id )
    {
      vec3_copy( (f32*)hover_col, tile->tint );
      if ( input_get_mouse_down( MOUSE_LEFT ) )
      {
        selected_tile_id = tile->id;
      }
      if ( selected_char_id >= 0 )
      {
        entity_t* c = state_entity_get( selected_char_id );
        debug_draw_line( c->pos, tile->pos, RGBF(0.0f, 1.0f, 1.0f) );
      }
    }
    else { vec3_copy( VEC3(1.0f), tile->tint ); }
  }

  core_data->outline_id = selected_char_id;
}
void controller_cleanup()
{
}

static void controller_camera_controller()
{
  // -- camera-controller --
  // vec3_copy(VEC3_XYZ(-7.85f, 18.80f, 7.90f), core_data->cam.pos);
  camera_set_pitch_yaw(-0.89f, 11.85f);

  const f32 speed = 10.0f * core_data->delta_t;

  vec3 front, back, left, right;
  
  camera_get_front(front);
  front[1] = 0.0f;
  vec3_normalize(front, front);
  vec3_mul_f(front, speed, front);
  vec3_mul_f(front, -1.0f, back);

  camera_get_right(left);
  vec3_mul_f(left, speed, left);
  vec3_mul_f(left, -1.0f, right);

  if ( input_get_key_down(KEY_W) ) { vec3_add(core_data->cam.pos, front, core_data->cam.pos); }
  if ( input_get_key_down(KEY_S) ) { vec3_add(core_data->cam.pos, back,  core_data->cam.pos); }
  if ( input_get_key_down(KEY_A) ) { vec3_add(core_data->cam.pos, left,  core_data->cam.pos); }
  if ( input_get_key_down(KEY_D) ) { vec3_add(core_data->cam.pos, right, core_data->cam.pos); }
  if ( input_get_key_down(KEY_Q) ) { vec3_add(core_data->cam.pos, VEC3_Y( speed), core_data->cam.pos); }
  if ( input_get_key_down(KEY_E) ) { vec3_add(core_data->cam.pos, VEC3_Y(-speed), core_data->cam.pos); }
}

static void controller_ui()
{
  // -- fps counter --
  mui_textf(VEC2_XY(0.8f, 0.8f), MUI_CENTER | MUI_MIDDLE, "fps: %.1f", core_data->cur_fps); 
  
  mui_textf(VEC2_XY(0.8f, 0.7f), MUI_CENTER | MUI_MIDDLE, "selected: %d ", selected_char_id); 
  
  for ( int i = 0; i < TILES_MAX; ++i )
  {
    // mui_textf_3d();
  }
}
