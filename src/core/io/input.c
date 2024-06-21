#include "core/io/input.h"
#include "core/core_data.h"
#include "core/window.h"

#include "GLFW/glfw3.h"
// #include "stb/stb_ds.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_KEYSTATE_BASED_INPUT
#include "nuklear/nuklear.h"
#include "nuklear/nuklear_glfw_gl3.h"

#include <stdarg.h>

// ---- vars ----
 
// [0]: qwerty, [1]: qwertz
#define LOCALE_QWERTY 0
#define LOCALE_QWERTZ 1
#define LOCALE_MAX 2
key_type key_locale[LOCALE_MAX][KEY_MAX];
int cur_locale = LOCALE_QWERTZ;

// key state last frame 
bool
Space_st, Apostrophe_st, Comma_st, Minus_st, Period_st, Slash_st,
Alpha0_st, Alpha1_st, Alpha2_st, Alpha3_st, Alpha4_st, Alpha5_st, Alpha6_st, Alpha7_st, Alpha8_st, Alpha9_st,
SemiColon_st, Equal_st,
A_st, B_st, C_st, D_st, E_st, F_st, G_st, H_st, I_st, J_st, K_st, L_st, M_st,
N_st, O_st, P_st, Q_st, R_st, S_st, T_st, U_st, V_st, W_st, X_st, Y_st, Z_st,
LeftBracket_st, Backslash_st, RightBracket_st, GraveAccent_st, World1_st,
World2_st, Escape_st, Enter_st, Tab_st, Backspace_st, Insert_st, Delete_st,
RightArrow_st, LeftArrow_st, DownArrow_st, UpArrow_st,
PageUp_st, PageDown_st, Home_st, End_st, CapsLock_st, ScrollLock_st, NumLock_st, PrintScreen_st, Pause_st,
F1_st, F2_st, F3_st, F4_st, F5_st, F6_st, F7_st, F8_st, F9_st, F10_st, F11_st, F12_st, F13_st, F14_st, F15_st,
F16_st, F17_st, F18_st, F19_st, F20_st, F21_st, F22_st, F23_st, F24_st, F25_st,
Numpad0_st, Numpad1_st, Numpad2_st, Numpad3_st, Numpad4_st, Numpad5_st, Numpad6_st, Numpad7_st, Numpad8_st, Numpad9_st,
NumpadDecimal_st, NumpadDivide_st, NumpadMultiply_st, NumpadSubtract_st, NumpadAdd_st, NumpadEnter_st, NumpadEqual_st,
LeftShift_st, LeftControl_st, LeftAlt_st, LeftSuper_st, RightShift_st, RightControl_st, RightAlt_st, RightSuper_st, Menu_st,
LeftWinMacSymbol_st, RightWinMacSymbol_st;

// mouse_button state last frame. 
bool mouse_button1, mouse_button2, mouse_button3, mouse_button4, mouse_button5, mouse_button6, mouse_button7, mouse_button8;


// empty_callback* mouse_pos_callbacks = NULL;
// int mouse_pos_callbacks_len = 0;

// --------------

