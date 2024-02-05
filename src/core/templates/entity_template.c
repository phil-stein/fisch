#include "core/templates/entity_template.h"


const entity_template_t entity_template_empty = ENTITY_TEMPLATE_T_EMPTY();



const entity_template_t* entity_template_get(int idx)
{
  // P_INT(idx);

  if (idx == -1)  // empty entity template
  { return &entity_template_empty; }

  ERR_CHECK(idx > -1, "entity template requested not valid: %d\n", idx); // @TODO: check upper bound
  
  return &entity_template_table[idx];
}
const entity_template_t* entity_template_get_all(int* len)
{
  *len = entity_template_table_len;
  return entity_template_table;
}

