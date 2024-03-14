#ifndef GAME__GAMES__H
#define GAME__GAMES__H

#include "global/global.h"
#include "core/state/state_scripts.h"

#ifdef __cplusplus
extern "C" {
#endif

// @DOC: definitions for this function are found in 
//       ./games/[game_name]/[game_name].c
//       f.e. ./games/sandbox/sandbox.c
//       or   ./games/test/test.c
//       each game has its own defenition
void __pre_init__();  // load scene
void __init__();      // do initialization
void __update__();    // logic
void __cleanup__();   // cleanup allocations, etc. 

#ifdef EDITOR
void __editor_update__();    // called everyframe while in editor
#endif // EDITOR

  

// ---- state_scripts.h ----

void __scripts_init__();
void __scripts_update__();
void __scripts_cleanup__();

// @DOC: needs to be includede here bc. needed by ecs/ecs.h in state_entity_remove_id()
bool SCRIPT_REMOVE_FUNC_GENERIC_NAME(u32 uid);
// @DOC: needs to be includede here bc. needed by gui_properties.c 
char* SCRIPT_GET_TYPE_STR_FUNC_NAME(u32 uid);
// @DOC: needs to be includede here bc. needed by state_clear_state() 
void  SCRIPTS_CLEAR_FUNC_NAME(); 
// @DOC: get script without type, slower tham SCRIPT_GET()
script_t* SCRIPT_GET_FUNC_GENERIC_NAME(u32 uid, u32* size);


#ifdef __cplusplus
}   // extern c
#endif

#endif  // GAME__GAMES__H
