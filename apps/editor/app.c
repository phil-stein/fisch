#include "editor/app.h"
#include "core/window.h"
#include "editor/gui/gui.h"
#include "editor/gizmo.h"
#ifdef TERRAIN_ADDON 
#include "editor/terrain_edit.h"
#endif  // TERRAIN_ADDON 
#include "editor/stylesheet.h"
#include "editor/operation.h"
#include "editor/editor_save.h"
#include "core/program.h"
#include "core/core_data.h"
#include "core/io/input.h"
#include "core/renderer/renderer.h"
#include "core/renderer/renderer_extra.h"
#include "core/camera.h"
#include "core/io/assetm.h"
#include "core/io/save_sys/save_sys.h"
#include "core/state/state.h"
#include "core/event_sys.h"
#include "core/debug/debug_draw.h"
#include "core/debug/debug_timer.h"
#ifdef TERRAIN_ADDON 
#include "core/terrain.h"
#endif  // TERRAIN_ADDON 
#include "core/debug/debug_opengl.h"

#include "global/global_print.h"

#include "tinyfiledialogs/tinyfiledialogs.h"
#include "stb/stb_ds.h"

#include <limits.h>


#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "GLAD/glad.h"

app_data_t  app_data_data = APP_DATA_INIT(); 
app_data_t* app_data = &app_data_data;

void move_cam_by_keys();
void rotate_cam_by_mouse();

TRACE_REGISTER() // initialize filestream needed for trace

int main(void)
{
  TRACE_INIT();   // initialize filestream needed for trace
  TRACE();


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
  
  // P_U32(assetm_str_to_u32("image.png"));
  // P_U32(assetm_str_to_u32("image.png"));
  // P_U32(assetm_str_to_u32("image.png"));
  // P_U32(assetm_str_to_u32("image.png"));
  // P_U32(assetm_str_to_u32("image.png"));

  // P_U32(assetm_str_to_u32("a"));
  // P_U32(assetm_str_to_u32("b"));
  // P_U32(assetm_str_to_u32("c"));
  // P_U32(assetm_str_to_u32("A"));
  // P_U32(assetm_str_to_u32("B"));
  // P_U32(assetm_str_to_u32("C"));
  // 
  // P_U32(assetm_str_to_u32("aa"));
  // P_U32(assetm_str_to_u32("ab"));
  // P_U32(assetm_str_to_u32("ac"));
  // P_U32(assetm_str_to_u32("Aa"));
  // P_U32(assetm_str_to_u32("Ab"));
  // P_U32(assetm_str_to_u32("Ac"));

  // P_U32(assetm_str_to_u32("aaaaaab"));
  // P_U32(assetm_str_to_u32("aaaaaac"));
  // P_U32(assetm_str_to_u32("aaaaaad"));
  // 
  // P_U32(assetm_str_to_u32("textures/images/test/cock/balls/texture_image_albedo_color_normal_.png"));
  // 
  // P_U32(UINT_MAX);
  // P_U64(ULONG_MAX);

  // PF("%u\n%u\n", assetm_str_to_u32("textures/images/test/cock/balls/texture_image_albedo_color_normal_.png"), UINT_MAX);
  // PF("%u\n%u\n", assetm_str_to_u32("textures/images/test/cock/balls/testing/if/too/high/texture_image_albedo_color_normal_.png"), UINT_MAX);
  // PF("%u | ", UINT_MAX); 
  // P_U32(assetm_str_to_u32("textures/this/should/do/it/lol/maybe/not_that/be/good/too/images/test///testing/if/too/high/texture_image_albedo_color_normal_.png"));

  // P_U32(assetm_str_to_u32("/"));
  // P_U32(assetm_str_to_u32("\\"));
  // P_U32(assetm_str_to_u32(" "));
  // P_U32(assetm_str_to_u32(":"));
  // P_U32(assetm_str_to_u32("."));
  // P_U32(assetm_str_to_u32("-"));
  // P_U32(assetm_str_to_u32("_"));
  // 
  // P_LINE();
  // abort();
  
  program_start(1800, 1000, "editor", WINDOW_MIN, app_init, app_update, app_cleanup, ASSET_PATH);  // WINDOW_FULL
  
  return 0;
}

void app_scene_pre_clear_callback()
{
  app_data->selected_id = -1;
}

