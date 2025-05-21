#include "core/mui/mui.h"
#include "core/audio/audio.h"
#include "core/io/input.h"
#include "core/types/framebuffer.h"
#include "core/window.h"
#include "core/io/assetm.h"
#include "core/types/shader.h"
#include "core/core_data.h"
#include "core/renderer/renderer_direct.h"
#include "core/debug/debug_opengl.h"
#include "global/global_print.h"
#include "math/math_vec2.h"
#include "text/text_inc.h"

#include "stb/stb_ds.h"

// #include <ctype.h>
// #include <direct.h>

glyph* g_full = NULL;

// #define FONT_SIZE_STEP  1
// int  font_size = 14;

font_t font_x = FONT_INIT();
font_t font_s = FONT_INIT();
font_t font_m = FONT_INIT();
font_t font_l = FONT_INIT();
// #define FONT_X_SIZE_DIF  -4
// #define FONT_S_SIZE_DIF  -2
// #define FONT_M_SIZE_DIF   0
// #define FONT_L_SIZE_DIF   2

font_t* font_main;

mui_obj_t* obj_arr = NULL;
u32        obj_arr_len = 0;

static mat4 proj;
static mat4 view;

shader_t  circle_shader;
shader_t  rounded_shader;

mui_style_t  mui_style_data = MUI_STYLE_T_INIT();
mui_style_t* mui_style = &mui_style_data;

void mui_init()
{
  // char* _cwd = _getcwd(NULL, 0);
  // ASSERT(_cwd != NULL);
  // strcpy(cwd, _cwd);
  // P_STR(cwd);
  
  // --- shapes init ---

  char vert_path[ASSET_PATH_MAX + 64];
  char frag_path[ASSET_PATH_MAX + 64];
  SPRINTF(ASSET_PATH_MAX + 64, vert_path, "%sshaders/mui/mui.vert", core_data->asset_path);
  SPRINTF(ASSET_PATH_MAX + 64, frag_path, "%sshaders/mui/circle.frag", core_data->asset_path);
  circle_shader = shader_create_from_file(vert_path, frag_path, NULL, "mui_circle_shader");
  SPRINTF(ASSET_PATH_MAX + 64, frag_path, "%sshaders/mui/rounded.frag", core_data->asset_path);
  rounded_shader = shader_create_from_file(vert_path, frag_path, NULL, "mui_rounded_shader");
  
  // --- text init ---
 
  char path_0[ASSET_PATH_MAX + 64];
  SPRINTF(ASSET_PATH_MAX + 64, path_0, "%s%s", core_data->asset_path, "fonts/JetBrainsMonoNL-Regular.ttf");

  text_load_font(path_0, mui_style->font_size + mui_style->font_x_size_dif, &font_x);
  text_load_font(path_0, mui_style->font_size + mui_style->font_s_size_dif, &font_s);
  text_load_font(path_0, mui_style->font_size + mui_style->font_m_size_dif, &font_m);
  text_load_font(path_0, mui_style->font_size + mui_style->font_l_size_dif, &font_l);

  font_main     = &font_m;
    
  char path_1[ASSET_PATH_MAX + 64];
  SPRINTF(ASSET_PATH_MAX + 64, path_0, "%s%s", core_data->asset_path, "shaders/text/text.vert");
  SPRINTF(ASSET_PATH_MAX + 64, path_1, "%s%s", core_data->asset_path, "shaders/text/text.frag");
  bool err = false;
  // u32 text_shader = shader_create(path_0, path_1, "text_shader", &err);
  shader_t _text_shader = shader_create_from_file(path_0, path_1, NULL, "text_shader");
  u32       text_shader = _text_shader.handle;
  // u32 text_shader = shader_create("_assets/shaders/text.vert", 
	// 				                        "_assets/shaders/text.frag", 
	// 				                        "text_shader", &err);
  ASSERT(!err);
  // u32 img_shader  = shader_create(ASSET_PATH"shaders/text.vert", 
	// 				                        ASSET_PATH"shaders/text.frag", 
	// 				                        "text_shader", &err);
  u32 blank_tex   = assetm_get_texture("#internal/blank.png", true)->handle;
    
  text_draw_init(font_main, 
                 text_shader, text_shader, 
                 blank_tex,
                 window_get_size,
                 shader_use_handle, shader_set_int_handle_no_dbg,
                 shader_set_vec2_handle_no_dbg, shader_set_vec3_handle_no_dbg);

  // ------------------

  g_full = text_get_glyph(U_FULL, font_main);

  // input_register_key_callback(app_key_callback);
  // input_register_utf8_callback(app_utf8_callback);

  // int w, h;
  // window_get_size(&w, &h);
  // P_INT(w);
  // P_INT(h);
  // float dpi_x, dpi_y;
  // window_get_monitor_dpi(&dpi_x, &dpi_y);
  // P_F32(dpi_x);
  // P_F32(dpi_y);

  // view & proj mat for _draw() funcs
  mat4_lookat_2d(VEC2(0), VIEW_SCL, view);
  int w, h;
  window_get_size(&w, &h);
  camera_get_proj_mat(w, h, proj);
}

