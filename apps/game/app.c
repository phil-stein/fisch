#include "game/app.h"
#include "core/program.h"
#include "core/core_data.h"
#include "core/io/input.h"
#include "core/event_sys.h"

#include "stb/stb_ds.h"

#include "global/global_print.h"


app_data_t  app_data_data = APP_DATA_INIT(); 
app_data_t* app_data = &app_data_data;


// 
// @TODO: mui in game builds, also renderer_drect smaller and in bottom left corner
//        - possibly core_data data on window size wrong
//        - maybe bc. fullscreen, but doesnt affect editor
//

int main(void)
{
  // @TODO: @UNSURE: these results are sus
  vec3 a = VEC3_INIT(1);
  vec3 b = VEC3_INIT(2);
  vec3 c; 
  vec3_lerp(a, b, 0.5f, c);
  P_VEC3(c);
  P_VEC3(VEC3_LERP(a, b, 0.5f));
  
  vec3_lerp_f(0, 1, 0.5f, c);
  P_VEC3(c);
  P_VEC3(VEC3_LERP_F(0, 1, 0.5f));
  
  P_LINE();
  
  program_start(1600, 900, "game", WINDOW_FULL, app_init, app_update, app_cleanup, ASSET_PATH);  // WINDOW_FULL
  
  return 0;
}

void app_init()
{
  // @BUGG: shouldnt have to set this
  core_data->phys_act       = true;
  core_data->phys_debug_act = false;
  core_data->scripts_act    = true;
  core_data->is_paused      = false;
}

void app_update()
{
  // toggle wireframe, esc to quit, etc.
  // programm_app_default_logic(core_data);
  
  input_set_cursor_visible(false);

  // toggle wireframe
  if (input_get_key_pressed(KEY_WIREFRAME_TOGGLE))
  {
    core_data->wireframe_mode_enabled = !core_data->wireframe_mode_enabled;
    P_BOOL(core_data->wireframe_mode_enabled);
    // core_data->wireframe_mode_enabled = app_data->wireframe_act;
    // app_data->wireframe_act = !app_data->wireframe_act;
  }

  if (input_get_key_pressed(KEY_TOGGLE_FULLSCREEN))
  {
    window_type type = window_get_mode();
    
    P_WINDOW_TYPE(type); 
    
    // @NOTE: min -> max -> full
    type = type == WINDOW_MIN ? WINDOW_MAX : type == WINDOW_MAX ? WINDOW_FULL : WINDOW_MAX;
    
    P_WINDOW_TYPE(type); 
    P("------------");

    window_set_mode(type);
  }

  if (input_get_key_pressed(KEY_EXIT))
  {
    program_quit();
  }

}

void app_cleanup()
{}

void app_entity_removed_callback(int id)
{
  (void)id;
}