void input_init()
{
  TRACE();

  glfwSetKeyCallback(core_data->window, (GLFWkeyfun)input_key_callback);
  glfwSetCursorPosCallback(core_data->window, (GLFWcursorposfun)input_mouse_pos_callback);
  // @BUGG: not being called
  glfwSetMouseButtonCallback(core_data->window, (GLFWmousebuttonfun)input_mouse_callback);
  glfwSetScrollCallback(core_data->window, (GLFWscrollfun)input_scroll_callback);

  core_data->mouse_x = 0.0;
  core_data->mouse_y = 0.0;
  core_data->mouse_delta_x = 0.0;
  core_data->mouse_delta_y = 0.0;
  core_data->scroll_x = 0.0;
  core_data->scroll_y = 0.0;
  core_data->scroll_delta_x = 0.0;
  core_data->scroll_delta_y = 0.0;
  
  // @TODO: get keyboard locale

  // fill locale so key_locale[0][KEY_CODE] = KEY_CODE
  for (u32 i = 0; i < KEY_MAX; ++i)
  { 
    for (u32 j = 0; j < LOCALE_MAX; ++j)
    { key_locale[j][i] = (key_type)i; }
  }

  // switch  y, z for qwertz
  key_locale[LOCALE_QWERTZ][KEY_Z] = KEY_Y;
  key_locale[LOCALE_QWERTZ][KEY_Y] = KEY_Z;
}
void input_update()
{
  TRACE();

  // wipe state vars
  Space_st = false;
  Apostrophe_st = false; Comma_st = false; Minus_st = false; Period_st = false; Slash_st = false;
  Alpha0_st = false; Alpha1_st = false; Alpha2_st = false; Alpha3_st = false; Alpha4_st = false; Alpha5_st = false; Alpha6_st = false; Alpha7_st = false; Alpha8_st = false; Alpha9_st = false;
  SemiColon_st = false; Equal_st = false;
  A_st = false; B_st = false; C_st = false; D_st = false; E_st = false; F_st = false; G_st = false; H_st = false; I_st = false; J_st = false; K_st = false; L_st = false; M_st = false;
  N_st = false; O_st = false; P_st = false; Q_st = false; R_st = false; S_st = false; T_st = false; U_st = false; V_st = false; W_st = false; X_st = false; Y_st = false; Z_st = false;
  LeftBracket_st = false; Backslash_st = false; RightBracket_st = false; GraveAccent_st = false; World1_st = false;
  World2_st = false; Escape_st = false; Enter_st = false; Tab_st = false; Backspace_st = false; Insert_st = false; Delete_st = false;
  RightArrow_st = false; LeftArrow_st = false; DownArrow_st = false; UpArrow_st = false;
  PageUp_st = false; PageDown_st = false; Home_st = false; End_st = false; CapsLock_st = false; ScrollLock_st = false; NumLock_st = false; PrintScreen_st = false; Pause_st = false;
  F1_st = false; F2_st = false; F3_st = false; F4_st = false; F5_st = false; F6_st = false; F7_st = false; F8_st = false; F9_st = false; F10_st = false; F11_st = false; F12_st = false; F13_st = false; F14_st = false; F15_st = false;
  F16_st = false; F17_st = false; F18_st = false; F19_st = false; F20_st = false; F21_st = false; F22_st = false; F23_st = false; F24_st = false; F25_st = false;
  Numpad0_st = false; Numpad1_st = false; Numpad2_st = false; Numpad3_st = false; Numpad4_st = false; Numpad5_st = false; Numpad6_st = false; Numpad7_st = false; Numpad8_st = false; Numpad9_st = false;
  NumpadDecimal_st = false; NumpadDivide_st = false; NumpadMultiply_st = false; NumpadSubtract_st = false; NumpadAdd_st = false; NumpadEnter_st = false; NumpadEqual_st = false;
  LeftShift_st = false; LeftControl_st = false; LeftAlt_st = false; LeftSuper_st = false; RightShift_st = false; RightControl_st = false; RightAlt_st = false; RightSuper_st = false; Menu_st = false;
  LeftWinMacSymbol_st = false; RightWinMacSymbol_st = false;

  mouse_button1 = false; mouse_button2 = false; mouse_button3 = false; mouse_button4 = false; 
  mouse_button5 = false; mouse_button6 = false; mouse_button7 = false; mouse_button8 = false;
  
  core_data->mouse_delta_x = 0.0;
  core_data->mouse_delta_y = 0.0;
}


input_state input_get_key_state(key_type _key)
{
  TRACE();

  // convert qwerty to cur locale
  key_type key = key_locale[cur_locale][_key];

  // key_type & keystate map directly to glfws key definitions
	return glfwGetKey(core_data->window, key);
}
bool input_get_key_down(key_type _key)
{
  TRACE();

	return input_get_key_state(_key) == STATE_PRESS;
}
bool is_key_released(key_type _key)
{
  TRACE();

	return input_get_key_state(_key) == STATE_RELEASED;

}
bool input_get_key_pressed(key_type _key)
{
  TRACE();

  return input_get_key_down(_key) && input_get_last_key_state(_key);
}