void mui_update()
{
  core_data->mui.mouse_over_mui = false;

  // @NOTE: no blendd-type specified because same as renderer.c
  framebuffer_unbind();
  _glEnable(GL_BLEND);
  _glDisable(GL_CULL_FACE);
  _glDisable(GL_DEPTH_TEST);
  core_data->opengl_state |= OPENGL_BLEND;
  REMOVE_FLAG(core_data->opengl_state, (opengl_state_flag)OPENGL_CULL_FACE);
  REMOVE_FLAG(core_data->opengl_state, (opengl_state_flag)OPENGL_DEPTH_TEST);

  
  for (int i = 0; i < (int)obj_arr_len; ++i)
  {
    mui_obj_t* o = &obj_arr[i];
    bool hover = mui_mouse_over_obj(o);
    core_data->mui.mouse_over_mui |= hover;
   
    switch (o->type)
    {
      case MUI_OBJ_TEXT:
        text_draw_line(o->pos, o->text, o->text_len, font_main);
        break;
      case MUI_OBJ_IMG:
        // @TODO: make mui_draw_img() func
        renderer_direct_draw_quad_textured(VEC2(0), 10.0f, o->pos, o->scl, o->tex, o->color);
        break;
      case MUI_OBJ_SHAPE_RECT:
      case MUI_OBJ_SHAPE_RECT_ROUND:
      case MUI_OBJ_SHAPE_CIRCLE:
        mui_draw_shape(view, proj, o->pos, o->scl, o->color, o->type);
        break;
      case MUI_OBJ_ICON:
        mui_draw_icon(view, proj, o->pos, o->scl, o->color, o->asset_idx);
        break;

      default:
        break;
    }

  }
  if (obj_arr_len > 0)
  {
    arrfree(obj_arr);
    obj_arr_len = 0;
  }

  // @TMP: testijng shapes
  // mui_draw_shape(VEC2(0), 10.0f, VEC2_Y(2.5f), VEC2(1), RGB_F(0, 1, 1), MUI_OBJ_SHAPE_CIRCLE);
  // mui_draw_shape(VEC2(0), 10.0f, VEC2(0), VEC2(1), RGB_F(0, 1, 1), MUI_OBJ_SHAPE_RECT_ROUND);
  // mui_draw_shape(VEC2(0), 10.0f, VEC2_X(3.5f), VEC2_XY(2, 1), RGB_F(0, 1, 1), MUI_OBJ_SHAPE_RECT_ROUND);
  // mui_draw_shape(VEC2(0), 10.0f, VEC2_X(-3.5f), VEC2_XY(1, 2), RGB_F(0, 1, 1), MUI_OBJ_SHAPE_RECT_ROUND);
  // mui_draw_icon(view, proj, VEC2_XY(0.35f, -0.25f), VEC2_XY(1.0f, 0.5f), VEC3(0.5f), assetm_get_mesh_idx("robot_character_06_01"));
  // mui_draw_icon(view, proj, VEC2_XY(0.35f, -0.25f), VEC2_XY(-1.0f, 1.0f), VEC3(0.5f), assetm_get_mesh_idx("icons/play_icon"));

  _glEnable(GL_CULL_FACE);
  _glEnable(GL_DEPTH_TEST);
  core_data->opengl_state |= OPENGL_CULL_FACE;
  core_data->opengl_state |= OPENGL_DEPTH_TEST;
}

