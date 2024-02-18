#ifndef CORE_TEXT_TEXT_INC_H
#define CORE_TEXT_TEXT_INC_H

#include "core/types/texture.h"
#include "global/global.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FONT_NAME_MAX 64

typedef struct
{
  char name[FONT_NAME_MAX];
  // int  atlas; // assetm idx
  texture_t atlas;

} font_t;


void text_init();
// @DOC: name is relative to core_data->assetpath/fonts/<name>
void text_load_font(const char* name, font_t* _font);
void text_draw_char();

#ifdef __cplusplus
} // extern c
#endif

#endif // CORE_TEXT_TEXT_INC_H