// @NOTE: neat idea, but kinda unnecessary
// bool input_get_keys_down_or(u32 count, ...)
// {
//   va_list args;
//   va_start(args, count);
// 
//   bool result = false;
//   for (u32 i = 0; i < count; ++i)
//   {
//     key _key = va_arg(args, key);
//     result = result || input_get_key_down(_key);
//   }
// 
//   va_end(args);
// 
//   return result;
// }

bool input_get_last_key_state(key_type key)
{
  TRACE();

  // convert cur locale to qwerty
  key_type _key = key_locale[cur_locale][key];
 
  switch (_key)
  {
    case KEY_UNKNOWN: return false;
    case KEY_SPACE: return Space_st;
    case KEY_APOSTROPHE: return Apostrophe_st;
    case KEY_COMMA: return Comma_st;
    case KEY_MINUS: return Minus_st;
    case KEY_PERIOD: return Period_st;
    case KEY_SLASH: return Slash_st;
    case KEY_ALPHA0: return Alpha0_st;
    case KEY_ALPHA1: return Alpha1_st;
    case KEY_ALPHA2: return Alpha2_st;
    case KEY_ALPHA3: return Alpha3_st;
    case KEY_ALPHA4: return Alpha4_st;
    case KEY_ALPHA5: return Alpha5_st;
    case KEY_ALPHA6: return Alpha6_st;
    case KEY_ALPHA7: return Alpha7_st;
    case KEY_ALPHA8: return Alpha8_st;
    case KEY_ALPHA9: return Alpha9_st;
    case KEY_SEMICOLON: return SemiColon_st;
    case KEY_EQUAL: return Equal_st;
    case KEY_A: return A_st;
    case KEY_B: return B_st;
    case KEY_C: return C_st;
    case KEY_D: return D_st;
    case KEY_E: return E_st;
    case KEY_F: return F_st;
    case KEY_G: return G_st;
    case KEY_H: return H_st;
    case KEY_I: return I_st;
    case KEY_J: return J_st;
    case KEY_K: return K_st;
    case KEY_L: return L_st;
    case KEY_M: return M_st;
    case KEY_N: return N_st;
    case KEY_O: return O_st;
    case KEY_P: return P_st;
    case KEY_Q: return Q_st;
    case KEY_R: return R_st;
    case KEY_S: return S_st;
    case KEY_T: return T_st;
    case KEY_U: return U_st;
    case KEY_V: return V_st;
    case KEY_W: return W_st;
    case KEY_X: return X_st;
    case KEY_Y: return Y_st;
    case KEY_Z: return Z_st;
    case KEY_LEFT_BRACKET: return LeftBracket_st;
    case KEY_BACKSLASH: return Backslash_st;
    case KEY_RIGHT_BRACKET: return RightBracket_st;
    case KEY_GRAVE_ACCENT: return GraveAccent_st;
    case KEY_WORLD1: return World1_st; 
    case KEY_WORLD2: return World1_st; 
    case KEY_ESCAPE: return Escape_st; 
    case KEY_ENTER: return Enter_st; 
    case KEY_TAB: return Tab_st;
    case KEY_BACKSPACE: return Backspace_st;
    case KEY_INSERT: return Insert_st; 
    case KEY_DELETE: return Delete_st; 
    case KEY_RIGHT_ARROW: return RightArrow_st; 
    case KEY_LEFT_ARROW: return LeftArrow_st; 
    case KEY_DOWN_ARROW: return DownArrow_st; 
    case KEY_UP_ARROW: return UpArrow_st;
    case KEY_PAGE_UP: return PageUp_st; 
    case KEY_PAGE_DOWN: return PageDown_st;
    case KEY_HOME: return Home_st;
    case KEY_END: return End_st; 
    case KEY_CAPS_LOCK: return CapsLock_st; 
    case KEY_SCROLL_LOCK: return ScrollLock_st;
    case KEY_NUM_LOCK: return NumLock_st; 
    case KEY_PRINT: return PrintScreen_st;
    case KEY_PAUSE: return Pause_st;
    case KEY_F1:  return F1_st; 
    case KEY_F2:  return F2_st; 
    case KEY_F3:  return F3_st; 
    case KEY_F4:  return F4_st; 
    case KEY_F5:  return F5_st; 
    case KEY_F6:  return F6_st; 
    case KEY_F7:  return F7_st; 
    case KEY_F8:  return F8_st; 
    case KEY_F9:  return F9_st; 
    case KEY_F10: return F10_st;
    case KEY_F11: return F11_st;
    case KEY_F12: return F12_st;
    case KEY_F13: return F13_st;
    case KEY_F14: return F14_st;
    case KEY_F15: return F15_st;
    case KEY_F16: return F16_st;
    case KEY_F17: return F17_st;
    case KEY_F18: return F18_st;
    case KEY_F19: return F19_st;
    case KEY_F20: return F20_st;
    case KEY_F21: return F21_st;
    case KEY_F22: return F22_st;
    case KEY_F23: return F23_st;
    case KEY_F24: return F24_st;
    case KEY_F25: return F25_st;
    case KEY_NUMPAD0: return Numpad0_st; 
    case KEY_NUMPAD1: return Numpad1_st; 
    case KEY_NUMPAD2: return Numpad2_st; 
    case KEY_NUMPAD3: return Numpad3_st; 
    case KEY_NUMPAD4: return Numpad4_st; 
    case KEY_NUMPAD5: return Numpad5_st; 
    case KEY_NUMPAD6: return Numpad6_st; 
    case KEY_NUMPAD7: return Numpad7_st; 
    case KEY_NUMPAD8: return Numpad8_st; 
    case KEY_NUMPAD9: return Numpad9_st; 
    case KEY_NUMPAD_DECIMAL:   return NumpadDecimal_st;
    case KEY_NUMPAD_DIVIDE:    return NumpadDivide_st;
    case KEY_NUMPAD_MULTIPLY: return NumpadMultiply_st; 
    case KEY_NUMPAD_SUBTRACT: return NumpadSubtract_st; 
    case KEY_NUMPAD_ADD: return NumpadAdd_st;
    case KEY_NUMPAD_ENTER: return NumpadEnter_st; 
    case KEY_NUMPAD_EQUAL: return NumpadEqual_st; 
    case KEY_LEFT_SHIFT: return LeftShift_st;
    case KEY_LEFT_CONTROL: return LeftControl_st;
    case KEY_LEFT_ALT: return LeftAlt_st;
    //same as returning LeftWinMacSymbol_st
    case KEY_LEFT_SUPER: return LeftSuper_st;     
    //same as returning RightWinMacSymbol_st
    case KEY_RIGHT_SUPER: return RightSuper_st;  
    case KEY_RIGHT_SHIFT: return RightShift_st; 
    case KEY_RIGHT_CONTROL: return RightControl_st;
    case KEY_RIGHT_ALT: return RightAlt_st;
    case KEY_MENU: return Menu_st;

    default: return false;
  }

  return false;
}

