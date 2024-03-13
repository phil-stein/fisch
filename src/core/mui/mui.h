#ifndef MUI_MUI_H
#define MUI_MUI_H

#include "core/io/input.h"
#include "core/renderer/renderer_direct.h"
#include "global/global.h"
#include "math/math_inc.h"
#include "core/types/texture.h"
#include "core/window.h"

#ifdef __cplusplus
extern "C" {
#endif

#define VIEW_SCL     10.0f
#define VIEW_SCL_INV (1.0f / VIEW_SCL)

typedef struct
{
  rgb color_text;
  rgb color_bg;   // idk some like dis

}mui_data;


typedef enum
{
  // -- general --

  MUI_UP     = FLAG(0),
  MUI_MIDDLE = FLAG(1),
  MUI_DOWN   = FLAG(2),

  MUI_LEFT   = FLAG(3),
  MUI_CENTER = FLAG(4),
  MUI_RIGHT  = FLAG(5),
  
  // -- group --

  MUI_DYNAMIC = FLAG(6), 
  MUI_STATIC  = FLAG(7),

  MUI_ROW     = FLAG(8),
  MUI_COLUMN  = FLAG(9),
  MUI_GRID    = FLAG(10),

}mui_orientation_type;

INLINE void P_MUI_ORIENTATION(mui_orientation_type o)     
{                               
  // PF("%s:", #o);                  
  PF("mui_orientation_type: ");
  if (HAS_FLAG(o, MUI_UP))     
  { PF("MUI_UP | "); }
  if (HAS_FLAG(o, MUI_MIDDLE)) 
  { PF("MUI_MIDDLE | "); } 
  if (HAS_FLAG(o, MUI_DOWN))   
  { PF("MUI_DOWN | "); }   

  if (HAS_FLAG(o, MUI_LEFT))   
  { PF("TEXT_LEFT | "); }   
  if (HAS_FLAG(o, MUI_CENTER)) 
  { PF("MUI_CENTER | "); } 
  if (HAS_FLAG(o, MUI_RIGHT))  
  { PF("MUI_RIGHT | "); }  
  
  PF("\n");
} 

typedef enum 
{ 
  MUI_OBJ_TEXT              = 1,  // have to start at 1 i think 
  MUI_OBJ_IMG,
  MUI_OBJ_SHAPE_RECT, 
  MUI_OBJ_SHAPE_CIRCLE,
  MUI_OBJ_SHAPE_RECT_ROUND,
  MUI_OBJ_ICON, 
  // MUI_OBJ_BUTTON,

} mui_obj_type;

#define MUI_OBJ_TEXT_MAX 128
typedef struct
{
  mui_obj_type type;
  bool active;        // determines if drawn or not

  vec2 pos;
  vec2 pos_original;
  
  int text[MUI_OBJ_TEXT_MAX];
  int text_len;
  mui_orientation_type orientation;
  
  // vec2 pos;
  vec2 scl;
  rgbf color;
  texture_t* tex;

  int asset_idx;

}mui_obj_t;
// @NOTE: text doesnt work like this
#define MUI_OBJ_T_INIT_TEXT(pos, text, orientation) \
{                                                   \
  .type   = MUI_OBJ_TEXT,                           \
  .active = true,                                   \
  .pos    = { (pos)[0], (pos)[1], (pos)[2] },       \
  .text   = (text),                                 \
  .orientation = (orientation),                     \
}

#define MUI_OBJ_T_INIT_QUAD(px, py, sx, sy, cr, cg, cb) \
{                                                       \
  .type  = MUI_OBJ_QUAD,                                \
  .active = true,                                       \
  .pos   = { (px), (py) },                              \
  .scl   = { (sx), (sy) },                              \
  .color = { (cr), (cg), (cb) },                        \
  .orientation = MUI_CENTER | MUI_MIDDLE,               \
}
#define MUI_OBJ_T_INIT_QUAD_GROUP(r, g, b)  MUI_OBJ_T_INIT_QUAD(0, 0,  1, 1,   (r), (g), (b)) 
#define MUI_OBJ_T_QUAD_GROUP(r, g, b)       (mui_obj_t)MUI_OBJ_T_INIT_QUAD(0, 0,  1, 1,   (r), (g), (b)) 

#define MUI_OBJ_T_INIT_IMG(px, py, sx, sy, _tex, cr, cg, cb) \
{                                                            \
  .type  = MUI_OBJ_IMG,                                      \
  .active = true,                                            \
  .pos   = { (px), (py) },                                   \
  .scl   = { (sx), (sy) },                                   \
  .color = { (cr), (cg), (cb) },                             \
  .tex   = (_tex),                                           \
  .orientation = MUI_CENTER | MUI_MIDDLE,                    \
}
#define MUI_OBJ_T_IMG(_px, _py, _sx, _sy, _tex, r, g, b)  (mui_obj_t)MUI_OBJ_T_INIT_IMG((_px), (_py), (_sx), (_sy), (_tex),  (r), (g), (b))
#define MUI_OBJ_T_INIT_IMG_GROUP(_tex, r, g, b)           MUI_OBJ_T_INIT_IMG(0, 0,  1, 1,  (_tex),  (r), (g), (b)) 
#define MUI_OBJ_T_IMG_GROUP(_tex, r, g, b)                (mui_obj_t)MUI_OBJ_T_INIT_IMG(0, 0,  1, 1,  (_tex),  (r), (g), (b))

#define MUI_OBJ_T_INIT_SHAPE(px, py, sx, sy, _type, cr, cg, cb)   \
{                                                                 \
  .type   = (_type),                                              \
  .active = true,                                                 \
  .pos    = { (px), (py) },                                       \
  .scl    = { (sx), (sy) },                                       \
  .color  = { (cr), (cg), (cb) },                                 \
  .orientation = MUI_CENTER | MUI_MIDDLE,                         \
}
#define MUI_OBJ_T_SHAPE(_px, _py, _sx, _sy, _type, r, g, b)  (mui_obj_t)MUI_OBJ_T_INIT_SHAPE((_px), (_py), (_sx), (_sy), (_type),  (r), (g), (b))
#define MUI_OBJ_T_INIT_SHAPE_GROUP(_type, r, g, b)           MUI_OBJ_T_INIT_SHAPE(0, 0,  1, 1,  (_type),  (r), (g), (b)) 
#define MUI_OBJ_T_SHAPE_GROUP(_type, r, g, b)                (mui_obj_t)MUI_OBJ_T_INIT_SHAPE(0, 0,  1, 1,  (_type),  (r), (g), (b))

#define MUI_OBJ_T_INIT_ICON(px, py, sx, sy, _asset_idx, cr, cg, cb)   \
{                                                                     \
  .type   = MUI_OBJ_ICON,                                             \
  .active = true,                                                     \
  .pos    = { (px), (py) },                                           \
  .scl    = { (sx), (sy) },                                           \
  .color  = { (cr), (cg), (cb) },                                     \
  .asset_idx = _asset_idx,                                            \
  .orientation = MUI_CENTER | MUI_MIDDLE,                             \
}

typedef struct
{
  vec2 pos;
  vec2 scl;
  f32  margin;
  bool scale_by_ratio; 

  rgbf  bg_color;
  bool  bg_visible;
  
  mui_obj_t* objs;
  int        objs_len;

  mui_orientation_type orientation;

  int max_wrap; // wrap after x mui_obj_t, -1 for no wrap

}mui_group_t;
#define MUI_GROUP_T_ADD(_group, _obj)           { arrput((_group)->objs, (_obj)); (_group)->objs_len++; }
#define MUI_GROUP_T_ADD_PTR(_group, _obj, _ptr)       \
      {                                               \
        arrput((_group)->objs, (_obj));               \
        (_ptr) = &(_group)->objs[(_group)->objs_len]; \
        (_group)->objs_len++;                         \
      }

INLINE void MUI_GROUP_T_INIT(mui_group_t* g, vec2 pos, vec2 scl, f32 margin, mui_orientation_type orientation, bool scale_by_ratio)  
{                                                           
  (g)->pos[0]         = (pos)[0];                              
  (g)->pos[1]         = (pos)[1];                              
  (g)->scl[0]         = (scl)[0];                              
  (g)->scl[1]         = (scl)[1];                              
  (g)->margin         = (margin);                              
  (g)->objs           = NULL;                                  
  (g)->objs_len       = 0;  
  (g)->orientation    = (orientation);                    
  (g)->max_wrap       = -1;  // no wrap
  (g)->scale_by_ratio = scale_by_ratio;
}


void mui_init();
void mui_update();


// @DOC: draw text 
int mui_text(vec2 pos, mui_orientation_type orientation, char* text);
#define mui_textf(_pos, _orientation, _text, ...)         \
  {                                                       \
    char _tmp[MUI_OBJ_TEXT_MAX];                          \
    SPRINTF(MUI_OBJ_TEXT_MAX, _tmp, _text, __VA_ARGS__);  \
    mui_text(_pos, _orientation, _tmp);                   \
  }
// void mui_text(ivec2 pos, ... txt, rgb color);

void mui_setup_obj(mui_obj_t* obj, bool scale_by_ratio);
int mui_add_obj(mui_obj_t* obj, bool scale_by_ratio);

INLINE int mui_img_complx(vec2 pos, vec2 scl, texture_t* tex, rgbf tint, bool scale_by_ratio)
{
  mui_obj_t obj = MUI_OBJ_T_INIT_IMG(pos[0], pos[1], scl[0], scl[1], tex, tint[0], tint[1], tint[2]);
  int idx = mui_add_obj(&obj, scale_by_ratio);
  return idx;
}
#define mui_img(pos, scl, tex)            mui_img_tint((pos), (scl), (tex), VEC3(1))
#define mui_img_tint(pos, scl, tex, tint) mui_img_complx((pos), (scl), (tex), (tint), false)

INLINE int mui_shape(vec2 pos, vec2 scl, rgbf color, mui_obj_type type, mui_orientation_type orientation, bool scale_by_ratio)
{
  mui_obj_t obj = MUI_OBJ_T_INIT_SHAPE(pos[0], pos[1], scl[0], scl[1], type, color[0], color[1], color[2]);
  obj.orientation = orientation;
  int idx = mui_add_obj(&obj, scale_by_ratio);
  return idx;
}
#define mui_circle(_pos, _scl, _color)      mui_shape((_pos), (_scl), (_color), MUI_OBJ_SHAPE_CIRCLE,     MUI_CENTER | MUI_MIDDLE, false)
#define mui_rect(_pos, _scl, _color)        mui_shape((_pos), (_scl), (_color), MUI_OBJ_SHAPE_RECT,       MUI_CENTER | MUI_MIDDLE, false)
#define mui_rect_round(_pos, _scl, _color)  mui_shape((_pos), (_scl), (_color), MUI_OBJ_SHAPE_RECT_ROUND, MUI_CENTER | MUI_MIDDLE, false)
#define mui_circle_oriented(_pos, _scl, _color, _orientation)      mui_shape((_pos), (_scl), (_color), MUI_OBJ_SHAPE_CIRCLE,     _orientation, false)
#define mui_rect_oriented(_pos, _scl, _color, _orientation)        mui_shape((_pos), (_scl), (_color), MUI_OBJ_SHAPE_RECT,       _orientation, false)
#define mui_rect_round_oriented(_pos, _scl, _color, _orientation)  mui_shape((_pos), (_scl), (_color), MUI_OBJ_SHAPE_RECT_ROUND, _orientation, false)

INLINE int mui_icon_complex(vec2 pos, vec2 scl, rgbf color, int asset_idx, bool scale_by_ratio)
{
  mui_obj_t obj = MUI_OBJ_T_INIT_ICON(pos[0], pos[1], scl[0], scl[1], asset_idx, color[0], color[1], color[2]);
  int idx = mui_add_obj(&obj, scale_by_ratio);
  return idx;
}
#define mui_icon(_pos, _scl, _color, _asset_idx) mui_icon_complex(_pos, _scl, _color, _asset_idx, false)

bool mui_button_complex(vec2 pos, vec2 scl, rgbf color, mui_obj_type type, char* text, int icon_idx);
#define mui_button(_pos, _scl, _color, _text)     mui_button_complex(_pos, _scl, _color, MUI_OBJ_TEXT, _text, -1)
#define mui_button_icon(_pos, _scl, _color, _idx) mui_button_complex(_pos, _scl, _color, MUI_OBJ_ICON, NULL, _idx)

// void mui_space();
void mui_group(mui_group_t* g);

// -- draw --

// void mui_draw_shape(vec2 cam_pos, f32 cam_zoom, vec2 pos, vec2 size, rgbf color, mui_obj_type type);
void mui_draw_shape(mat4 view, mat4 proj, vec2 pos, vec2 size, rgbf color, mui_obj_type type);
void mui_draw_icon(mat4 view, mat4 proj, vec2 pos, vec2 size, rgbf color, int asset_idx);

INLINE bool mui_mouse_over_obj(mui_obj_t* obj)
{
  f64 _x, _y;
  input_get_mouse_pos_normalized(&_x, &_y);
  f32 x = (f32)_x;
  f32 y = (f32)_y;
  x = x*2 -1;
  y = y*2 -1;
  y *= -1;    // flip y, is different in mouse

  // orinetation & scaling 
  int win_w, win_h;
  window_get_size(&win_w, &win_h);
  // f32 r_wh = ((f32)win_w / (f32)win_h);
  f32 r_hw = ((f32)win_h / (f32)win_w);

  // f32 w = fabs(obj->scl[0]) * VIEW_SCL_INV * 1.35f;
  // f32 h = fabs(obj->scl[1]) * VIEW_SCL_INV * 1.35f;
  f32 w = fabsf(obj->scl[0]) * VIEW_SCL_INV * r_hw * 2.0f;
  f32 h = fabsf(obj->scl[1]) * VIEW_SCL_INV * 1.35f;
  // f32 px = obj->pos[0] - w;
  // f32 py = obj->pos[1] - h;
  // f32 px = obj->pos_original[0] - (obj->scl[0] * VIEW_SCL_INV);
  // f32 py = obj->pos_original[1] - (obj->scl[1] * VIEW_SCL_INV);
  f32 px = obj->pos_original[0] -w;
  f32 py = obj->pos_original[1] -h;
  w *= 2;
  h *= 2;

  // mui_circle(VEC2_XY(px, py), VEC2(0.1f), RGB_F(1, 0, 0));
  // // mui_circle(obj->pos, VEC2(0.1f), RGB_F(0, 1, 0));
  // mui_circle(obj->pos_original, VEC2(0.1f), RGB_F(1, 0, 1));
  // mui_circle(VEC2_XY(px +w, py +h), VEC2(0.1f), RGB_F(0, 0, 1));

  // printf("mouse x:  %.2f y:  %.2f\n", x, y);
  // printf("      x:  %.2f y:  %.2f\n", obj->pos[0], obj->pos[1]);
  // printf("      x:  %.2f y:  %.2f\n", obj->pos_original[0], obj->pos_original[1]);
  // printf("      px: %.2f py: %.2f\n", px, py);
  // printf("      w:  %.2f h:  %.2f\n", w, h);
  
  return POINT_IN_RECT(x, y, px, py, w, h);
}

// --- style ---

typedef struct
{
  int font_size;
  int font_x_size_dif;
  int font_s_size_dif;
  int font_m_size_dif;
  int font_l_size_dif;

  rgbf button_normal;
  rgbf button_hover;
  rgbf button_click;
  rgbf button_border;
  rgbf button_icon;
  f32  button_icon_size;
  f32  button_border_width;

} mui_style_t;
#define MUI_STYLE_T_INIT()                \
{                                         \
  .font_size       = 14,                  \
  .font_x_size_dif = -4,                  \
  .font_s_size_dif = -2,                  \
  .font_m_size_dif =  0,                  \
  .font_l_size_dif =  2,                  \
                                          \
  .button_normal = { 0.5f, 0.5f, 0.5f },  \
  .button_hover  = { 1.0f, 1.0f, 1.0f },  \
  .button_click  = { 1.0f, 0.0f, 0.0f },  \
  .button_border = { 0.8f, 0.8f, 0.8f },  \
  .button_icon   = { 0.8f, 0.8f, 0.8f },  \
  .button_icon_size    = 0.25f,           \
  .button_border_width = 0.05f,           \
}
extern mui_style_t* mui_style;
INLINE void mui_set_button_style(rgbf _button_normal, rgbf _button_hover, rgbf _button_click)
{
  vec3_copy(_button_normal, mui_style->button_normal);
  vec3_copy(_button_hover,  mui_style->button_hover);
  vec3_copy(_button_click,  mui_style->button_click);
}
INLINE void mui_set_style(rgbf _button_normal, rgbf _button_hover, rgbf _button_click)
{
  mui_set_button_style(_button_normal, _button_hover, _button_click);
}



#ifdef __cplusplus
} // extern c
#endif

#endif  // MUI_MUI_H