int mui_text(vec2 pos, mui_orientation_type orientation, char* text)
{

#ifdef ASSERT_FIX_USE_FIX
  // printf("ASSERT_FIX_USE_FIX");
#endif
  // ERR_CHECK(text != NULL, "text passed to mui_text() is null: %p\n", (void*)text);
  ASSERT_FIX(text != NULL, 
    char null_str[] = "null";
    text = (char*)null_str;
    printf("used null fix\n");
  );

  int len = (int)strlen(text);
  // ASSERT(len >= 0); 
  // ASSERT(len < MUI_OBJ_TEXT_MAX);
  ASSERT_FIX(len >= 0, 
    printf("starting empty fix\n");
    char empty_str[] = "empty";
    text = (char*)empty_str;
    len = (int)strlen(text);
    printf("used empty fix\n");
  );
  ASSERT_FIX(len < MUI_OBJ_TEXT_MAX, 
    // printf("len: %d\n", (int)strlen("1234"));
    // printf("starting too long fix, len: %d\n", len);
    // text[MUI_OBJ_TEXT_MAX -4] = '.';
    // text[MUI_OBJ_TEXT_MAX -3] = '.';
    // text[MUI_OBJ_TEXT_MAX -2] = '.';
    // text[MUI_OBJ_TEXT_MAX -1] = '\0';
    // len = MUI_OBJ_TEXT_MAX -1;
    char empty_str[] = "too long";
    text = (char*)empty_str;
    len = (int)strlen(text);
    printf("used too long fix\n");
  );
 
  // P_TEXT_ORIENTATION(orientation);
  ERR_CHECK(!((HAS_FLAG(orientation, MUI_UP)     && HAS_FLAG(orientation, MUI_MIDDLE)) ||
              (HAS_FLAG(orientation, MUI_UP)     && HAS_FLAG(orientation, MUI_DOWN))   ||
              (HAS_FLAG(orientation, MUI_MIDDLE) && HAS_FLAG(orientation, MUI_DOWN))),
              "can only have one of MUI_UP, MUI_MIDDLE or MUI_DOWN");
  ERR_CHECK(!((HAS_FLAG(orientation, MUI_LEFT)   && HAS_FLAG(orientation, MUI_CENTER)) ||
              (HAS_FLAG(orientation, MUI_LEFT)   && HAS_FLAG(orientation, MUI_RIGHT))  ||
              (HAS_FLAG(orientation, MUI_CENTER) && HAS_FLAG(orientation, MUI_RIGHT))),
              "can only have one of MUI_LEFT, MUI_CENTER or MUI_RIGHT");

  mui_obj_t o;
  o.type        = MUI_OBJ_TEXT; 
  o.text_len    = len;
  o.orientation = orientation;

  vec2_copy(pos, o.pos);
  
  // convert to int array
  for (int i = 0; i < len; ++i)
  { o.text[i] = (int)text[i]; }

  // // adjust height and width
  // int w, h;
  // window_get_size(&w, &h);
  // // f32 r_wh = ((f32)w / (f32)h);
  // // f32 r_hw = ((f32)h / (f32)w);
  // o.pos[1] *= -1.0f;
  // // o.pos[0] *= r_wh * 1.0f;
  // // o.pos[1] *= 4.0f;
  // // vec2_mul_f(o.pos, 2.0f, o.pos);
  // // // vec2_mul_f(pos, 1.0f, pos);
  // // vec2_sub_f(o.pos, 1.0f, o.pos);
  // vec2_add_f(o.pos, 1.0f, o.pos);
  // // flip y 
  // o.pos[1] *= -1.0f;
  // // if (o.pos[1] <= 0.0f)
  // // { o.pos[1] -= font_main->gh; }
  // // else 
  // // { o.pos[1] += font_main->gh; }
  // // // pos[1] += font_main->gh;
  // // printf("%.2f, %.2f\n", o.pos[0], o.pos[1]);
  // // to pixel coordinates
  // o.pos[0] *= (f32)w;
  // o.pos[1] *= (f32)h;
  // // mui_setup_obj(&o, false);
  // // vec2_mul_f(o.pos, 0.5f, o.pos);
  // // vec2_add_f(o.pos, 0.5f, o.pos);
  
  // // adjust height and width
  // int w, h;
  // window_get_size(&w, &h);
  // //  f32 r_wh = ((f32)w / (f32)h);
  // //  f32 r_hw = ((f32)h / (f32)w);
  // o.pos[1] *= -1.0f ;
  // // vec2_mul_f(pos, 2, pos);
  // // vec2_mul_f(pos, 1.0f, pos);
  // vec2_add_f(o.pos, 1, o.pos);
  // o.pos[0] *= w;
  // o.pos[1] *= h;
  // // flip y 
  // o.pos[1] *= -1.0f ;
  // o.pos[1] -= font_main->gh;
  // // o.pos[1] += font_main->gh;
  
  // // adjust height and width
  // int w, h;
  // window_get_size(&w, &h);
  // // vec2_add_f(o.pos, 1.0f, o.pos);
  // // vec2_mul_f(o.pos, 0.5f, o.pos);
  // o.pos[0] += 1.0f;
  // // pixel-space
  // o.pos[0] *= w;
  // o.pos[1] *= h;
  
  // adjust height and width
  int w, h;
  window_get_size(&w, &h);
  o.pos[1] *= -1.0f ;
  
  // @TODO: @UNSURE: idk why this is necessary
  o.pos[0] *= 1.02f;
  // o.pos[1] *= 0.95f;
  
  vec2_add_f(o.pos, 1, o.pos);
  
  // o.pos[0] *= 1.0f / 0.75f;
  // o.pos[1] *= 0.4f;
  // o.pos[0] *= 1.25f;

  o.pos[0] *= (f32)w;
  o.pos[1] *= (f32)h;
  // flip y 
  o.pos[1] *= -1.0f ;
  o.pos[1] -= (f32)font_main->gh;

  // @TODO: @UNSURE: idk why this is necessary
  o.pos[1] *= 0.98f;
  
  // if (HAS_FLAG(orientation, MUI_RIGHT))
  if (HAS_FLAG(orientation, MUI_LEFT))
  { o.pos[0] -= (f32)(font_main->gw * len); }
  else if (HAS_FLAG(orientation, MUI_CENTER)) 
  { o.pos[0] -= (f32)(font_main->gw * len) * 0.5f; }

  // if no flag
  if(!HAS_FLAG(orientation, MUI_UP) && !HAS_FLAG(orientation, MUI_MIDDLE) && 
     !HAS_FLAG(orientation, MUI_DOWN))
  { orientation |= MUI_UP; }

  if (HAS_FLAG(orientation, MUI_UP)) 
  { o.pos[1] += (f32)font_main->gh; }
  else if (HAS_FLAG(orientation, MUI_MIDDLE))
  // { o.pos[1] -= font_main->gh * 0.5f; }
  { o.pos[1] += (f32)font_main->gh * 0.5f; }
  // else if (HAS_FLAG(orientation, MUI_DOWN))
  // // { o.pos[1] -= font_main->gh; }
  // { o.pos[1] -= font_main->gh * 0.5f; }


  // text_draw_line(pos, text_buffer, len, font_main);
  arrput(obj_arr, o);
  obj_arr_len++;
  return (int)obj_arr_len -1;
}