// window is type GLFWwindow*
void input_key_callback(void* window, key_type _key, int scancode, input_state state, int _mods)
{
  (void)window; (void)scancode; (void)_mods;
  TRACE();

    if (state == STATE_PRESS)
    {
      switch (_key)
      {
        // not actually keys
        case KEY_UNKNOWN: return;
        case KEY_MAX:     return;

        //mapping glfw keys to our keycodes
        case KEY_SPACE:      Space_st = true; break;
        case KEY_APOSTROPHE: Apostrophe_st = true; break;
        case KEY_COMMA:      Comma_st = true; break;
        case KEY_MINUS:      Minus_st = true;  break;
        case KEY_PERIOD:     Period_st = true;  break;
        case KEY_SLASH:      Slash_st = true;  break;
        case KEY_ALPHA0:     Alpha0_st = true;  break;
        case KEY_ALPHA1:     Alpha1_st = true;  break;
        case KEY_ALPHA2:     Alpha2_st = true;  break;
        case KEY_ALPHA3:     Alpha3_st = true;  break;
        case KEY_ALPHA4:     Alpha4_st = true;  break;
        case KEY_ALPHA5:     Alpha5_st = true;  break;
        case KEY_ALPHA6:     Alpha6_st = true;  break;
        case KEY_ALPHA7:     Alpha7_st = true;  break;
        case KEY_ALPHA8:     Alpha8_st = true;  break;
        case KEY_ALPHA9:     Alpha9_st = true;  break;
        case KEY_SEMICOLON:  SemiColon_st = true;  break;
        case KEY_EQUAL:      Equal_st = true;  break;
        case KEY_A: A_st = true; break;
        case KEY_B: B_st = true; break;
        case KEY_C: C_st = true; break;
        case KEY_D: D_st = true; break;
        case KEY_E: E_st = true; break;
        case KEY_F: F_st = true; break; 
        case KEY_G: G_st = true; break; 
        case KEY_H: H_st = true; break; 
        case KEY_I: I_st = true; break; 
        case KEY_J: J_st = true; break; 
        case KEY_K: K_st = true; break; 
        case KEY_L: L_st = true; break; 
        case KEY_M: M_st = true; break; 
        case KEY_N: N_st = true; break; 
        case KEY_O: O_st = true; break; 
        case KEY_P: P_st = true; break; 
        case KEY_Q: Q_st = true; break; 
        case KEY_R: R_st = true; break; 
        case KEY_S: S_st = true; break; 
        case KEY_T: T_st = true; break; 
        case KEY_U: U_st = true; break; 
        case KEY_V: V_st = true; break; 
        case KEY_W: W_st = true; break; 
        case KEY_X: X_st = true; break; 
        case KEY_Y: Y_st = true; break; 
        case KEY_Z: Z_st = true; break; 
        case KEY_LEFT_BRACKET: LeftBracket_st = true;  break;
        case KEY_BACKSLASH:    Backslash_st = true;  break;
        case KEY_RIGHT_BRACKET: RightBracket_st = true;  break;
        case KEY_GRAVE_ACCENT:  GraveAccent_st = true; break;
        case KEY_WORLD1:        World1_st = true;  break;
        case KEY_WORLD2:        World1_st = true;  break;
        case KEY_ESCAPE:        Escape_st = true;  break;
        case KEY_ENTER:         Enter_st = true;  break;
        case KEY_TAB:           Tab_st = true; break;
        case KEY_BACKSPACE:     Backspace_st = true;  break;
        case KEY_INSERT:        Insert_st = true;  break;
        case KEY_DELETE:        Delete_st = true;  break;
        case KEY_RIGHT_ARROW:   RightArrow_st = true;  break;
        case KEY_LEFT_ARROW:    LeftArrow_st = true;  break;
        case KEY_DOWN_ARROW:    DownArrow_st = true;  break;
        case KEY_UP_ARROW:      UpArrow_st = true;  break;
        case KEY_PAGE_UP:       PageUp_st = true; break;
        case KEY_PAGE_DOWN:     PageDown_st = true;  break;
        case KEY_HOME:          Home_st = true;  break;
        case KEY_END:           End_st = true;  break;
        case KEY_CAPS_LOCK:     CapsLock_st = true;  break;
        case KEY_SCROLL_LOCK:   ScrollLock_st = true;  break;
        case KEY_NUM_LOCK:      NumLock_st = true;  break;
        case KEY_PRINT:         PrintScreen_st = true;  break;
        case KEY_PAUSE:         Pause_st = true;  break;
        case KEY_F1:  F1_st = true;  break; 
        case KEY_F2:  F2_st = true;  break; 
        case KEY_F3:  F3_st = true;  break; 
        case KEY_F4:  F4_st = true;  break; 
        case KEY_F5:  F5_st = true;  break; 
        case KEY_F6:  F6_st = true;  break; 
        case KEY_F7:  F7_st = true;  break; 
        case KEY_F8:  F8_st = true;  break; 
        case KEY_F9:  F9_st = true;  break; 
        case KEY_F10: F10_st = true; break; 
        case KEY_F11: F11_st = true; break; 
        case KEY_F12: F12_st = true; break; 
        case KEY_F13: F13_st = true; break; 
        case KEY_F14: F14_st = true; break; 
        case KEY_F15: F15_st = true; break; 
        case KEY_F16: F16_st = true; break; 
        case KEY_F17: F17_st = true; break; 
        case KEY_F18: F18_st = true; break; 
        case KEY_F19: F19_st = true; break; 
        case KEY_F20: F20_st = true; break; 
        case KEY_F21: F21_st = true; break; 
        case KEY_F22: F22_st = true; break; 
        case KEY_F23: F23_st = true; break; 
        case KEY_F24: F24_st = true; break; 
        case KEY_F25: F25_st = true; break; 
        case KEY_NUMPAD0: Numpad0_st = true; break; 
        case KEY_NUMPAD1: Numpad1_st = true; break; 
        case KEY_NUMPAD2: Numpad2_st = true; break; 
        case KEY_NUMPAD3: Numpad3_st = true; break; 
        case KEY_NUMPAD4: Numpad4_st = true; break; 
        case KEY_NUMPAD5: Numpad5_st = true; break; 
        case KEY_NUMPAD6: Numpad6_st = true; break; 
        case KEY_NUMPAD7: Numpad7_st = true; break; 
        case KEY_NUMPAD8: Numpad8_st = true; break; 
        case KEY_NUMPAD9: Numpad9_st = true; break; 
        case KEY_NUMPAD_DECIMAL:  NumpadDecimal_st = true;  break;
        case KEY_NUMPAD_DIVIDE:   NumpadDivide_st = true;  break;
        case KEY_NUMPAD_MULTIPLY: NumpadMultiply_st = true;  break;
        case KEY_NUMPAD_SUBTRACT: NumpadSubtract_st = true;  break;
        case KEY_NUMPAD_ADD:      NumpadAdd_st = true;  break;
        case KEY_NUMPAD_ENTER:    NumpadEnter_st = true;  break;
        case KEY_NUMPAD_EQUAL:    NumpadEqual_st = true;  break;
        case KEY_LEFT_SHIFT:      LeftShift_st = true;  break;
        case KEY_LEFT_CONTROL:    LeftControl_st = true;  break;
        case KEY_LEFT_ALT:        LeftAlt_st = true;  break;
        case KEY_LEFT_SUPER:      LeftSuper_st = true; LeftWinMacSymbol_st = true;  break;
        case KEY_RIGHT_SUPER:     RightSuper_st = true; RightWinMacSymbol_st = true;  break;
        case KEY_RIGHT_SHIFT:     RightShift_st = true;  break;
        case KEY_RIGHT_CONTROL:   RightControl_st = true;  break;
        case KEY_RIGHT_ALT:       RightAlt_st = true;  break;
        case KEY_MENU:            Menu_st = true;  break;
      }
    }
}


