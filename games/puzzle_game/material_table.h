#ifndef MATERIAL_TABLE_H
#define MATERIAL_TABLE_H

#ifdef __cplusplus
extern "C" {
#endif

// @DOC: act as index for material_template_get()
//       ! if out of order materials will be loaded as the wrong material
typedef enum material_table_type
{
  MATERIAL_TEMPLATE_METALL              = 0,
  MATERIAL_TEMPLATE_DEMON01             = 1,
  MATERIAL_TEMPLATE_DEMON02             = 2,
  MATERIAL_TEMPLATE_SHOTGUN             = 3,
  MATERIAL_TEMPLATE_GRASS               = 4,
  MATERIAL_TEMPLATE_STONE01             = 5,
  MATERIAL_TEMPLATE_TREE01              = 6,
  MATERIAL_TEMPLATE_PATH                = 7,
  MATERIAL_TEMPLATE_PLANKS              = 8,
  MATERIAL_TEMPLATE_BRICK               = 9,
  MATERIAL_TEMPLATE_PALADIN_BODY        = 10,
  MATERIAL_TEMPLATE_PALADIN_ARMOUR      = 11,
  MATERIAL_TEMPLATE_PALADIN_WEAPON      = 12,
  MATERIAL_TEMPLATE_ROBOT_CHARACTER_06  = 13,
  MATERIAL_TEMPLATE_FEMALE_CHAR_01      = 14,
  MATERIAL_TEMPLATE_GRID                = 15,
  MATERIAL_TEMPLATE_PLANT_01            = 16,
  MATERIAL_TEMPLATE_MAX,

}material_table_type;


void material_table_init();

#ifdef __cplusplus
} // extern C
#endif


#endif  // ENTITY_TABLE_H
