#ifndef APP_H
#define APP_H

#include "global/global.h"
#include "core/io/input.h"
#include "core/types/types.h"

// @DOC: houses all publicly accessible data for app (editor)
//       instance of app_data_t is in app.c and accesible via app_data_get()
typedef struct app_data_t
{

    
}app_data_t;

#define APP_DATA_INIT()                                \
{                                                      \
}

// -- keymappings --
// some are in program.h

#define CAM_SPEED             5
#define CAM_SPEED_SHIFT_MULT  5
#define KEY_MOUSE_MOVE_START  MOUSE_RIGHT
#define KEY_MOVE_FORWARD      KEY_W 
#define KEY_MOVE_BACKWARD     KEY_S
#define KEY_MOVE_LEFT         KEY_A
#define KEY_MOVE_RIGHT        KEY_D 
#define KEY_MOVE_UP           KEY_E 
#define KEY_MOVE_DOWN         KEY_Q 


#define SCENE_FILE_NAME   "test.scene"
#define TERRAIN_FILE_NAME "test.terrain"

// @DOC: initalize, call before any other calls to app
void app_init();
// @DOC: upate logic called once a frame
void app_update();

// @DOC: returns a pointer to app_data_t var in app.c
app_data_t* app_data_get();

void app_entity_removed_callback(int id);


#endif