void app_init()
{

  // // -- scene --
  // const char scene_name[] =  "test.scene";
  // // const char scene_name[] =  "empty.scene";
  // // save_sys_write_scene_to_file(scene_name);
  // TIMER_FUNC_STATIC(save_sys_load_scene_from_file(scene_name));
  // // in game will be done by camera-controller
  // // input_center_cursor_pos();
  // camera_set_pos(VEC3_XYZ(0.0f,   6.0f,  10.0f));
  // camera_set_front(VEC3_XYZ(0.0f,  -0.15f, -1.0f));
  vec3_copy(VEC3_XYZ(-13.5f, 10.5f, 15.75f), core_data->cam.pos);
  camera_set_pitch_yaw(-0.32f, -0.82f);
  
  event_sys_register_entity_removed(app_entity_removed_callback);
  event_sys_register_scene_pre_clear(app_scene_pre_clear_callback);

  editor_save_init();
  TIMER_FUNC_STATIC(gui_init());
  
  #ifdef TERRAIN_ADDON
  // // -- terrain --
  // TIMER_FUNC_STATIC(save_sys_load_terrain_from_file("test.terrain"));
  // TIMER_FUNC_STATIC(terrain_create(25));
  
  // vec2* info = malloc(sizeof(vec2) * TERRAIN_LAYOUT_VERT_INFO_LEN(core_data));
  // for (int i = 0; i < TERRAIN_LAYOUT_VERT_INFO_LEN(core_data); ++i)
  // { info[i][0] = 0.0f; info[i][1] = 0.0f; }
  // // { height[i] = (u8)(rand_f32() * 256.0f); }

  // const int size = 10;
  // for (int x = -(size/2); x < (size/2); ++x)
  // {
  //   for (int z = -(size/2); z < (size/2); ++z)
  //   {
  //     terrain_layout_t layout = { .pos = { x, z }, .vert_info = info  };
  //     arrput(core_data->terrain_layout, layout);
  //   }
  // }
  // terrain_layout_t layout = { .pos = { 0, 0 }, .vert_info = info  };
  // arrput(core_data->terrain_layout, layout);

  // // core_data->terrain_layout_len = arrlen(core_data->terrain_layout);
  // // core_data->terrain_layout_len = 0;
  // terrain_add_chunk(IVEC2(0));
  // P_INT(core_data->terrain_layout_len);
  // TIMER_FUNC_STATIC(terrain_create(25));
  // core_data->terrain_scl = 100;
  #endif // TERRAIN_ADDON
}

