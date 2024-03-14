#include "core/templates/material_template.h"
#include "test/material_table.h"
#include "test/shader_table.h"


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
    .tint     = { 1, 1, 1 },
    .roughn_f = 0.3f,
    .metall_f = 1.0f,
    // .shader_template = SHADER_TEMPLATE_NONE,
    .shader_template = SHADER_TEMPLATE_TEST,
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
}

#pragma GCC diagnostic pop // "-Winitializer-overrides" or -Woverride-init"
