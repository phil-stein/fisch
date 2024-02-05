#include "core/templates/entity_template.h"
#include "test/material_table.h"
#include "test/entity_tags.h"
#include "test/test_comp.h"

const entity_template_t entity_template_table[] = 
{
  {
    ENTITY_TEMPLATE_T_SET_DEFAULTS(),
    .name = "demon01",
    .mesh = "demon01",
    .mat  = MATERIAL_TEMPLATE_DEMON01,
  },
  {
    ENTITY_TEMPLATE_T_SET_DEFAULTS(),
    .name = "shotgun",
    .mesh = "shotgun",
    .mat  = MATERIAL_TEMPLATE_SHOTGUN,
    // .update_f = entity_update,
  },
  {
    ENTITY_TEMPLATE_T_SET_DEFAULTS(),
    .name = "demon02",
    .mesh = "demon02",
    .mat  = MATERIAL_TEMPLATE_DEMON02,
  },
  {
    ENTITY_TEMPLATE_T_SET_DEFAULTS(),
    .name = "stone01",
    .mesh = "stones/stone01_ld02_tri",
    .mat  = MATERIAL_TEMPLATE_STONE01,
  },
  {
    ENTITY_TEMPLATE_T_SET_DEFAULTS(),
    .name = "tree01",
    .mesh = "trees/tree01_ld_tri",
    .mat  = MATERIAL_TEMPLATE_TREE01,
  },
  {
    ENTITY_TEMPLATE_T_SET_DEFAULTS(),
    .name = "hut_test",
    .mesh = "base_hut",
    .mat  = MATERIAL_TEMPLATE_DEFAULT,
 },
  {
    ENTITY_TEMPLATE_T_SET_DEFAULTS(),
    .name = "devil_paladin_body",
    .mesh = "mooh_body01",
    .mat  = MATERIAL_TEMPLATE_PALADIN_BODY,
  },
  {
    ENTITY_TEMPLATE_T_SET_DEFAULTS(),
    .name = "devil_paladin_armour",
    .mesh = "mooh_armour01",
    .mat  = MATERIAL_TEMPLATE_PALADIN_ARMOUR,
  },
  {
    ENTITY_TEMPLATE_T_SET_DEFAULTS(),
    .name = "devil_paladin_weapon",
    .mesh = "mooh_weapon01",
    .mat  = MATERIAL_TEMPLATE_PALADIN_WEAPON,
  },
};
extern const int entity_template_table_len = sizeof(entity_template_table) / sizeof(entity_template_table[0]);
