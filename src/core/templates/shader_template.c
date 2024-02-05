#include "core/templates/shader_template.h"
#include "core/types/shader.h"


const shader_template_t* shader_template_get(int idx)
{
  ERR_CHECK(idx < shader_table_len, "shader template idx too high: %d", idx);
  ERR_CHECK(idx >= 0,                "shader template idx too low: %d", idx);
  return &shader_table[idx]; 
}

const shader_template_t* shader_template_get_all(int* len)
{
  *len = shader_table_len;
  return shader_table;
}