// [[gnu::destructor()]]
void app_update()
{

  // // @TMP:
  // mui_rect_round(VEC2_XY( 0.5f,  0.0f), VEC2_XY(1.0f, 0.5f), RGB_F(1, 0, 0)); 
  // mui_rect_round(VEC2_XY(-0.5f,  0.0f), VEC2_XY(1.0f, 0.5f), RGB_F(0, 1, 0)); 
  // mui_rect_round(VEC2_XY( 0.0f,  0.5f), VEC2_XY(1.0f, 0.5f), RGB_F(1, 0, 1)); 
  // mui_rect_round(VEC2_XY( 0.0f, -0.5f), VEC2_XY(1.0f, 0.5f), RGB_F(0, 1, 1)); 

  // mui_rect_round(VEC2_XY( 0.25f, 0.0f ), VEC2_XY(0.5f, 1.0f), RGB_F(1, 0, 0)); 
  // mui_rect_round(VEC2_XY(-0.25f, 0.0f ), VEC2_XY(0.5f, 1.0f), RGB_F(0, 1, 0)); 
  // mui_rect_round(VEC2_XY( 0.0f,  0.25f), VEC2_XY(0.5f, 1.0f), RGB_F(1, 0, 1)); 
  // mui_rect_round(VEC2_XY( 0.0f, -0.25f), VEC2_XY(0.5f, 1.0f), RGB_F(0, 1, 1)); 

  // mui_rect_round(VEC2_XY( 0.75f,  0.5f), VEC2_XY(1.0f, 1.0f), RGB_F(1, 0, 0)); 
  // mui_rect_round(VEC2_XY(-0.75f,  0.5f), VEC2_XY(1.0f, 1.0f), RGB_F(0, 1, 0)); 
  // mui_rect_round(VEC2_XY( 0.5f,  0.75f), VEC2_XY(1.0f, 1.0f), RGB_F(1, 0, 1)); 
  // mui_rect_round(VEC2_XY( 0.5f, -0.75f), VEC2_XY(1.0f, 1.0f), RGB_F(0, 1, 1)); 
  // 
  // mui_rect_round(VEC2_XY(-0.45f,-0.25f), VEC2_XY(0.5f, 2.0f), RGB_F(1, 0, 0)); 
  // mui_rect_round(VEC2_XY(-0.45f,-0.25f), VEC2_XY(2.0f, 0.5f), RGB_F(1, 0, 0)); 
  // mui_rect_round(VEC2_XY(-0.75f,-0.25f), VEC2_XY(2.0f, 0.2f), RGB_F(0, 1, 0)); 
  // mui_rect_round(VEC2_XY(-0.75f,-0.25f), VEC2_XY(0.2f, 2.0f), RGB_F(0, 1, 0)); 

  // if (input_get_key_pressed(KEY_P))
  // {
  //   audio_play_sound(sound_godspeed_idx, 0.5f);
  // }

  { // @TMP: testing mui
    // if (mui_button(VEC2_XY(0.0f, 0.0f), VEC2_XY(1.0f, 0.5f), VEC3(0.5f), "button00"))
    // {
    //   _PF("over button00\n");
    // }
    // if (mui_button(VEC2_XY(-0.5f, 0.0f), VEC2_XY(1.0f, 0.5f), VEC3(0.5f), "button01"))
    // {
    //   _PF("over button01\n");
    // }
    // if (mui_button(VEC2_XY(0.5f, 0.0f), VEC2_XY(1.0f, 0.5f), VEC3(0.5f), "button02"))
    // {
    //   _PF("over button02\n");
    // }
    // if (mui_button(VEC2_XY(0.0f, -0.5f), VEC2_XY(1.0f, 0.5f), VEC3(0.5f), "button03"))
    // {
    //   _PF("over button03\n");
    // }
    // if (mui_button(VEC2_XY(0.0f, 0.5f), VEC2_XY(1.0f, 0.5f), VEC3(0.5f), "button04"))
    // {
    //   _PF("over button04\n");
    // }

    // if (mui_button(VEC2_XY(0.5f, 0.5f), VEC2_XY(1.0f, 1.0f), VEC3(0.5f), "button05"))
    // {
    //   _PF("over button05\n");
    // }
    // if (mui_button(VEC2_XY(-0.5f, 0.5f), VEC2_XY(0.5f, 1.0f), VEC3(0.5f), "button06"))
    // {
    //   _PF("over button06\n");
    // }
    // if (mui_button(VEC2_XY(0.5f, -0.5f), VEC2_XY(1.0f, 1.0f), VEC3(0.5f), "button07"))
    // {
    //   _PF("over button07\n");
    // }
    // if (mui_button(VEC2_XY(-0.5f, -0.5f), VEC2_XY(0.5f, 1.0f), VEC3(0.5f), "button08"))
    // {
    //   _PF("over button08\n");
    // }

    // vec2 pos = { 0.75f, 0.75f };
    // mui_circle(pos, VEC2(0.1f), RGB_F(1, 0, 1));
    // mui_text(pos, "CENTER|UP",      MUI_CENTER | MUI_UP);
    // mui_text(pos, "CENTER|MIDDLE",  MUI_CENTER | MUI_MIDDLE);
    // mui_text(pos, "CENTER|DOWN",    MUI_CENTER | MUI_DOWN);
   
    // vec2_copy(VEC2_XY(0.75f, 0.65f), pos);
    // mui_circle(pos, VEC2(0.1f), RGB_F(1, 0, 1));
    // mui_text(pos, "LEFT|UP",        MUI_LEFT | MUI_UP);
    // mui_text(pos, "LEFT|MIDDLE",    MUI_LEFT | MUI_MIDDLE);
    // mui_text(pos, "LEFT|DOWN",      MUI_LEFT | MUI_DOWN);
   
    // vec2_copy(VEC2_XY(0.75f, 0.55f), pos);
    // mui_circle(pos, VEC2(0.1f), RGB_F(1, 0, 1));
    // mui_text(pos, "RIGHT|UP",       MUI_RIGHT | MUI_UP);
    // mui_text(pos, "RIGHT|MIDDLE",   MUI_RIGHT | MUI_MIDDLE);
    // mui_text(pos, "RIGHT|DOWN",     MUI_RIGHT | MUI_DOWN);

    // vec2_copy(VEC2_XY(0.75f, 0.0f), pos);
    // mui_circle(pos, VEC2(0.1f), RGB_F(1, 0, 1));
    // mui_text(pos, "CENTER|UP",      MUI_CENTER | MUI_UP);
    // mui_text(pos, "CENTER|MIDDLE",  MUI_CENTER | MUI_MIDDLE);
    // mui_text(pos, "CENTER|DOWN",    MUI_CENTER | MUI_DOWN);

    // vec2_copy(VEC2_XY(0.75f, -0.55f), pos);
    // mui_circle(pos, VEC2(0.1f), RGB_F(1, 0, 1));
    // mui_text(pos, "CENTER|UP",      MUI_CENTER | MUI_UP);
    // mui_text(pos, "CENTER|MIDDLE",  MUI_CENTER | MUI_MIDDLE);
    // mui_text(pos, "CENTER|DOWN",    MUI_CENTER | MUI_DOWN);
    // 
    // vec2_copy(VEC2_XY(0.75f, -0.65f), pos);
    // mui_circle(pos, VEC2(0.1f), RGB_F(1, 0, 1));
    // mui_text(pos, "LEFT|UP",        MUI_LEFT | MUI_UP);
    // mui_text(pos, "LEFT|MIDDLE",    MUI_LEFT | MUI_MIDDLE);
    // mui_text(pos, "LEFT|DOWN",      MUI_LEFT | MUI_DOWN);
   
    // vec2_copy(VEC2_XY(0.75f, -0.75f), pos);
    // mui_circle(pos, VEC2(0.1f), RGB_F(1, 0, 1));
    // mui_text(pos, "RIGHT|UP",       MUI_RIGHT | MUI_UP);
    // mui_text(pos, "RIGHT|MIDDLE",   MUI_RIGHT | MUI_MIDDLE);
    // mui_text(pos, "RIGHT|DOWN",     MUI_RIGHT | MUI_DOWN);

    // vec2_copy(VEC2_XY(0.75f, -0.95f), pos);
    // mui_circle(pos, VEC2(0.1f), RGB_F(1, 0, 1));
    // mui_text(pos, "CENTER|UP",      MUI_CENTER | MUI_UP);
    // mui_text(pos, "CENTER|MIDDLE",  MUI_CENTER | MUI_MIDDLE);
    // mui_text(pos, "CENTER|DOWN",    MUI_CENTER | MUI_DOWN);

    
    // mui_circle(VEC2_XY( 0.7f,  0.7f), VEC2(0.1f), RGB_F(0, 0, 1));
    // mui_circle(VEC2_XY(-0.7f,  0.7f), VEC2(0.1f), RGB_F(0, 0, 1));
    // mui_circle(VEC2_XY( 0.7f, -0.7f), VEC2(0.1f), RGB_F(0, 0, 1));
    // mui_circle(VEC2_XY(-0.7f, -0.7f), VEC2(0.1f), RGB_F(0, 0, 1));

    // mui_circle(VEC2_XY( 0.8f,  0.8f), VEC2(0.1f), RGB_F(0, 1, 0));
    // mui_circle(VEC2_XY(-0.8f,  0.8f), VEC2(0.1f), RGB_F(0, 1, 0));
    // mui_circle(VEC2_XY( 0.8f, -0.8f), VEC2(0.1f), RGB_F(0, 1, 0));
    // mui_circle(VEC2_XY(-0.8f, -0.8f), VEC2(0.1f), RGB_F(0, 1, 0));
    // 
    // mui_circle(VEC2_XY( 0.9f,  0.9f), VEC2(0.1f), RGB_F(1, 0, 1));
    // mui_circle(VEC2_XY(-0.9f,  0.9f), VEC2(0.1f), RGB_F(1, 0, 1));
    // mui_circle(VEC2_XY( 0.9f, -0.9f), VEC2(0.1f), RGB_F(1, 0, 1));
    // mui_circle(VEC2_XY(-0.9f, -0.9f), VEC2(0.1f), RGB_F(1, 0, 1));
    // 
    // mui_circle(VEC2_XY( 0.95f,  0.95f), VEC2(0.1f), RGB_F(1, 0, 0));
    // mui_circle(VEC2_XY(-0.95f,  0.95f), VEC2(0.1f), RGB_F(1, 0, 0));
    // mui_circle(VEC2_XY( 0.95f, -0.95f), VEC2(0.1f), RGB_F(1, 0, 0));
    // mui_circle(VEC2_XY(-0.95f, -0.95f), VEC2(0.1f), RGB_F(1, 0, 0));

    // mui_rect(VEC2_XY(-0.5f,  0.5f), VEC2_XY(1.0f, 0.5f), VEC3(0.5f));
    // mui_rect(VEC2_XY(-0.5f,  0.0f), VEC2_XY(0.5f, 0.5f), VEC3(0.5f));
    // mui_rect(VEC2_XY(-0.5f, -0.5f), VEC2_XY(0.5f, 1.0f), VEC3(0.5f));
  }

  // // @TMP: camera dirs debug draw
  // vec3 cam_pos, up, right;
  // vec3 front;
  // // vec3_copy(core_data->cam.front, front);
  // camera_get_front(front);
  // camera_get_right(right);
  // camera_get_up(up);
  // 
  // // vec3_mul_f(up, -0.5f, cam_pos);
  // // vec3_add(core_data->cam.pos, cam_pos, cam_pos);
  // vec3_copy(core_data->cam.pos, cam_pos);
  // vec3_add(front, cam_pos, cam_pos);
  // 
  // vec3_add(cam_pos, up, up);
  // vec3_add(cam_pos, right, right);
  // vec3_add(cam_pos, front, front);
  // debug_draw_line_register(cam_pos, up,    RGB_F(0, 1, 0));
  // debug_draw_line_register(cam_pos, right, RGB_F(1, 0, 0));
  // debug_draw_line_register(cam_pos, front, RGB_F(0, 0, 1));

  // if (input_get_key_pressed(KEY_SPACE))
  // {
  //   debug_draw_line_register_t(cam_pos, up,    RGB_F(0, 1, 0), 10.0f);
  //   debug_draw_line_register_t(cam_pos, right, RGB_F(1, 0, 0), 10.0f);
  //   debug_draw_line_register_t(cam_pos, front, RGB_F(0, 0, 1), 10.0f);
  // }

  // f32 color[]    = {1, 0, 1};
  // color[1]   =  1.0f;
  // texture_t* tex = assetm_get_texture("#internal/preview_bg.png", false); 

  // vec3 pos2 = { -2, -4, -5 };
  // vec3 rot  = { 0, 0, 0 };
  // vec3 scl  = { 1, 1, 1 };
  // mesh_t* m = assetm_get_mesh("demon01");  
  // renderer_direct_draw_mesh_textured(pos2, rot, scl, m, tex, scl);


  // toggle wireframe, esc to quit, etc.
  programm_app_default_logic();

  if (input_get_key_pressed(KEY_EXIT))
  {
    if ( core_data_get_play_state() != PLAY_STATE_STOPPED )
    { core_data_stop(); }
    else
    { core_data->program_quit = true; }
  }

  // @TODO: this shows infront of gizmos
  //        also move to gizmo.c
  // draw lights
  if (core_data_get_play_state() != PLAY_STATE_PLAY)
  {
    int world_len = 0;
    int world_dead_len = 0;
    entity_t* world = state_entity_get_arr(&world_len, &world_dead_len);
    vec3 pos;
    for (int i = 0; i < world_len; ++i)
    {
      if (world[i].point_light_idx >= 0) 
      {
        bool error = false;
        point_light_t* p = state_point_light_get(world[i].point_light_idx, &error); ASSERT(!error);
        vec3 e_pos;
        mat4_get_pos(world[i].model, e_pos);
        vec3_add(e_pos, p->offset, pos);
        debug_draw_mesh(pos, GIZMO_POINT_LIGHT_ROT, GIZMO_POINT_LIGHT_SCL, p->color, assetm_get_mesh_idx(GIZMO_POINT_LIGHT_MESH)); 
      }
    }
  }

  mat4 model, display_model;
  vec3 pos;
  // GIZMO_MODEL_POS(app_data, model, display_model, pos);
  gizmo_get_model_pos(model, display_model, pos);
  TIMER_FUNC(editor_renderer_extra_draw_scene_mouse_pick(display_model)); 
  TIMER_FUNC(gui_update());
  // if(!core_data_is_play()) { TIMER_FUNC(gizmo_update()); }
  if (core_data_get_play_state() != PLAY_STATE_PLAY) { TIMER_FUNC(gizmo_update()); }
  #ifdef TERRAIN_ADDON
  TIMER_FUNC(terrain_edit_update());
  #endif // TERRAIN_ADDON

  // @NOTE: sync selected with outline
  core_data->outline_id = app_data->selected_id;

  // save map & terrain
  if ( input_get_key_down(KEY_LEFT_CONTROL) && input_get_key_pressed(KEY_S) && core_data_get_play_state() != PLAY_STATE_PLAY )
  { 
    int rtn = 1;
    if ( core_data_get_play_state() == PLAY_STATE_PAUSED )
    {
      // returns 1 on ok
      rtn = tinyfd_messageBox(
          "saveing in paused mode", // NULL or ""
          "attempting to save in PAUSED mode.\n" 
          "save changes ?", // NULL or "" may contain \n \t
          "yesno", // "ok" "okcancel" "yesno" "yesnocancel"
          "warning", // "info" "warning" "error" "question"
          0); // 0 for cancel/no , 1 for ok/yes , 2 for no in yesnocancel
    }
    if ( rtn == 1 ) { app_save(); }
  }

  // undo operation
  if ( input_get_key_down(KEY_LEFT_CONTROL) && input_get_key_pressed(KEY_Z) && core_data_get_play_state() != PLAY_STATE_PLAY )
  { operation_reverse(); }
  
  // stop / pause pla-mode
  if (input_get_key_down(KEY_LEFT_CONTROL) && input_get_key_pressed(KEY_SPACE))
  {
    if      (core_data_get_play_state() == PLAY_STATE_PLAY)   { GUI_INFO_STR_SET(app_data, "paused");  core_data_pause(); }
    else if (core_data_get_play_state() == PLAY_STATE_PAUSED) { GUI_INFO_STR_SET(app_data, "stopped"); core_data_stop();  }
    else                                                      { GUI_INFO_STR_SET(app_data, "play");    core_data_play();  }
  }

   
    // toggle phys debug display, only works in play_mode as it happens in program_phys_sync
  if (input_get_key_down(KEY_LEFT_CONTROL) && input_get_key_pressed(KEY_TAB)) 
  { core_data->phys_debug_act = !core_data->phys_debug_act; }

  // toggle shadows
  if (input_get_key_pressed(KEY_SPACE))
  { core_data->show_shadows = !core_data->show_shadows; }

  // toggle wireframe, ctrl+tab is toggle phys display
  if (input_get_key_pressed(KEY_WIREFRAME_TOGGLE) && !input_get_key_down(KEY_LEFT_CONTROL))
  {
    core_data->wireframe_mode_enabled = !core_data->wireframe_mode_enabled;
  }

    // @TODO: flickers first frame
  static bool start = true;
  if (!app_data->mouse_over_ui && input_get_mouse_down(KEY_MOUSE_MOVE_START))
  {
    app_data->switch_gizmos_act = false;
    if (start)
    { 
      input_center_cursor_pos(); 
      start = false;
    }
    else
    {
      input_set_cursor_visible(false);
      rotate_cam_by_mouse(); 
      move_cam_by_keys();
    }
  }
  else
  { 
    app_data->switch_gizmos_act = true;
    start = true;
    input_set_cursor_visible(true); 
  }

  // snapping enabled when holding ctrl
  app_data->gizmo_snapping = (app_data->selected_id >= 0 && input_get_key_down(KEY_GIZMO_SNAPPING));
  
  // duplicate with 'ctrl + d'
  if (app_data->selected_id >= 0 && input_get_key_down(KEY_LEFT_CONTROL) && input_get_key_pressed(KEY_D))
  {
    int id = state_entity_duplicate_id(app_data->selected_id, VEC3_XYZ(2, 0, 0));
    app_data->selected_id = id;
  }

}
void app_cleanup()
{
  editor_save_cleanup();
}

