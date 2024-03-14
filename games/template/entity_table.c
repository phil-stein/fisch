#include "core/templates/entity_template.h"
#include "test/entity_table.h"
#include "test/material_table.h"
#include "test/entity_tags.h"
#include "test/scripts.h"

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
  ENTRY(CUBE_STATIC)
    .mesh = "cube",
    .mat  = MATERIAL_TEMPLATE_BRICK, // _PLANKS, // _PATH, // _METALL,
    .phys_flag = ENTITY_HAS_BOX,
    .aabb_size  = { 1, 1, 1 },
  };
  ENTRY(CUBE_DYNAMIC)
    .tags_flag = TAG_CUBE_DYN, 
    .mesh = "cube",
    .mat  = MATERIAL_TEMPLATE_PLANKS,
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
  ENTRY(SPHERE_DYN)
    .mesh = "sphere",
    .mat  = MATERIAL_TEMPLATE_DEFAULT,
    .phys_flag = ENTITY_HAS_RIGIDBODY | ENTITY_HAS_SPHERE,
    .mass = 20.0f,
    .friction = 0.05f, 
    .radius    = 1.0f,
  };
}

#pragma GCC diagnostic pop // "-Winitializer-overrides" or -Woverride-init"