bool mui_button_complex(vec2 pos, vec2 scl, rgbf color, mui_obj_type type, char* text, int icon_idx)
{
  // // if (scale_by_ratio) { P_ERR("scale_by_ratio in %s doesnt do anything\n", __func__); }
  // mui_obj_t* obj = mui_text(pos, text, orientation);
  // vec2_copy(obj->pos, obj->pos_02); // use pos_02 for text
  // obj->type = MUI_OBJ_BUTTON;
  // vec2_copy(pos,   obj->pos); 
  // vec2_copy(scl,   obj->scl);
  // vec3_copy(color, obj->color);
  // mui_setup_obj(obj, scale_by_ratio);
 
  mui_shape(pos, scl, mui_style->button_border, MUI_OBJ_SHAPE_RECT_ROUND, MUI_CENTER | MUI_MIDDLE, false);
  vec2_sub_f(scl, mui_style->button_border_width, scl);
  int rect_idx = mui_shape(pos, scl, color, MUI_OBJ_SHAPE_RECT_ROUND, MUI_CENTER | MUI_MIDDLE, false);
  if (type == MUI_OBJ_TEXT)
  {
    vec2 text_pos = { 0 };
    vec2_copy(pos, text_pos);
    mui_text(text_pos, MUI_CENTER | MUI_MIDDLE, text);
    //  mui_text(text_pos, text, MUI_CENTER | MUI_DOWN);
  }
  else if (type == MUI_OBJ_ICON)
  {
    vec2 _pos = { 0 };
    vec2_copy(pos, _pos);
    // vec2_copy(scl, _scl);
    mui_icon(_pos, VEC2(mui_style->button_icon_size), mui_style->button_icon, icon_idx);
  }

  ASSERT(rect_idx >= 0);
  mui_obj_t* rect = &obj_arr[rect_idx];
  bool hover = mui_mouse_over_obj(rect);
  bool click = hover && input_get_mouse_down(MOUSE_BUTTON1);
  
  if (click) 
  { vec3_copy(mui_style->button_click,  rect->color); }
  else if (hover) 
  { vec3_copy(mui_style->button_hover,  rect->color); }
  else 
  { vec3_copy(mui_style->button_normal, rect->color); }
 
  bool pressed = hover && input_get_mouse_pressed(MOUSE_BUTTON1);
  if (pressed && AUDIO_IDX_IS_VALID(core_data->mui.button_click_sound))
  {
    audio_play_sound(core_data->mui.button_click_sound, core_data->mui.button_click_sound_volume);
  }
  return pressed; 
}


