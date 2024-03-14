#ifndef GAME_TEMPLATE_TEMPLATE_H
#define GAME_TEMPLATE_TEMPLATE_H

#include "global/global.h"
#include "core/types/types.h"
#include "core/state/state.h"

#ifdef __cplusplus
extern "C" {
#endif


// @TODO: move game_data to test.h
// @DOC: houses all publicly accessible data for app (editor)
//       instance of app_data_t is in app.c and accesible via app_data_get()
typedef struct 
{
  bool run_scripts;
  bool pause_menu_act;

}game_data_t;

#define GAME_DATA_INIT()                                \
{                                                       \
  .run_scripts    = true,                               \
  .pause_menu_act = false,                              \
}
// @DOC: extern pointer to app_data_t in game/app.c
//       available everywhere that includes "game/app.h"
extern game_data_t* game_data;

// --- func decls ---

void template_play();
void template_pause();
void template_ui_pause_menu();

#ifdef __cplusplus
} // extern c
#endif

#endif  // GAME_TEST_TEST_H