input_state input_get_mouse_state(mouse_btn_type btn)
{
  TRACE();
  // "key" & "keystate" map directly to glfws key definitions
  return glfwGetMouseButton(core_data->window, btn);
}

bool input_get_mouse_down(mouse_btn_type btn)
{
  TRACE();
  return input_get_mouse_state(btn) == STATE_PRESS;
}

bool input_get_mouse_released(mouse_btn_type btn)
{
  TRACE();
  return input_get_mouse_state(btn) == STATE_RELEASED;
}

bool input_get_mouse_pressed(mouse_btn_type btn)
{
  TRACE();
  // P_INT(input_get_mouse_down(btn));
  // P_INT(input_get_last_mouse_state(btn));
  // P_BOOL(mouse_button1);
  return input_get_mouse_down(btn) && input_get_last_mouse_state(btn);
}

bool input_get_last_mouse_state(mouse_btn_type btn)
{
  TRACE();

  switch (btn)
  {
    case MOUSE_BUTTON1: return mouse_button1;
    case MOUSE_BUTTON2: return mouse_button2;
    case MOUSE_BUTTON3: return mouse_button3;
    case MOUSE_BUTTON4: return mouse_button4;
    case MOUSE_BUTTON5: return mouse_button5;
    case MOUSE_BUTTON6: return mouse_button6;
    case MOUSE_BUTTON7: return mouse_button7;
    case MOUSE_BUTTON8: return mouse_button8;
    default: return false;
  }
  return false;
}

