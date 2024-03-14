#ifndef MATERIAL_TABLE_H
#define MATERIAL_TABLE_H

#ifdef __cplusplus
extern "C" {
#endif

// @DOC: act as index for material_template_get()
//       ! if out of order materials will be loaded as the wrong material
typedef enum material_table_type
{
  MATERIAL_TEMPLATE_METALL,
  MATERIAL_TEMPLATE_GRASS,
  MATERIAL_TEMPLATE_PATH,
  MATERIAL_TEMPLATE_PLANKS,
  MATERIAL_TEMPLATE_BRICK,
  MATERIAL_TEMPLATE_MAX,

}material_table_type;

#ifdef __cplusplus
} // extern C
#endif


#endif  // ENTITY_TABLE_H
