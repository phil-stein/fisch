#include "core/templates/material_template.h"
#include "puzzle_game/material_table.h"
#include "puzzle_game/shader_table.h"


material_template_t material_table[MATERIAL_TEMPLATE_MAX];
const int material_table_len = MATERIAL_TEMPLATE_MAX;
#define ENTRY(_type)  MATERIAL_TEMPLATE_ENTRY(material_table, _type)

#ifdef _MSC_VER // gcc doesnt knwo this warning, GCC diagnostic push works in clang/msvc too
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winitializer-overrides"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverride-init"
#endif

void material_table_init()
{
   // metall
  // material_table[MATERIAL_TEMPLATE_METALL] = (material_template_t)
  // {
  //   MATERIAL_TEMPLATE_DEFAULT_INIT(MATERIAL_TEMPLATE_METALL),
  ENTRY(MATERIAL_TEMPLATE_METALL)
    .albedo   = NULL,
    .normal   = NULL,
    .roughn   = NULL,
    .metall   = NULL,
    // .tint     = { 1, 1, 1 },
    // .tint     = { 0.68f, 0.68f, 0.68f },
    .tint     = { 0.68f, 0.68f, 0.68f },
    .roughn_f = 0.3f,
    .metall_f = 1.0f,
    .shader_template = SHADER_TEMPLATE_NONE,
    // .shader_template = SHADER_TEMPLATE_TEST,
  };
  // demon01
  ENTRY(MATERIAL_TEMPLATE_DEMON01)
    .albedo   = "demon01/demon01_albedo.png",
    .normal   = "demon01/demon01_normal.png",
    .roughn   = "demon01/demon01_roughness.png",
    .metall   = NULL,
    .tint     = { 1, 1, 1 },
    .roughn_f = 1.0f,
    .metall_f = 0.0f,
    .shader_template = SHADER_TEMPLATE_NONE,
  };
  // demon02
  ENTRY(MATERIAL_TEMPLATE_DEMON02)
    .albedo   = "demon02/demon02_albedo.png",
    .normal   = "demon02/demon02_normal.png",
    .roughn   = "demon02/demon02_roughness.png",
    .metall   = "demon02/demon02_metallic.png",
    .tint     = { 1, 1, 1 },
    .roughn_f = 1.0f,
    .metall_f = 1.0f,
    .shader_template = SHADER_TEMPLATE_NONE,
  };
  // shotgun
  ENTRY(MATERIAL_TEMPLATE_SHOTGUN)
    .albedo   = "shotgun/shotgun_albedo.png",
    .normal   = "shotgun/shotgun_normal.png",
    .roughn   = "shotgun/shotgun_roughness.png",
    .metall   = "shotgun/shotgun_metallic.png",
    .tint     = { 1, 1, 1 },
    .roughn_f = 1.0f,
    .metall_f = 1.0f,
    .shader_template = SHADER_TEMPLATE_NONE,
  };
  // grass
  ENTRY(MATERIAL_TEMPLATE_GRASS)
    .albedo   = "stylized_grass/stylized_grass_albedo.jpg",
    .normal   = "stylized_grass/stylized_grass_normal.jpg",
    .roughn   = "stylized_grass/stylized_grass_roughness.jpg",
    .metall   = NULL,
    .tint     = { 1, 1, 1 },
    .roughn_f = 1.0f,
    .metall_f = 0.0f,
    .shader_template = SHADER_TEMPLATE_NONE,
  };
  // stone01
  ENTRY(MATERIAL_TEMPLATE_STONE01)
    .albedo   = "stone01_02/stone01_albedo.png",
    .normal   = "stone01_02/stone01_normal.png",
    .roughn   = "stone01_02/stone01_roughness.png",
    .metall   = NULL,
    .tint     = { 1, 1, 1 },
    .roughn_f = 1.0f,
    .metall_f = 0.0f,
    .shader_template = SHADER_TEMPLATE_NONE,
  };
  // tree01
  ENTRY(MATERIAL_TEMPLATE_TREE01)
    .albedo   = "tree01/tree01_albedo.png",
    .normal   = "tree01/tree01_normal.png",
    .roughn   = "tree01/tree01_roughness.png",
    .metall   = NULL,
    .tint     = { 1, 1, 1 },
    .roughn_f = 1.0f,
    .metall_f = 0.0f,
    .shader_template = SHADER_TEMPLATE_NONE,
  };
  // path
  ENTRY(MATERIAL_TEMPLATE_PATH)
    .albedo   = "stylized_path/stylized_path_albedo.jpg",
    .normal   = "stylized_path/stylized_path_normal.jpg",
    .roughn   = "stylized_path/stylized_path_roughness.jpg",
    .metall   = NULL,
    .tint     = { 1, 1, 1 },
    .roughn_f = 1.0f,
    .metall_f = 0.0f,
    .shader_template = SHADER_TEMPLATE_NONE,
  };
  // planks
  ENTRY(MATERIAL_TEMPLATE_PLANKS)
    .albedo   = "stylized_planks/stylized_planks_albedo.jpg",
    .normal   = "stylized_planks/stylized_planks_normal.jpg",
    .roughn   = "stylized_planks/stylized_planks_roughness.jpg",
    .metall   = NULL,
    .tint     = { 1, 1, 1 },
    .roughn_f = 0.8f,
    .metall_f = 0.0f,
    .shader_template = SHADER_TEMPLATE_NONE,
    .tile_scl = 1.0f,
    .tile_by_scl = true,
  };
  // bricks
  ENTRY(MATERIAL_TEMPLATE_BRICK)
    .albedo   = "stylized_brick/stylized_bricks_albedo.jpg",
    .normal   = "stylized_brick/stylized_bricks_normal.jpg",
    .roughn   = "stylized_brick/stylized_bricks_roughness.jpg",
    .metall   = NULL,
    .tint     = { 1, 1, 1 },
    .roughn_f = 0.65f,
    .metall_f = 0.0f,
    .shader_template = SHADER_TEMPLATE_NONE,
    .tile_scl = 1.0f,
    .tile_by_scl = true,
  };
  // paladin body
  ENTRY(MATERIAL_TEMPLATE_PALADIN_BODY)
    .albedo   = "devil_paladin_body/albedo.png",
    .normal   = "devil_paladin_body/normal.png",
    .roughn   = "devil_paladin_body/roughness.png",
    // .metall = "devil_paladin_body/metallic.png",
    .metall   = NULL,
    .tint     = { 1, 1, 1 },
    .roughn_f = 1.0f,
    .metall_f = 0.0f,
    .shader_template = SHADER_TEMPLATE_NONE,
  };
  // paladin armour
  ENTRY(MATERIAL_TEMPLATE_PALADIN_ARMOUR)
    .albedo   = "devil_paladin_armour/albedo.png",
    .normal   = "devil_paladin_armour/normal.png",
    .roughn   = "devil_paladin_armour/roughness.png",
    .metall   = "devil_paladin_armour/metallic.png",
    .tint     = { 1, 1, 1 },
    .roughn_f = 1.0f,
    .metall_f = 1.0f,
    .shader_template = SHADER_TEMPLATE_NONE,
  };
  // paladin weapon
  ENTRY(MATERIAL_TEMPLATE_PALADIN_WEAPON)
    .albedo   = "devil_paladin_weapon/albedo.png",
    .normal   = "devil_paladin_weapon/normal.png",
    .roughn   = "devil_paladin_weapon/roughness.png",
    .metall   = "devil_paladin_weapon/metallic.png",
    .tint     = { 1, 1, 1 },
    .roughn_f = 1.0f,
    .metall_f = 1.0f,
    .shader_template = SHADER_TEMPLATE_NONE,
  };
  // robot_character_06
  ENTRY(MATERIAL_TEMPLATE_ROBOT_CHARACTER_06)
    .albedo     = "robot_character_06/albedo.png",
    .normal     = "robot_character_06/normal.png",
    .roughn     = "robot_character_06/roughness.png",
    .metall     = "robot_character_06/metallic.png",
    .emissive   = "robot_character_06/emissive.png",
    .tint       = { 1, 1, 1 },
    .roughn_f   = 1.0f,
    .metall_f   = 1.0f,
    .emissive_f = 1.0f,
    .shader_template = SHADER_TEMPLATE_NONE,
  };
  // female_char_01 
  ENTRY(MATERIAL_TEMPLATE_FEMALE_CHAR_01)
    .albedo   = "female_char_01/albedo.png",
    .normal   = "female_char_01/normal.png",
    .roughn   = "female_char_01/roughness.png",
    .metall   = "female_char_01/metallic.png",
    .tint     = { 1, 1, 1 },
    .roughn_f = 1.0f,
    .metall_f = 1.0f,
    .shader_template = SHADER_TEMPLATE_NONE,
  };
  ENTRY(MATERIAL_TEMPLATE_GRID)
    .albedo   = "grid_02.jpg", // "grid_01.jpg",
    // .albedo      = "grass_01.png", 
    .translucent = false,
    .normal   = NULL,
    .roughn   = NULL,
    .metall   = NULL,
    .tint     = { 1, 1, 1 },
    .roughn_f = 0.6f,
    .metall_f = 0.1f,
    .shader_template = SHADER_TEMPLATE_NONE,
  };
  ENTRY(MATERIAL_TEMPLATE_PLANT_01)
    .albedo   = "plant_01.png",
    .normal   = NULL,
    .roughn   = NULL,
    .metall   = NULL,
    .tint     = { 1, 1, 1 },
    .roughn_f = 0.0f,
    .metall_f = 0.0f,
    .translucent = true,
    .shader_template = SHADER_TEMPLATE_NONE,
  };
  ENTRY(MATERIAL_TEMPLATE_TRANS_01)
    .albedo = "#internal/blank_alpha.png",
    .normal   = NULL,
    .roughn   = NULL,
    .metall   = NULL,
    .tint     = { 0.35f, 0.45f, 1 },
    .roughn_f = 0.0f,
    .metall_f = 0.0f,
    .translucent = true,
    .shader_template = SHADER_TEMPLATE_NONE,
  };
  ENTRY(MATERIAL_TEMPLATE_TRANS_02)
    .albedo = "#internal/blank_alpha.png",
    .normal   = NULL,
    .roughn   = NULL,
    .metall   = NULL,
    .tint     = { 0, 1, 0.3f },
    .roughn_f = 0.0f,
    .metall_f = 0.0f,
    .translucent = true,
    .shader_template = SHADER_TEMPLATE_NONE,
  };
}

#pragma GCC diagnostic pop // "-Winitializer-overrides" or -Woverride-init"