void app_entity_removed_callback(int id)
{
  if (id == app_data->selected_id)
  {
    app_data->selected_id = -1;
  }
}

void app_save()
{
  // save_sys_write_scene_to_file(SCENE_FILE_NAME); 
  save_sys_write_scene_to_current_file();
  #ifdef TERRAIN_ADDON
  // // save_sys_write_terrain_to_file(TERRAIN_FILE_NAME); 
  save_sys_write_terrain_to_current_file();
  #endif // TERRAIN_ADDON

  editor_save_write_info_to_file();
  app_data->unsaved_changes = false;
  GUI_INFO_STR_SET(app_data, "saved");

  if (app_data->app_save_f != NULL) { app_data->app_save_f(); }
}

void move_cam_by_keys()
{
	vec3 front, up, dist;
  camera_get_front(front);
  camera_get_up(up);
  // -- move the cam --
	float cam_speed = CAM_SPEED * core_data->delta_t;
	if (input_get_key_down(KEY_LEFT_SHIFT))
	{ cam_speed *= CAM_SPEED_SHIFT_MULT; }
	if (input_get_key_down(KEY_MOVE_FORWARD))
	{
		vec3_mul_f(front, cam_speed, dist);
    vec3_add(core_data->cam.pos, dist, core_data->cam.pos);
	}
	if (input_get_key_down(KEY_MOVE_BACKWARD))
	{
		vec3_mul_f(front, -cam_speed, dist);
    vec3_add(core_data->cam.pos, dist, core_data->cam.pos);
	}
	if (input_get_key_down(KEY_MOVE_LEFT))
	{
		vec3_cross(front, up, dist);
		vec3_normalize(dist, dist);
		vec3_mul_f(dist, -cam_speed, dist);
    vec3_add(core_data->cam.pos, dist, core_data->cam.pos);
	}
	if (input_get_key_down(KEY_MOVE_RIGHT))
	{
		vec3_cross(front, up, dist);
		vec3_normalize(dist, dist);
		vec3_mul_f(dist, cam_speed, dist);
    vec3_add(core_data->cam.pos, dist, core_data->cam.pos);
	}
	if (input_get_key_down(KEY_MOVE_DOWN))
	{
		vec3_mul_f(up, -cam_speed, dist);
    vec3_add(core_data->cam.pos, dist, core_data->cam.pos);
	}
	if (input_get_key_down(KEY_MOVE_UP))
	{
		vec3_mul_f(up, cam_speed, dist);
    vec3_add(core_data->cam.pos, dist, core_data->cam.pos);
	}
}