// @BUGG: not being called
// window is type GLFWwindow*
void input_mouse_callback(void* window, mouse_btn_type button, input_state state, int _mods)
{
  (void)window; (void)_mods;
  TRACE();

#ifdef EDITOR
  nk_glfw3_mouse_button_callback(window, button, state, _mods);
#endif

  // printf("button: %d, state: %d\n", button, state);
  // PF("button: %d, state: %d\n", button, state);
  if (state == STATE_PRESS)
  {
    switch (button)
    {
      // actions GLFW_PRESS & GLFW_RELEASE map directly to bool
      case MOUSE_BUTTON1: mouse_button1 = true; break; 
      case MOUSE_BUTTON2: mouse_button2 = true; break; 
      case MOUSE_BUTTON3: mouse_button3 = true; break; 
      case MOUSE_BUTTON4: mouse_button4 = true; break; 
      case MOUSE_BUTTON5: mouse_button5 = true; break; 
      case MOUSE_BUTTON6: mouse_button6 = true; break; 
      case MOUSE_BUTTON7: mouse_button7 = true; break; 
      case MOUSE_BUTTON8: mouse_button8 = true; break; 
      default: break;
    }
  }
}

f64 input_get_mouse_x()
{
  TRACE();

    return core_data->mouse_x;
}
f64 input_get_mouse_y()
{
  TRACE();

    return core_data->mouse_y;
}
void input_get_mouse_pos(f64* x, f64* y)
{
  TRACE();

    *x = core_data->mouse_x;
    *y = core_data->mouse_y;
}
void input_get_mouse_pos_normalized(f64* x, f64* y)
{
  TRACE();
  
  int w, h;
  window_get_size(&w, &h);
  *x = core_data->mouse_x / (f64)w;
  *y = core_data->mouse_y / (f64)h;
}
f64 input_get_mouse_delta_x()
{
  TRACE();

    return core_data->mouse_delta_x;
}
f64 input_get_mouse_delta_y()
{
  TRACE();

    return core_data->mouse_delta_y;
}
void input_get_mouse_delta(f64* x, f64* y)
{
  TRACE();

    *x = core_data->mouse_delta_x;
    *y = core_data->mouse_delta_y;
}

