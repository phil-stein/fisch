#ifndef ENTITY_TABLE_H
#define ENTITY_TABLE_H

#ifdef __cplusplus
extern "C" {
#endif

// @DOC: act as index for entity_template_get()
//       ! if out of order entities will be loaded as the wrong entity
typedef enum entity_template_type 
{
  ENTITY_TEMPLATE_QUAD                = 0,
  ENTITY_TEMPLATE_SPHERE01            = 1,
  ENTITY_TEMPLATE_SPHERE02            = 2,
  ENTITY_TEMPLATE_DEMON01             = 3,
  ENTITY_TEMPLATE_SHOTGUN             = 4,
  ENTITY_TEMPLATE_DEMON02             = 5,
  ENTITY_TEMPLATE_STONE01             = 6,
  ENTITY_TEMPLATE_HUT_TEST            = 7,
  ENTITY_TEMPLATE_CUBE_STATIC         = 8,
  ENTITY_TEMPLATE_CUBE_DYNAMIC        = 9,
  ENTITY_TEMPLATE_CUBE_STATIC_TRIGGER = 10,
  ENTITY_TEMPLATE_PALADIN_BODY        = 11,
  ENTITY_TEMPLATE_PALADIN_ARMOUR      = 12,
  ENTITY_TEMPLATE_PALADIN_WEAPON      = 13,
  ENTITY_TEMPLATE_SPHERE_DYN          = 14,
  ENTITY_TEMPLATE_ROBOT_CHARACTER_06  = 15,
  ENTITY_TEMPLATE_FEMALE_CHAR_01      = 16,
  ENTITY_TEMPLATE_FPS                 = 17,
  ENTITY_TEMPLATE_POWER_LEVER_01      = 18,
  ENTITY_TEMPLATE_POWER_LEVER_02      = 19,
  ENTITY_TEMPLATE_DOOR_01             = 20,
  ENTITY_TEMPLATE_MAX,

}entity_template_type;


#ifdef __cplusplus
} // extern C
#endif


#endif  // ENTITY_TABLE_H
