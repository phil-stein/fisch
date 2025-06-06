#include "core/state/state_scripts.h"
#include "core/templates/entity_template.h"
#include "puzzle_game/entity_table.h"
#include "core/templates/material_template.h"
#include "puzzle_game/material_table.h"
#include "puzzle_game/entity_tags.h"
#include "puzzle_game/scripts.h"

entity_template_t entity_table[ENTITY_TEMPLATE_MAX];
const int entity_table_len = ENTITY_TEMPLATE_MAX;

#define ENTRY(_type)  ENTITY_TEMPLATE_ENTRY(entity_table, _type)

#ifdef _MSC_VER // gcc doesnt knwo this warning, GCC diagnostic push works in clang/msvc too
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winitializer-overrides"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverride-init"
#endif

void entity_table_init()
{
  // entity_table[ENTITY_TEMPLATE_QUAD] = (entity_template_t)
  // {
  ENTRY(QUAD)
    // ENTITY_TEMPLATE_T_SET_DEFAULTS(),
    // .name = "quad",
    .mesh = "quad",
    .mat  = MATERIAL_TEMPLATE_DEFAULT,
  };
  ENTRY(SPHERE01)
    .mesh = "sphere",
    .mat  = MATERIAL_TEMPLATE_DEFAULT,
    .phys_flag = ENTITY_HAS_SPHERE,
    .radius    = 1.0f,
  };
  ENTRY(SPHERE02)
    .mesh = "sphere",
    .mat  = MATERIAL_TEMPLATE_METALL,
    .phys_flag = ENTITY_HAS_SPHERE,
    .radius    = 1.0f,
  };
  ENTRY(DEMON01)
    .mesh = "demon01",
    .mat  = MATERIAL_TEMPLATE_DEMON01,
  };
  ENTRY(SHOTGUN)
    .mesh = "shotgun",
    .mat  = MATERIAL_TEMPLATE_SHOTGUN,
    // .update_f = entity_update,
  };
  ENTRY(DEMON02)
    .mesh = "demon02",
    .mat  = MATERIAL_TEMPLATE_DEMON02,
  };
  ENTRY(STONE01)
    .mesh = "stones/stone01_ld02_tri",
    .mat  = MATERIAL_TEMPLATE_STONE01,
  };
  ENTRY(HUT_TEST)
    .mesh = "base_hut",
    .mat  = MATERIAL_TEMPLATE_DEFAULT,
  };
  ENTRY(CUBE_STATIC)
    .mesh = "cube",
    .mat  = MATERIAL_TEMPLATE_GRID, // BRICK, // _PLANKS, // _PATH, // _METALL,
    .phys_flag = ENTITY_HAS_BOX,
    .aabb_size  = { 1, 1, 1 },
  };
  ENTRY(CUBE_DYNAMIC)
    .tags_flag = TAG_CUBE_DYN, 
    .mesh = "cube",
    .mat  = MATERIAL_TEMPLATE_PLANKS,
    .tags_flag = TAG_PICKUPABLE, 
    .phys_flag = ENTITY_HAS_RIGIDBODY | ENTITY_HAS_BOX,
    .mass = 15.0f,
    .aabb_size  = { 1, 1, 1 },
  };
  ENTRY(CUBE_STATIC_TRIGGER)
    .tags_flag = TAG_UP_FORCE,
    .mesh = "cube",
    .mat  = MATERIAL_TEMPLATE_METALL,
    .phys_flag = ENTITY_HAS_BOX,
    .aabb_size  = { 1, 1, 1 },
    .is_trigger = true,
  };
  ENTRY(PALADIN_BODY)
    .mesh = "mooh_body01",
    .mat  = MATERIAL_TEMPLATE_PALADIN_BODY,
    .phys_flag   = ENTITY_HAS_BOX,
    .aabb_size   = { 1.0f, 2.75f, 0.75f },
    .collider_offset = { 0.0f, 2.75f, 0.0f },
  };
  ENTRY(PALADIN_ARMOUR)
    .mesh = "mooh_armour01",
    .mat  = MATERIAL_TEMPLATE_PALADIN_ARMOUR,
  };
  ENTRY(PALADIN_WEAPON)
    .mesh = "mooh_weapon01",
    .mat  = MATERIAL_TEMPLATE_PALADIN_WEAPON,
    .phys_flag   = ENTITY_HAS_BOX,
    .aabb_size       = {  0.5f, 2.25f, 0.5f },
    .collider_offset = { -0.5f, 2.25f, 0.25f },
  };
  ENTRY(SPHERE_DYN)
    .mesh = "sphere",
    .mat  = MATERIAL_TEMPLATE_DEFAULT,
    .phys_flag = ENTITY_HAS_RIGIDBODY | ENTITY_HAS_SPHERE,
    .mass = 20.0f,
    .friction = 0.05f, 
    .radius    = 1.0f,
  };
  ENTRY(ROBOT_CHARACTER_06)
    .mesh = "robot_character_06_01",
    .mat  = MATERIAL_TEMPLATE_ROBOT_CHARACTER_06,
    // .rot  = { 90, 0, 0 },
    // .scl  = { 0.5f, 0.5f, 0.5f },
    .pointlight.add       = true,
    .pointlight.offset    = { 0.0f, 4.38f, 0.50f },
    .pointlight.color     = { 0.03f, 0.96f, 0.94f},
    .pointlight.intensity = 0.31f,
    .phys_flag   = ENTITY_HAS_BOX,
    .aabb_size   = { 0.5f, 2.25f, 0.5f },
    .collider_offset = { 0.0f, 2.25f, 0.0f },
  };
  ENTRY(FEMALE_CHAR_01)
    .mesh = "female_char_01_01",
    .mat  = MATERIAL_TEMPLATE_FEMALE_CHAR_01,
    // .rot  = { 90, 0, 0 },
    // .scl  = { 0.3f, 0.3f, 0.3f },
    .phys_flag   = ENTITY_HAS_BOX,
    .aabb_size   = { 0.5f, 2.25f, 0.5f },
    .collider_offset = { 0.0f, 2.25f, 0.0f },
  };
  ENTRY(FPS)
    .tags_flag = TAG_PLAYER,
    // .mesh = "female_char_01_01",
    // .mat  = MATERIAL_TEMPLATE_FEMALE_CHAR_01,
    // .mesh = "robot_character_06_01",
    // .mat  = MATERIAL_TEMPLATE_ROBOT_CHARACTER_06,
    .mesh = "-",
    .mat  = -1,
    .script_00_f = SCRIPT_ADD_PTR(fps_controller_script_t),
    .phys_flag   = ENTITY_HAS_RIGIDBODY | ENTITY_HAS_BOX,
    .mass = 65.0f,
    .friction = 0.05f, 
    .aabb_size   = { 0.5f, 2.25f, 0.5f },
    .collider_offset = { 0.0f, 2.25f, 0.0f },
  };
  ENTRY(POWER_LEVER_01)
    .mesh = "puzzle_game/power_lever_01",
    .mat  = MATERIAL_TEMPLATE_DEFAULT,
    .tags_flag = TAG_POWER_LEVER,
    // .phys_flag   = ENTITY_HAS_BOX,
    // .aabb_size       = {  0.5f, 2.25f, 0.5f },
    // .collider_offset = { -0.5f, 2.25f, 0.25f },
  };
  ENTRY(POWER_LEVER_02)
    .mesh = "puzzle_game/power_lever_02",
    .mat  = MATERIAL_TEMPLATE_METALL,
    .tags_flag = TAG_POWER_LEVER,
    .script_00_f = SCRIPT_ADD_PTR(power_lever_script_t),
    .pointlight.add       = true,
    .pointlight.offset    = { 0.0f, 1.0f, 0.0f },
    .pointlight.color     = { 1.00f, 0.0f, 0.0f},
    // .phys_flag   = ENTITY_HAS_BOX,
    // .aabb_size       = {  0.5f, 2.25f, 0.5f },
    // .collider_offset = { -0.5f, 2.25f, 0.25f },
  };
  ENTRY(DOOR_01)
    .mesh = "puzzle_game/door_01",
    .mat  = MATERIAL_TEMPLATE_DEFAULT,
  };
  ENTRY(PLANT_01)
    .mesh = "puzzle_game/plant_01",
    .mat  = MATERIAL_TEMPLATE_PLANT_01,
  };
  ENTRY(SPHERE_TRANS_01)
    .mesh = "sphere",
    .mat  = MATERIAL_TEMPLATE_TRANS_01,
    .phys_flag = ENTITY_HAS_SPHERE,
    .radius    = 1.0f,
  };
  ENTRY(SPHERE_TRANS_02)
    .mesh = "sphere",
    .mat  = MATERIAL_TEMPLATE_TRANS_02,
    .phys_flag = ENTITY_HAS_SPHERE,
    .radius    = 1.0f,
  };
}

#pragma GCC diagnostic pop // "-Winitializer-overrides" or -Woverride-init"

