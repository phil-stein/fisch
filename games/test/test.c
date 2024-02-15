#include "global/global.h"
#include "math/math_inc.h"
#include "games.h"  // ./games/games.h
#include "core/ecs/ecs.h"
#include "core/core_data.h"
#include "core/io/input.h"
#include "core/io/assetm.h"
#include "core/io/save_sys/save_sys.h"
#include "core/debug/debug_timer.h"
#include "mui/mui.h"
#include "test/material_table.h"
#include "test/entity_table.h"
// #include "test/test_comp.h"
#include "test/scripts.h"

#include "stb/stb_ds.h"

static core_data_t* core_data = NULL;
u8* buffer = NULL;

#define AMMO_MAX 30
int ammo = AMMO_MAX;


// @UNSURE: gets called before main loop regardless if editor or game
void __init__()
{
  core_data = core_data_get();
  
  save_sys_load_scene_terrain("test.scene", NULL);  
  // save_sys_load_scene_terrain("phys_test.scene", NULL);  
  // // @TODO: this should be safed in .terrain
  // terrain_add_material(MATERIAL_TEMPLATE_GRASS);
  // terrain_add_material(MATERIAL_TEMPLATE_PATH);

  // in game will be done by camera-controller
  vec3_copy(VEC3_XYZ(0.0f,   6.0f,  10.0f), core_data->cam.pos);
  vec3_copy(VEC3_XYZ(0.0f,  -0.15f, -1.0f), core_data->cam.front);

  // @NOTE: scripts_init() replaced by calling scripts_init...() in SCRIPT_ADD()
  // TIMER_FUNC_STATIC(scripts_init());
}

void __update__()
{
  TIMER_FUNC(scripts_update());
  
// #ifdef _MSC_VER
//   {
//     // TIMER_START("ui");
// 
//     texture_t* circle_tex = assetm_get_texture("#internal/circle.png", false);
//     texture_t* weapon_tex = assetm_get_texture("_icons/kriss_vector_01.png", false);
//     
//     // -- circle & ammo --
//     { 
//       mui_img_tint(VEC2_XY(-0.8f, -0.8f),   VEC2(0.80f), circle_tex, VEC3(0.75f));
//       mui_img_tint(VEC2_XY(-0.8f, -0.8f),   VEC2(0.65f), circle_tex, VEC3(0.55f));
// 
//       mui_img_tint(VEC2_XY(-0.8f, -0.8f),   VEC2(0.45f), weapon_tex, VEC3(1.00f));
// 
//       mui_img_tint(VEC2_XY(-0.72f, -0.72f), VEC2(0.50f), circle_tex, VEC3(0.35f));
// 
//       char txt[64];
//       SPRINTF(64, txt, "%d|%d", ammo, AMMO_MAX);
//       mui_text(VEC2_XY(-0.70f, -0.65f), txt, MUI_CENTER | MUI_UP);
//     }
//     // -- inventory --
//     {
//       mui_group_t group;
//       mui_group_t items;
//       // MUI_GROUP_T_INIT(&group, VEC2_XY(0.0f, -0.8f), VEC2_XY(1.75f, 0.5f), 0.0f, MUI_DYNAMIC | MUI_CENTER | MUI_ROW, false);
//       // MUI_GROUP_T_INIT(&items, VEC2_XY(0.0f, -0.8f), VEC2_XY(1.75f, 0.5f), 0.0f, MUI_DYNAMIC | MUI_CENTER | MUI_ROW, false);
//       // 
//       // MUI_GROUP_T_ADD(&group, MUI_OBJ_T_IMG_GROUP(circle_tex, 1.00f, 1.00f, 1.00f));
//       // MUI_GROUP_T_ADD(&group, MUI_OBJ_T_IMG_GROUP(circle_tex, 0.75f, 0.75f, 0.75f));
//       // MUI_GROUP_T_ADD(&group, MUI_OBJ_T_IMG_GROUP(circle_tex, 0.50f, 0.50f, 0.50f));
//       // MUI_GROUP_T_ADD(&group, MUI_OBJ_T_IMG_GROUP(circle_tex, 0.25f, 0.25f, 0.25f));
//       // 
//       // mui_group(&group);
//       {
//         MUI_GROUP_T_INIT(&group, VEC2_XY(0.0f, -0.8f), VEC2_XY(1.75f, 0.5f), 0.0f, MUI_STATIC | MUI_CENTER | MUI_ROW, false);
//         
//         MUI_GROUP_T_ADD(&group, MUI_OBJ_T_IMG_GROUP(circle_tex, 1.00f, 1.00f, 1.00f));
//         MUI_GROUP_T_ADD(&group, MUI_OBJ_T_IMG_GROUP(circle_tex, 0.75f, 0.75f, 0.75f));
//         MUI_GROUP_T_ADD(&group, MUI_OBJ_T_IMG_GROUP(circle_tex, 0.50f, 0.50f, 0.50f));
//         MUI_GROUP_T_ADD(&group, MUI_OBJ_T_IMG_GROUP(circle_tex, 0.25f, 0.25f, 0.25f));
//         
//         mui_group(&group);
//       }
//       {
//         MUI_GROUP_T_INIT(&items, VEC2_XY(0.0f, -0.8f), VEC2_XY(1.75f, 0.5f), 0.0f, MUI_STATIC | MUI_CENTER | MUI_ROW, false);
//         mui_obj_t* slot_1, *slot_2, *slot_3, *slot_4;
//         MUI_GROUP_T_ADD_PTR(&items, MUI_OBJ_T_IMG(0.0f, 0.0f, 0.55f, 0.55f, weapon_tex, 1.00f, 1.00f, 1.00f), slot_1);
//         MUI_GROUP_T_ADD_PTR(&items, MUI_OBJ_T_IMG(0.0f, 0.0f, 0.55f, 0.55f, weapon_tex, 0.75f, 0.75f, 0.75f), slot_2);
//         MUI_GROUP_T_ADD_PTR(&items, MUI_OBJ_T_IMG(0.0f, 0.0f, 0.55f, 0.55f, weapon_tex, 0.50f, 0.50f, 0.50f), slot_3);
//         MUI_GROUP_T_ADD_PTR(&items, MUI_OBJ_T_IMG(0.0f, 0.0f, 0.55f, 0.55f, weapon_tex, 0.25f, 0.25f, 0.25f), slot_4);
//         
//         static int active_slot = 1;
//         slot_1->active = active_slot == 1;
//         slot_2->active = active_slot == 2;
//         slot_3->active = active_slot == 3;
//         slot_4->active = active_slot == 4;
// 
//         if (input_get_key_pressed(KEY_ALPHA1) || input_get_key_pressed(KEY_NUMPAD1)) { active_slot = 1; }
//         if (input_get_key_pressed(KEY_ALPHA2) || input_get_key_pressed(KEY_NUMPAD2)) { active_slot = 2; }
//         if (input_get_key_pressed(KEY_ALPHA3) || input_get_key_pressed(KEY_NUMPAD3)) { active_slot = 3; }
//         if (input_get_key_pressed(KEY_ALPHA4) || input_get_key_pressed(KEY_NUMPAD4)) { active_slot = 4; }
// 
//         
//         mui_group(&items);
//       }
//     }
//     // TIMER_STOP_PRINT(); // ui    
//   }
// #endif // _MSC_VER
}

void __cleanup__()
{
}