// rotates the camera accoding to the mouse-movement
void rotate_cam_by_mouse()
{
	static bool init = false;
	static f32 pitch, yaw;

	f32 xoffset = (f32)input_get_mouse_delta_x();
	f32 yoffset = (f32)input_get_mouse_delta_y();

	xoffset *= app_data->mouse_sensitivity;
	yoffset *= app_data->mouse_sensitivity;

	
	yaw += xoffset;
	pitch += yoffset;

	// printf("pitch: %f, yaw: %f\n", pitch, yaw);

	if (pitch > 89.0f)
	{ pitch = 89.0f; }
	if (pitch < -89.0f)
	{ pitch = -89.0f; }

	if (!init)
	{
    // old cam sys:
    // vec3 front;
    // // camera_get_front(front);
    // vec3_copy(core_data->cam.front, front);
		// pitch = front[1] * 90; // -30.375f;
		// yaw	  =	front[2] * 90; // -90.875;
    pitch = core_data->cam.pitch_rad;
    yaw   = core_data->cam.yaw_rad;
    m_rad_to_deg(&yaw);
    m_rad_to_deg(&pitch);
		init = true;
	}

	// vec3 dir;
	f32 yaw_rad = yaw;     m_deg_to_rad(&yaw_rad);
	f32 pitch_rad = pitch; m_deg_to_rad(&pitch_rad);

	// dir[0] = (f32)cos(yaw_rad) * (f32)cos(pitch_rad);
	// dir[1] = (f32)sin(pitch_rad);
	// dir[2] = (f32)sin(yaw_rad) * (f32)cos(pitch_rad);
	// camera_set_front(dir);
  // camera_set_front(pitch_rad, yaw_rad);
  camera_set_pitch_yaw(pitch_rad, yaw_rad);
}

