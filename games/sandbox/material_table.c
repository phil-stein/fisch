#include "core/templates/material_template.h"
#include "core/templates/shader_template.h"
#include "test/material_table.h"

// const material_template_t material_table[] = 
// {
//   // metall
//   {
//     MATERIAL_TEMPLATE_DEFAULT_INIT(),
//     .albedo = NULL,
//     .normal = NULL,
//     .roughn = NULL,
//     .metall = NULL,
//     .tint   = { 1, 1, 1 },
//     .roughn_f = 0.3f,
//     .metall_f = 1.0f,
//     .shader_template = SHADER_TEMPLATE_NONE,
//   },
//   // demon01
//   {
//     MATERIAL_TEMPLATE_DEFAULT_INIT(),
//     .albedo = "demon01/demon01_albedo.png",
//     .normal = "demon01/demon01_normal.png",
//     .roughn = "demon01/demon01_roughness.png",
//     .metall = NULL,
//     .tint   = { 1, 1, 1 },
//     .roughn_f = 1.0f,
//     .metall_f = 0.0f,
//     .shader_template = SHADER_TEMPLATE_NONE,
//   },
//   // demon02
//   {
//     MATERIAL_TEMPLATE_DEFAULT_INIT(),
//     .albedo = "demon02/demon02_albedo.png",
//     .normal = "demon02/demon02_normal.png",
//     .roughn = "demon02/demon02_roughness.png",
//     .metall = "demon02/demon02_metallic.png",
//     .tint   = { 1, 1, 1 },
//     .roughn_f = 1.0f,
//     .metall_f = 1.0f,
//     .shader_template = SHADER_TEMPLATE_NONE,
//   },
//   // shotgun
//   {
//     MATERIAL_TEMPLATE_DEFAULT_INIT(),
//     .albedo = "shotgun/shotgun_albedo.png",
//     .normal = "shotgun/shotgun_normal.png",
//     .roughn = "shotgun/shotgun_roughness.png",
//     .metall = "shotgun/shotgun_metallic.png",
//     .tint   = { 1, 1, 1 },
//     .roughn_f = 1.0f,
//     .metall_f = 1.0f,
//     .shader_template = SHADER_TEMPLATE_NONE,
//   },
//   // grass
//   {
//     MATERIAL_TEMPLATE_DEFAULT_INIT(),
//     .albedo = "stylized_grass/stylized_grass_albedo.jpg",
//     .normal = "stylized_grass/stylized_grass_normal.jpg",
//     .roughn = "stylized_grass/stylized_grass_roughness.jpg",
//     .metall = NULL,
//     .tint   = { 1, 1, 1 },
//     .roughn_f = 1.0f,
//     .metall_f = 0.0f,
//     .shader_template = SHADER_TEMPLATE_NONE,
//   },
//   // stone01
//   {
//     MATERIAL_TEMPLATE_DEFAULT_INIT(),
//     .albedo = "stone01_02/stone01_albedo.png",
//     .normal = "stone01_02/stone01_normal.png",
//     .roughn = "stone01_02/stone01_roughness.png",
//     .metall = NULL,
//     .tint   = { 1, 1, 1 },
//     .roughn_f = 1.0f,
//     .metall_f = 0.0f,
//     .shader_template = SHADER_TEMPLATE_NONE,
//   },
//   // tree01
//   {
//     MATERIAL_TEMPLATE_DEFAULT_INIT(),
//     .albedo = "tree01/tree01_albedo.png",
//     .normal = "tree01/tree01_normal.png",
//     .roughn = "tree01/tree01_roughness.png",
//     .metall = NULL,
//     .tint   = { 1, 1, 1 },
//     .roughn_f = 1.0f,
//     .metall_f = 0.0f,
//     .shader_template = SHADER_TEMPLATE_NONE,
//   },
//   // path
//   {
//     MATERIAL_TEMPLATE_DEFAULT_INIT(),
//     .albedo = "stylized_path/stylized_path_albedo.jpg",
//     .normal = "stylized_path/stylized_path_normal.jpg",
//     .roughn = "stylized_path/stylized_path_roughness.jpg",
//     .metall = NULL,
//     .tint   = { 1, 1, 1 },
//     .roughn_f = 1.0f,
//     .metall_f = 0.0f,
//     .shader_template = SHADER_TEMPLATE_NONE,
//   },
//   // planks
//   {
//     MATERIAL_TEMPLATE_DEFAULT_INIT(),
//     .albedo = "stylized_planks/stylized_planks_albedo.jpg",
//     .normal = "stylized_planks/stylized_planks_normal.jpg",
//     .roughn = "stylized_planks/stylized_planks_roughness.jpg",
//     .metall = NULL,
//     .tint   = { 1, 1, 1 },
//     .roughn_f = 0.8f,
//     .metall_f = 0.0f,
//     .shader_template = SHADER_TEMPLATE_NONE,
//     .tile_scl = 1.0f,
//     .tile_by_scl = true,
//   },
//   // bricks
//   {
//     MATERIAL_TEMPLATE_DEFAULT_INIT(),
// 
//     .albedo = "stylized_brick/stylized_bricks_albedo.jpg",
//     .normal = "stylized_brick/stylized_bricks_normal.jpg",
//     .roughn = "stylized_brick/stylized_bricks_roughness.jpg",
//     .metall = NULL,
//     .tint   = { 1, 1, 1 },
//     .roughn_f = 0.65f,
//     .metall_f = 0.0f,
//     .shader_template = SHADER_TEMPLATE_NONE,
//     .tile_scl = 1.0f,
//     .tile_by_scl = true,
//   },
//   // paladin body
//   {
//     MATERIAL_TEMPLATE_DEFAULT_INIT(),
//     .albedo = "devil_paladin_body/albedo.png",
//     .normal = "devil_paladin_body/normal.png",
//     .roughn = "devil_paladin_body/roughness.png",
//     // .metall = "devil_paladin_body/metallic.png",
//     .metall = NULL,
//     .tint   = { 1, 1, 1 },
//     .roughn_f = 1.0f,
//     .metall_f = 0.0f,
//     .shader_template = SHADER_TEMPLATE_NONE,
//   },
//   // paladin armour
//   {
//     MATERIAL_TEMPLATE_DEFAULT_INIT(),
//     .albedo = "devil_paladin_armour/albedo.png",
//     .normal = "devil_paladin_armour/normal.png",
//     .roughn = "devil_paladin_armour/roughness.png",
//     .metall = "devil_paladin_armour/metallic.png",
//     .tint   = { 1, 1, 1 },
//     .roughn_f = 1.0f,
//     .metall_f = 1.0f,
//     .shader_template = SHADER_TEMPLATE_NONE,
//   },
//   // paladin weapon
//   {
//     MATERIAL_TEMPLATE_DEFAULT_INIT(),
//     .albedo = "devil_paladin_weapon/albedo.png",
//     .normal = "devil_paladin_weapon/normal.png",
//     .roughn = "devil_paladin_weapon/roughness.png",
//     .metall = "devil_paladin_weapon/metallic.png",
//     .tint   = { 1, 1, 1 },
//     .roughn_f = 1.0f,
//     .metall_f = 1.0f,
//     .shader_template = SHADER_TEMPLATE_NONE,
//   },
//   // grid 
//   {
//     MATERIAL_TEMPLATE_DEFAULT_INIT(),
//     .albedo = "grid_01.jpg",
//     .normal = NULL,
//     .roughn = NULL,
//     .metall = NULL,
//     .tint   = { 1, 1, 1 },
//     .roughn_f = 0.3f,
//     .metall_f = 0.0f,
//     .shader_template = SHADER_TEMPLATE_NONE,
//   },
// };
// const int material_table_len = sizeof(material_table) / sizeof(material_table[0]);


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
  ENTRY(METALL)
    .albedo = NULL,
    .normal = NULL,
    .roughn = NULL,
    .metall = NULL,
    .tint   = { 1, 1, 1 },
    .roughn_f = 0.3f,
    .metall_f = 1.0f,
    .shader_template = SHADER_TEMPLATE_NONE,
  };
  // demon01
  {
    MATERIAL_TEMPLATE_DEFAULT_INIT(),
    .albedo = "demon01/demon01_albedo.png",
    .normal = "demon01/demon01_normal.png",
    .roughn = "demon01/demon01_roughness.png",
    .metall = NULL,
    .tint   = { 1, 1, 1 },
    .roughn_f = 1.0f,
    .metall_f = 0.0f,
    .shader_template = SHADER_TEMPLATE_NONE,
  },
  // demon02
  {
    MATERIAL_TEMPLATE_DEFAULT_INIT(),
    .albedo = "demon02/demon02_albedo.png",
    .normal = "demon02/demon02_normal.png",
    .roughn = "demon02/demon02_roughness.png",
    .metall = "demon02/demon02_metallic.png",
    .tint   = { 1, 1, 1 },
    .roughn_f = 1.0f,
    .metall_f = 1.0f,
    .shader_template = SHADER_TEMPLATE_NONE,
  },
  // shotgun
  {
    MATERIAL_TEMPLATE_DEFAULT_INIT(),
    .albedo = "shotgun/shotgun_albedo.png",
    .normal = "shotgun/shotgun_normal.png",
    .roughn = "shotgun/shotgun_roughness.png",
    .metall = "shotgun/shotgun_metallic.png",
    .tint   = { 1, 1, 1 },
    .roughn_f = 1.0f,
    .metall_f = 1.0f,
    .shader_template = SHADER_TEMPLATE_NONE,
  },
  // grass
  {
    MATERIAL_TEMPLATE_DEFAULT_INIT(),
    .albedo = "stylized_grass/stylized_grass_albedo.jpg",
    .normal = "stylized_grass/stylized_grass_normal.jpg",
    .roughn = "stylized_grass/stylized_grass_roughness.jpg",
    .metall = NULL,
    .tint   = { 1, 1, 1 },
    .roughn_f = 1.0f,
    .metall_f = 0.0f,
    .shader_template = SHADER_TEMPLATE_NONE,
  },
  // stone01
  {
    MATERIAL_TEMPLATE_DEFAULT_INIT(),
    .albedo = "stone01_02/stone01_albedo.png",
    .normal = "stone01_02/stone01_normal.png",
    .roughn = "stone01_02/stone01_roughness.png",
    .metall = NULL,
    .tint   = { 1, 1, 1 },
    .roughn_f = 1.0f,
    .metall_f = 0.0f,
    .shader_template = SHADER_TEMPLATE_NONE,
  },
  // tree01
  {
    MATERIAL_TEMPLATE_DEFAULT_INIT(),
    .albedo = "tree01/tree01_albedo.png",
    .normal = "tree01/tree01_normal.png",
    .roughn = "tree01/tree01_roughness.png",
    .metall = NULL,
    .tint   = { 1, 1, 1 },
    .roughn_f = 1.0f,
    .metall_f = 0.0f,
    .shader_template = SHADER_TEMPLATE_NONE,
  },
  // path
  {
    MATERIAL_TEMPLATE_DEFAULT_INIT(),
    .albedo = "stylized_path/stylized_path_albedo.jpg",
    .normal = "stylized_path/stylized_path_normal.jpg",
    .roughn = "stylized_path/stylized_path_roughness.jpg",
    .metall = NULL,
    .tint   = { 1, 1, 1 },
    .roughn_f = 1.0f,
    .metall_f = 0.0f,
    .shader_template = SHADER_TEMPLATE_NONE,
  },
  // planks
  {
    MATERIAL_TEMPLATE_DEFAULT_INIT(),
    .albedo = "stylized_planks/stylized_planks_albedo.jpg",
    .normal = "stylized_planks/stylized_planks_normal.jpg",
    .roughn = "stylized_planks/stylized_planks_roughness.jpg",
    .metall = NULL,
    .tint   = { 1, 1, 1 },
    .roughn_f = 0.8f,
    .metall_f = 0.0f,
    .shader_template = SHADER_TEMPLATE_NONE,
    .tile_scl = 1.0f,
    .tile_by_scl = true,
  },
  // bricks
  {
    MATERIAL_TEMPLATE_DEFAULT_INIT(),

    .albedo = "stylized_brick/stylized_bricks_albedo.jpg",
    .normal = "stylized_brick/stylized_bricks_normal.jpg",
    .roughn = "stylized_brick/stylized_bricks_roughness.jpg",
    .metall = NULL,
    .tint   = { 1, 1, 1 },
    .roughn_f = 0.65f,
    .metall_f = 0.0f,
    .shader_template = SHADER_TEMPLATE_NONE,
    .tile_scl = 1.0f,
    .tile_by_scl = true,
  },
  // paladin body
  {
    MATERIAL_TEMPLATE_DEFAULT_INIT(),
    .albedo = "devil_paladin_body/albedo.png",
    .normal = "devil_paladin_body/normal.png",
    .roughn = "devil_paladin_body/roughness.png",
    // .metall = "devil_paladin_body/metallic.png",
    .metall = NULL,
    .tint   = { 1, 1, 1 },
    .roughn_f = 1.0f,
    .metall_f = 0.0f,
    .shader_template = SHADER_TEMPLATE_NONE,
  },
  // paladin armour
  {
    MATERIAL_TEMPLATE_DEFAULT_INIT(),
    .albedo = "devil_paladin_armour/albedo.png",
    .normal = "devil_paladin_armour/normal.png",
    .roughn = "devil_paladin_armour/roughness.png",
    .metall = "devil_paladin_armour/metallic.png",
    .tint   = { 1, 1, 1 },
    .roughn_f = 1.0f,
    .metall_f = 1.0f,
    .shader_template = SHADER_TEMPLATE_NONE,
  },
  // paladin weapon
  {
    MATERIAL_TEMPLATE_DEFAULT_INIT(),
    .albedo = "devil_paladin_weapon/albedo.png",
    .normal = "devil_paladin_weapon/normal.png",
    .roughn = "devil_paladin_weapon/roughness.png",
    .metall = "devil_paladin_weapon/metallic.png",
    .tint   = { 1, 1, 1 },
    .roughn_f = 1.0f,
    .metall_f = 1.0f,
    .shader_template = SHADER_TEMPLATE_NONE,
  },
  // grid 
  {
    MATERIAL_TEMPLATE_DEFAULT_INIT(),
    .albedo = "grid_01.jpg",
    .normal = NULL,
    .roughn = NULL,
    .metall = NULL,
    .tint   = { 1, 1, 1 },
    .roughn_f = 0.3f,
    .metall_f = 0.0f,
    .shader_template = SHADER_TEMPLATE_NONE,
  },
}

#pragma GCC diagnostic pop // "-Winitializer-overrides" or -Woverride-init"
