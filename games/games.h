#ifndef __GAMES__H
#define __GAMES__H

#ifdef __cplusplus
extern "C" {
#endif

// @DOC: definitions for this function are found in 
//       ./games/[game_name]/[game_name].c
//       f.e. ./games/sandbox/sandbox.c
//       each game has its own defenition
void __init__();
void __update__();
void __cleanup__();

// ---- ecs_scripts.h ----

// @DOC: needs to be includede here bc. needed by ecs/ecs.h in ecs_entity_remove_id()
bool SCRIPT_REMOVE_FUNC_GENERIC_NAME(u32 uid);
// @DOC: needs to be includede here bc. needed by gui_properties.c 
char* SCRIPT_GET_TYPE_STR_FUNC_NAME(u32 uid);
// @DOC: needs to be includede here bc. needed by ecs_clear_state() 
void  SCRIPTS_CLEAR_FUNC_NAME(); 


#ifdef __cplusplus
}   // extern c
#endif

#endif  // __GAMES__H
