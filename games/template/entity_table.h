#ifndef ENTITY_TABLE_H
#define ENTITY_TABLE_H

#ifdef __cplusplus
extern "C" {
#endif

// @DOC: act as index for entity_template_get()
//       ! if out of order entities will be loaded as the wrong entity
typedef enum entity_template_type 
{
  ENTITY_TEMPLATE_QUAD,
  ENTITY_TEMPLATE_SPHERE01,
  ENTITY_TEMPLATE_SPHERE02,
  ENTITY_TEMPLATE_CUBE_STATIC,
  ENTITY_TEMPLATE_CUBE_DYNAMIC,
  ENTITY_TEMPLATE_CUBE_STATIC_TRIGGER,
  ENTITY_TEMPLATE_SPHERE_DYN,
  ENTITY_TEMPLATE_MAX,

}entity_template_type;


#ifdef __cplusplus
} // extern C
#endif


#endif  // ENTITY_TABLE_H
