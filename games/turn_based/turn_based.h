#ifndef GAME_TEST_TEST_H
#define GAME_TEST_TEST_H

#include "global/global.h"
#include "core/types/types.h"
#include "core/state/state.h"

#ifdef __cplusplus
extern "C" {
#endif


#define GRID_IDS_AXIS_MAX 128 // max tiles per x or z axis
#define GRID_IDS_MAX      (GRID_IDS_AXIS_MAX * GRID_IDS_AXIS_MAX) // x * z axis max
// @TODO: move game_data to test.h
// @DOC: houses all publicly accessible data for app (editor)
//       instance of app_data_t is in app.c and accesible via app_data_get()
typedef struct 
{
  bool run_scripts;
  bool pause_menu_act;
  int grid_ids[GRID_IDS_MAX];
  int grid_ids_pos;
}game_data_t;

#define GAME_DATA_INIT()              \
{                                     \
  .run_scripts    = true,             \
  .pause_menu_act = false,            \
  .grid_ids       = { -1 },           \
  .grid_ids_pos   = 0,                \
}
// @DOC: extern pointer to game_data_t in games/<game_name>/<game_name>.c
//       available everywhere that includes "games/<game_name>/<game_name>.h"
extern game_data_t* game_data;

// --- func decls ---
// ...

#ifdef __cplusplus
} // extern c
#endif

#endif  // GAME_TEST_TEST_H