void mui_setup_obj(mui_obj_t* obj, bool scale_by_ratio)
{
  if (!obj->active) { return; }
  // P_TEXT_ORIENTATION(orientation);
  ERR_CHECK(!((HAS_FLAG(obj->orientation, MUI_UP)     && HAS_FLAG(obj->orientation, MUI_MIDDLE)) ||
              (HAS_FLAG(obj->orientation, MUI_UP)     && HAS_FLAG(obj->orientation, MUI_DOWN))   ||
              (HAS_FLAG(obj->orientation, MUI_MIDDLE) && HAS_FLAG(obj->orientation, MUI_DOWN))),
              "can only have one of MUI_UP, MUI_MIDDLE or MUI_DOWN");
  ERR_CHECK(!((HAS_FLAG(obj->orientation, MUI_LEFT)   && HAS_FLAG(obj->orientation, MUI_CENTER)) ||
              (HAS_FLAG(obj->orientation, MUI_LEFT)   && HAS_FLAG(obj->orientation, MUI_RIGHT))  ||
              (HAS_FLAG(obj->orientation, MUI_CENTER) && HAS_FLAG(obj->orientation, MUI_RIGHT))),
              "can only have one of MUI_LEFT, MUI_CENTER or MUI_RIGHT");

  // orinetation & scaling 
  int w, h;
  window_get_size(&w, &h);
  f32 r_wh = ((f32)w / (f32)h);
  f32 r_hw = ((f32)h / (f32)w);
  // P_V(r_wh);
  // P_V(r_hw);
  
  vec2_copy(obj->pos, obj->pos_original);

  obj->pos[0] *= -1.0f;
  // obj->pos[0] *= VIEW_SCL;
  obj->pos[0] *= VIEW_SCL * 0.75f;
  // obj->pos[0] *= r_wh * 2.0f; // 4.0f;
  // obj->pos[1] *= 4.0f;
  // obj->pos[1] *= VIEW_SCL * 0.5f;
  obj->pos[1] *= VIEW_SCL * 0.4f;
  
  // obj.scl[0] *= (scale_by_ratio ? r_wh : 1.0f);
  if (scale_by_ratio)
  { 
    // obj->scl[0] *= r_wh; 
  }
  else  // @TODO: this doesnt seem right
  { 
    // idk
    // obj->scl[0] = obj->scl[1]; 
    
    // obj->scl[1] *= r_hw; 
    // obj->scl[0] *= r_wh; 
    
    obj->scl[0] *= r_hw * 2.0f; 
    // obj->scl[1] *= r_hw * 1.0f; 
    // obj->scl[0] *= r_hw; 
    // obj->scl[1] *= r_wh * 2.0f; 
  }
  
  if (obj->type == MUI_OBJ_IMG)
  {
    r_wh = ((f32)obj->tex->width / (f32)obj->tex->height);
    obj->scl[0] *= r_wh;
  }
  
  if (HAS_FLAG(obj->orientation, MUI_UP))
  { obj->pos[1] += obj->scl[1]; }
  else if (HAS_FLAG(obj->orientation, MUI_DOWN))
  { obj->pos[1] -= obj->scl[1]; }
  if (HAS_FLAG(obj->orientation, MUI_LEFT))
  { obj->pos[0] += obj->scl[0]; }
  else if (HAS_FLAG(obj->orientation, MUI_RIGHT))
  { obj->pos[0] -= obj->scl[0]; }

  // flip, otherwise upside down 
  vec2_negate(obj->scl, obj->scl); 
}
int mui_add_obj(mui_obj_t* obj, bool scale_by_ratio)
{
  if (!obj->active) { return -1; }
  mui_setup_obj(obj, scale_by_ratio);
  arrput(obj_arr, *obj);
  obj_arr_len++;
  return (int)obj_arr_len -1;
}