void editor_renderer_extra_draw_scene_mouse_pick(mat4 gizmo_model)
{
  TRACE();

  int w, h; window_get_size(&w, &h);
  
  framebuffer_bind(&core_data->fb_mouse_pick);
  _glViewport(0, 0, w / 4, h / 4);
  _glClearColor(0.0, 0.0, 0.0, 1.0);
  _glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // -- opengl state --
  _glEnable(GL_DEPTH_TEST); // enable the z-buffer
  _glEnable(GL_CULL_FACE);
  _glCullFace(GL_BACK);
  core_data->opengl_state |= OPENGL_DEPTH_TEST;
  core_data->opengl_state |= OPENGL_CULL_FACE;
  core_data->opengl_state |= OPENGL_CULL_FACE_BACK;

  mat4 view, proj;
  camera_get_view_mat(view);
  camera_get_proj_mat(w, h, proj);

  // cycle all objects
  int entities_len = 0;
  int entities_dead_len = 0;
  entity_t* entities = state_entity_get_arr(&entities_len, &entities_dead_len);
  shader_use(&core_data->mouse_pick_shader);
  for (int i = 0; i < entities_len; ++i)
  {
    entity_t* ent = &entities[i];
    if (ent->is_dead || ent->mesh < 0 || ent->mat < 0) { continue; }

    // state_entity_update_global_model(i);

    shader_set_float(&core_data->mouse_pick_shader, "id", (f32)ent->id);
    shader_set_mat4(&core_data->mouse_pick_shader, "model", ent->model);
    shader_set_mat4(&core_data->mouse_pick_shader, "view", view);
    shader_set_mat4(&core_data->mouse_pick_shader, "proj", proj);
    
    mesh_t* mesh = assetm_get_mesh_by_idx(ent->mesh);
    DRAW_MESH(mesh);
  }

  // -- draw terrain --
  #ifdef TERRAIN_ADDON
  for (int i = 0; i < (int)core_data->terrain_chunks_len; ++i) 
  { 
    if (!core_data->terrain_chunks[i].loaded || !core_data->terrain_chunks[i].visible) { continue; }
    shader_set_float(&core_data->mouse_pick_shader, "id", (f32)(ID_BUFFER_TERRAIN_0 -i)); // counts down
    shader_set_mat4(&core_data->mouse_pick_shader, "model", core_data->terrain_chunks[i].model);
    shader_set_mat4(&core_data->mouse_pick_shader, "view", view);
    shader_set_mat4(&core_data->mouse_pick_shader, "proj", proj);
    renderer_draw_terrain_mesh(&core_data->terrain_chunks[i]); 
  }
  #endif  // TERRAIN_ADDON

  // -- draw lights --
  mesh_t* sphere = assetm_get_mesh("sphere");
  for (int i = 0; i < entities_len; ++i)
  {
    entity_t* ent = &entities[i];
    if (ent->point_light_idx >= 0)
    {
      mat4 model;
      mat4_copy(ent->model, model);
      mat4_scale_f(model, 0.35f, model);
      shader_set_float(&core_data->mouse_pick_shader, "id", (f32)ent->id);

      shader_set_mat4(&core_data->mouse_pick_shader, "model", model);
      shader_set_mat4(&core_data->mouse_pick_shader, "view", view);
      shader_set_mat4(&core_data->mouse_pick_shader, "proj", proj);

      DRAW_MESH(sphere);
    }
  }

  // -- draw gizmo --
  if (app_data->selected_id >= 0 || app_data->selected_id == -2) // entity or terrain
  {
    _glClear(GL_DEPTH_BUFFER_BIT);
        
    // mat4 model;
    // if (app_data->selected_id >= 0)
    // { state_entity_model_no_scale(app_data->selected_id, model); }
    // else // terrain
    // { mat4_make_model(core_data->terrain_pos, core_data->terrain_rot, VEC3(1), model); }


    shader_set_mat4(&core_data->mouse_pick_shader, "model", gizmo_model);
    shader_set_mat4(&core_data->mouse_pick_shader, "view", view);
    shader_set_mat4(&core_data->mouse_pick_shader, "proj", proj);

    mesh_t* hitboxes_translate[] = 
    {
      assetm_get_mesh("gizmos/translate/x"),
      assetm_get_mesh("gizmos/translate/y"),
      assetm_get_mesh("gizmos/translate/z"),
      assetm_get_mesh("gizmos/translate/xy"),
      assetm_get_mesh("gizmos/translate/xz"),
      assetm_get_mesh("gizmos/translate/yz"),
      assetm_get_mesh("gizmos/translate/xyz")
    };
    mesh_t* hitboxes_scale[] = 
    {
      assetm_get_mesh("gizmos/scale/x"),
      assetm_get_mesh("gizmos/scale/y"),
      assetm_get_mesh("gizmos/scale/z"),
      assetm_get_mesh("gizmos/scale/xyz")
    };
    mesh_t* hitboxes_rotate[] = 
    {
      assetm_get_mesh("gizmos/rotate/x"),
      assetm_get_mesh("gizmos/rotate/y"),
      assetm_get_mesh("gizmos/rotate/z"),
      assetm_get_mesh("gizmos/rotate/xyz")
    };

    mesh_t** hitboxes = app_data->gizmo_type == 1 ? hitboxes_translate :
                        app_data->gizmo_type == 2 ? hitboxes_scale     : 
                        app_data->gizmo_type == 3 ? hitboxes_rotate    : NULL;
    int hitboxes_len  = app_data->gizmo_type == 1 ? 7 :
                        app_data->gizmo_type == 2 ? 4 :
                        app_data->gizmo_type == 3 ? 4 : 0;

    for (int i = 0; i < hitboxes_len; ++i)
    {
      shader_set_float(&core_data->mouse_pick_shader, "id", (f32)(ID_BUFFER_GIZMO_0 -i)); // -3, -4, -5, ...
      mesh_t* mesh = hitboxes[i];
      DRAW_MESH(mesh);
    }
  }

  framebuffer_unbind();
}