void input_center_cursor_pos()
{
  TRACE();

    int w, h;
    window_get_size(&w, &h);
    glfwSetCursorPos(core_data->window, (double)w / 2, (double)h / 2);
    core_data->mouse_x = (double)w / 2;
    core_data->mouse_y = (double)h / 2;
    core_data->mouse_delta_x = 0; core_data->mouse_delta_y = 0;
}

void input_set_cursor_visible_dbg(bool visible, const char* _file, const int _line)
{
  (void)_file; (void)_line;
  TRACE();

  // PF("| cursor: %s, [%s][%d]\n", STR_BOOL(visible), file, line);
  
  if (visible)
  {
    glfwSetInputMode(core_data->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }
  else
  {
    glfwSetInputMode(core_data->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }
}

// window is type GLFWwindow*
void input_mouse_pos_callback(void* window, double xpos, double ypos)
{
  (void)window;
  TRACE();

    core_data->mouse_delta_x = xpos - core_data->mouse_x;
    core_data->mouse_delta_y = core_data->mouse_y - ypos; // for some reason y is invers, prob because opengl is weird about coordinates

    core_data->mouse_x = xpos;
    core_data->mouse_y = ypos;
}

void input_scroll_callback(void* window, double xoff, double yoff)
{
  (void)window;
  TRACE();

#ifdef EDITOR
  nk_gflw3_scroll_callback(window, xoff, yoff);
#endif

  core_data->scroll_delta_x = xoff - core_data->scroll_x;
  core_data->scroll_delta_y = core_data->scroll_y - yoff; // for some reason y is invers, prob because opengl is weird about coordinates

  core_data->scroll_x += xoff;
  core_data->scroll_y += yoff;

  // printf("delta: x: %f\n", core_data->scroll_delta_x);
  // printf("delta: y: %f\n", core_data->scroll_delta_y);
  //
  // printf("scroll x: %f\n", core_data->scroll_x);
  // printf("scroll y: %f\n", core_data->scroll_y);
}