// int mui_shape(vec2 pos, vec2 scl, rgbf color, mui_obj_type type)
// { 
//   mui_obj_t obj;
//   obj.type = type;
//   vec2_copy(pos, obj.pos);
//   vec2_copy(scl, obj.scl);
//   vec3_copy(color, obj.color);
//  
//   // orinetation & scaling 
//   // @TODO: 
//   bool scale_by_ratio = false; 
//   int w, h;
//   window_get_size(&w, &h);
//   f32 r_wh = ((f32)w / h);
//   
//   obj.pos[0] *= -1.0f;
//   obj.pos[0] *= r_wh * 4.0f;
//   obj.pos[1] *= 4.0f;
//   
//   obj.scl[0] *= (scale_by_ratio ? r_wh : 1.0f);
//   
//   arrput(obj_arr, obj);
//   obj_arr_len++;
//   return obj_arr_len -1;
// }

void mui_group(mui_group_t* g)
{
  ERR_CHECK(g->objs_len > 0, "mui_group_t has no objects in it.\n");
  ERR_CHECK(HAS_FLAG(g->orientation, MUI_DYNAMIC) || HAS_FLAG(g->orientation, MUI_STATIC),
            "mui_group_t must have either MUI_DYNAMIC or MUI_STATIC");
  ERR_CHECK(!((HAS_FLAG(g->orientation, MUI_LEFT)   && HAS_FLAG(g->orientation, MUI_CENTER)) ||
              (HAS_FLAG(g->orientation, MUI_LEFT)   && HAS_FLAG(g->orientation, MUI_RIGHT))  ||
              (HAS_FLAG(g->orientation, MUI_CENTER) && HAS_FLAG(g->orientation, MUI_RIGHT))),
              "can only have one of MUI_LEFT, MUI_CENTER or MUI_RIGHT");

  vec2 size     = VEC2_INIT(0);
  vec2 pos      = VEC2_INIT(0);
  vec2 pos_step = VEC2_INIT(0);
  int  wraps = (g->objs_len / g->max_wrap) < 0 ? 1 : (g->objs_len / g->max_wrap);
  wraps += g->objs_len % g->max_wrap; // f.e. for 3 objs 2 max_wrap, would be 1 wrap
  int objs_len_wrap = g->objs_len + (g->objs_len % g->max_wrap);
  
  int w, h;
  window_get_size(&w, &h);
  // P_INT(objs_len_wrap);
  // P_INT(g->max_wrap);
  // P_INT(wraps);

  if (HAS_FLAG(g->orientation, MUI_ROW))
  {
    f32 length = ( (g->scl[0] / (f32)objs_len_wrap) * (f32)wraps);
    size[0] =  length - g->margin; 
    size[1] = (g->scl[1]  / (f32)wraps) - g->margin;
    pos_step[0] = length * 0.25f; 
    pos_step[1] = 0.0f;
    vec2_copy(g->pos, pos);
    
    pos[0] -= pos_step[0] * ((f32)(objs_len_wrap -1) * 0.5f);
    pos[1] += (f32)(wraps -1) * ( size[1] / (f32)wraps ) * 0.5f; 
  }
  else if (HAS_FLAG(g->orientation, MUI_COLUMN))
  {
    size[0] =  g->scl[0] - g->margin;
    size[1] = (g->scl[1] / (f32)g->objs_len) - g->margin;
    pos_step[0] = 0.0f;
    pos_step[1] = (g->scl[1] / (f32)g->objs_len) * 0.5f;
    vec2_copy(g->pos, pos);
    pos[1] -= pos_step[1] * ((f32)(g->objs_len -1) * 0.5f) ;   
  }

  // @UNSURE: if needed
  // // calc half width and move to the left
  // if (HAS_FLAG(g->orientation, MUI_STATIC) && HAS_FLAG(g->orientation, MUI_CENTER))
  // {
  //   // f32 width_avg = 0.0f;
  //   f32 width_total = 0.0f;
  //   for (u32 i = 0; i < g->objs_len; ++i)
  //   { width_total += g->objs[i].scl[0]; }
  //   // width_avg /= g->objs_len;
  //   // pos[0] -= (width_total * 0.5f) * 0.5f * 0.5f;
  // }

  for (int i = 0; i < g->objs_len; ++i)
  {
    // -- wrap --
    if (i != 0 && i % g->max_wrap == 0)
    {
      // P("wrap");
      pos[1] -= size[1] * 0.5f;
      pos[0] -= pos_step[0] * (f32)g->max_wrap;
    }
    
    // -- draw --
    mui_obj_t* o = &g->objs[i];
    switch (o->type)
    {
      case MUI_OBJ_SHAPE_RECT:
      case MUI_OBJ_SHAPE_RECT_ROUND:
      case MUI_OBJ_SHAPE_CIRCLE:
        // vec2_add(pos,  o->pos, o->pos);
        // vec2_mul(size, o->scl, o->scl);
        // mui_shape(pos, size, o->color, o->type);
        // break;
      case MUI_OBJ_IMG:
        vec2_add(pos,  o->pos, o->pos);
        vec2_mul(size, o->scl, o->scl);
        mui_add_obj(o, g->scale_by_ratio);
        break;

      default: 
        ERR("NOT IMPLEMENTED YET"); 
        break;
    }
   
    // account for window width  
    if (HAS_FLAG(g->orientation, MUI_STATIC))
    { 
      f32 ratio_hw = (f32)h / (f32)w;
      pos_step[0] = -1 * o->scl[0] * ratio_hw * 0.5f; 
    }
    vec2_add(pos_step, pos, pos);
  } 
}

