#include "global/global.h"
#include "math/math_inc.h"
#include "games.h"  // ./games/games.h
#include "core/ecs_manager.h"
#include "test/material_table.h"
#include "test/test_comp.h"

void __game_init__()
{
  // --- test ---

  ecs_entity_t e1 = ecs_manager_add_entity("entity_01", 
                                           VEC3_XYZ(-1.75f, -4, -5), VEC3(0), VEC3(1), 
                                           MATERIAL_TEMPLATE_DEMON01, "demon01");
  ecs_manager_entity_add_pointlight(e1, VEC3_XYZ(0, 2, 0), RGB_F(1, 0, 0), 1.0f);
  // --------
  ecs_entity_t e2 = ecs_manager_add_entity("entity_02", 
                                           VEC3_XYZ(0, 0, -5), VEC3(0), VEC3(0.5f), 
                                           MATERIAL_TEMPLATE_SHOTGUN, "shotgun");
  ecs_manager_entity_add_pointlight(e2, VEC3_XYZ(0, 1, 0), RGB_F(0, 1, 0), 1.0f);
  ecs_manager_entity_add_logic(e2, entity_init, entity_update, NULL); 
  // --------
  ecs_entity_t e3 = ecs_manager_add_entity("entity_03", 
                                           VEC3_XYZ(2, 0, -5), VEC3(0), VEC3(0.5f),
                                           MATERIAL_TEMPLATE_METALL, "sphere");
  // --------
  ecs_entity_t e4 = ecs_manager_add_entity_empty("entity_04", 
                                                 VEC3_XYZ(0, 0, -0), VEC3(0), VEC3(1)); 
  // --- child/parent ---
  ecs_manager_set_parent(e3, e2);
  // ------------
}
