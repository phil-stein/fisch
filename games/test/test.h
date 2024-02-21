#ifndef GAME_TEST_TEST_H
#define GAME_TEST_TEST_H

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
  int player_id;
  int score;
  int enemy_count;
}game_data_t;

#define GAME_DATA_INIT()                                \
{                                                       \
  .player_id   = -1,                                    \
  .score       =  0,                                    \
  .enemy_count = 0,                                     \
}
// @DOC: extern pointer to app_data_t in game/app.c
//       available everywhere that includes "game/app.h"
extern game_data_t* game_data;

#ifdef __cplusplus
} // extern c
#endif

#endif  // GAME_TEST_TEST_H