// void mui_draw_shape(vec2 cam_pos, f32 cam_zoom, vec2 pos, vec2 size, rgbf color, mui_obj_type type)
void mui_draw_shape(mat4 _view, mat4 _proj, vec2 pos, vec2 size, rgbf color, mui_obj_type type)
{
  TRACE();

  // ---- mvp ----

  mat4 model;
  mat4_make_model_2d(pos, size, 0.0f, model);

  // mat4 view;
  // mat4_lookat_2d(cam_pos, cam_zoom, view);

  // int w, h;
  // window_get_size(&w, &h);
  // mat4 proj;
  // camera_get_proj_mat(w, h, proj);

  shader_t* shader = NULL;
  switch (type)
  {
    case MUI_OBJ_SHAPE_RECT:
    case MUI_OBJ_SHAPE_CIRCLE:
      shader = &circle_shader;
      break;
    case MUI_OBJ_SHAPE_RECT_ROUND:
      shader = &rounded_shader;
      break;
    
    default: 
      shader = &circle_shader;
      break;
  }

  // ---- shader & draw call -----	

  shader_use(shader);
  shader_set_vec3(shader, "color", color);

  shader_set_mat4(shader, "model", model);
  shader_set_mat4(shader, "view", _view);
  shader_set_mat4(shader, "proj", _proj);

  if (type == MUI_OBJ_SHAPE_RECT)
  {
    shader_set_int(shader, "draw_rect", 1);
  }
  else if (type == MUI_OBJ_SHAPE_CIRCLE)
  {
    shader_set_int(shader, "draw_rect", 0);
  }
  else if (type == MUI_OBJ_SHAPE_RECT_ROUND)
  {
    f32 ratio_x = (size[0] / size[1]); // * ((f32)w / (f32)h);
    f32 ratio_y = (size[1] / size[0]); // * ((f32)h / (f32)w);
    shader_set_float(shader, "ratio_x", ratio_x);
    shader_set_float(shader, "ratio_y", ratio_y);
  }

  mesh_t* m = assetm_get_mesh_by_idx(core_data->quad_mesh);
  glBindVertexArray(m->vao);
  if (m->indexed)
  { _glDrawElements(GL_TRIANGLES, m->indices_count, GL_UNSIGNED_INT, 0); }
  else
  { _glDrawArrays(GL_TRIANGLES, 0, m->verts_count); }

}

void mui_draw_icon(mat4 _view, mat4 _proj, vec2 pos, vec2 size, rgbf color, int asset_idx)
{
  TRACE();

  // ---- mvp ----

  mat4 model;
  mat4_make_model_2d(pos, size, 0.0f, model);

  // mat4 view;
  // mat4_lookat_2d(cam_pos, cam_zoom, view);

  // int w, h;
  // window_get_size(&w, &h);
  // mat4 proj;
  // camera_get_proj_mat(w, h, proj);

  shader_t* shader = &core_data->basic_shader;

  // ---- shader & draw call -----	

  shader_use(shader);
  // shader_set_vec3(shader, "color", color);
  shader_set_vec3(shader, "tint", color);
  _glActiveTexture(GL_TEXTURE0);
  _glBindTexture(GL_TEXTURE_2D, assetm_get_texture("#internal/blank.png", true)->handle); 
  shader_set_int(&core_data->basic_shader, "tex", 0);

  shader_set_mat4(shader, "model", model);
  shader_set_mat4(shader, "view", _view);
  shader_set_mat4(shader, "proj", _proj);

  // mesh_t* m = assetm_get_mesh_by_idx(core_data->quad_mesh);
  mesh_t* m = assetm_get_mesh_by_idx(asset_idx);
  glBindVertexArray(m->vao);
  if (m->indexed)
  { _glDrawElements(GL_TRIANGLES, m->indices_count, GL_UNSIGNED_INT, 0); }
  else
  { _glDrawArrays(GL_TRIANGLES, 0, m->verts_count); }

}
