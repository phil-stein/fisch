#ifndef CORE_STATE_SCRIPTS_H
#define CORE_STATE_SCRIPTS_H

#include "global/global.h"
#include "core/types/types.h"
#include "core/event_sys.h"


// --- script macros ---
// example:
//  in script_file.h
//  | typedef struct
//  | {
//  |   int entity_id;  // required
//  |   bool is_dead;   // required
//  |   ...
//  | }test_script_t;
//  | #define ENEMY_BEHAVIOUR_SCRIPT_T_INIT .val = 12, val2 = 2
//  in script_file.c
//  | SCRIPT_REGISTER(fps_controller_script_t, 0); // no default init values
//  | SCRIPT_REGISTER(fps_controller_script_t); // no default init values
//  | SCRIPT_REGISTER(enemy_behaviour_script_t, .val = 12, .val2 = 2) // init values
//  | SCRIPT_REGISTER(enemy_behaviour_script_t, ENEMY_BEHAVIOUR_SCRIPT_T_INIT); // macro init values
//  |
//  | // clear arrays
//  | SCRIPTS_CLEAR_FUNC_START();
//  |   SCRIPTS_CLEAR_FUNC_SCRIPT(projectile_script_t);
//  |   SCRIPTS_CLEAR_FUNC_SCRIPT(player_controller_script_t);
//  | SCRIPTS_CLEAR_FUNC_END();
//  | // generic remove func
//  | SCRIPT_REMOVE_FUNC_GENERIC_START();
//  |   SCRIPT_REMOVE_FUNC_GENERIC_SCRIPT(test_script_t);
//  |   SCRIPT_REMOVE_FUNC_GENERIC_SCRIPT(another_script_t);
//  |   ...
//  | SCRIPT_REMOVE_FUNC_GENERIC_END();
//  | // get type str func
//  | SCRIPT_GET_TYPE_STR_FUNC_START();
//  |   SCRIPT_GET_TYPE_STR_FUNC_SCRIPT(projectile_script_t);
//  |   SCRIPT_GET_TYPE_STR_FUNC_SCRIPT(player_controller_script_t);
//  |   ...
//  | SCRIPT_GET_TYPE_STR_FUNC_END();
//  |
//  | // gets run in SCRIPT_ADD()
//  | // void scripts_init()
//  |
//  | void scripts_update()
//  | {
//  |   SCRIPT_RUN_UPDATE(test_script_t);
//  | }
//  | 
//  | void SCRIPT_REGISTER_TRIGGER_CALLBACK_FUNC(fps_controller_script_t)  
//  | {
//  |   PF("%d collided with trigger: %d\n", this->id, trigger->id);
//  | }
//  | void SCRIPT_REGISTER_COLLISION_CALLBACK_FUNC(fps_controller_script_t)
//  | {
//  |   PF("%d collided with: %d\n", this->id, collider->id);
//  | }
//  | 
//  | void SCRIPT_INIT(test_script_t)
//  | {
//  |   entity_t* e = state_entity_get(script->entity_id);
//  |   PF("test_script_t on entity: %d\n", e->id);
//  |
//  |   SCRIPT_REGISTER_TRIGGER_CALLBACK(fps_controller_script_t, script->entity_id);
//  |   SCRIPT_REGISTER_COLLISION_CALLBACK(fps_controller_script_t, script->entity_id);
//  | }
//  | void SCRIPT_UPDATE(test_script_t)
//  | {
//  |   entity_t* e = state_entity_get(script->entity_id);
//  |   PF("test_script_t on entity: %d\n", e->id);
//  | }
//  | void SCRIPT_CLEANUP(test_script_t)
//  | {
//  |   free(x);
//  | }
//  in script_file.h
//  | typedef struct
//  | {
//  |   int entity_id;  // required, needs to be first
//  |   bool is_dead;   // required
//  |   ... 
//  | }test_script_t;
//  | 
//  | SCRIPT_DECL(test_script_t);
//  anywhere.c
//  | test_script_t* script = SCRIPT_ADD(test_script_t, e->id);
//  | script->... = ...;
//  somewhere.c
//  |    ASSERT(projectile->script_uids_pos > 0);
//  |    projectile_script_t* proj_script = SCRIPT_GET(projectile_script_t, projectile->script_uids[0]);
//  |    P_F32(proj_script->alive_t);
//  |    entity_t* e = state_entity_get(id);
//  |    enemy_behaviour_script_t* enemy_behaviour = SCRIPT_ENTITY_GET(enemy_behaviour_script_t, e);
//  |    enemy_behaviour->var = 10;
//  |
//  |    // remove script, SCRIPT_REMOVE() is faster
//  |    bool succses = SCRIPT_REOMVE(projectile_script_t, projectile->script_uids[0]);
//  |    or
//  |    bool succses = SCRIPT_REOMVE_GENERIC(projectile->script_uids[0]);  // slower


// scripts ---------------------------------------------------
// @DOC: masks for extracting the values from uids
#define SCRIPT_UID_TYPE_BIT_COUNT    13
#define SCRIPT_UID_ARR_IDX_BIT_COUNT 18
// gcc -Wpedantic/-Werror doesnt allow binary literals
#define SCRIPT_UID_TYPE_MASK    0x1FFF      // 0b00000000000000000001111111111111
#define SCRIPT_UID_ARR_IDX_MASK 0x7FFFE000  // 0b01111111111111111110000000000000
#define SCRIPT_UID_ACTIVE_MASK  0x800000000 // b10000000000000000000000000000000
#define SCRIPT_UID_TYPE_MAX     1 << SCRIPT_UID_TYPE_BIT_COUNT
#define SCRIPT_UID_ARR_IDX_MAX  1 << SCRIPT_UID_ARR_IDX_BIT_COUNT
#define SCRIPT_UID_ACTIVE_MAX   1
// script_uids:
//  00000000000000000000000000000000
//  ||----------------||----13-----|
//  ||------18--------||comp_id|
//  ||--comp_arr_idx--| 
//  |--1---
//  |active

// @DOC: generates u32 from string
//       used to convert '_type' arg in macros
//       to a number that is unique to that type
INLINE u32 state_script_gen_type_from_str(const char* str)
{
  u32 type = 0;
  for (u32 i = 0; i < strlen(str); ++i)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
  { type += (u32)str[i]; }
#pragma GCC diagnostic pop
  ERR_CHECK(type < SCRIPT_UID_TYPE_MAX, 
      "state_script_gen_uid type too big, max is: %d\n\tuse shorter string/type-name", SCRIPT_UID_TYPE_MAX);
  return type;
}
// @DOC: generate script-uid, see above
INLINE u32 state_script_gen_uid(u32 type, u32 arr_idx)
{
  ERR_CHECK(type < SCRIPT_UID_TYPE_MAX, 
      "state_script_gen_uid type too big, max is: %d\n", SCRIPT_UID_TYPE_MAX);
  u32 uid = type; // comp_id
  
  ERR_CHECK(arr_idx < SCRIPT_UID_ARR_IDX_MAX, 
      "state_script_gen_uid arr_idx too big, max is: %d\n", SCRIPT_UID_ARR_IDX_MAX);
  uid |= arr_idx << SCRIPT_UID_TYPE_BIT_COUNT;

  uid |= (u32)(1 << 31);     // active

  return uid;
}
// @DOC: generate uid from script-struct-type and its array index
//       f.e. u32 uid = SCRIPT_GEN_UID(script_test_t, 3);
#define SCRIPT_GEN_UID(_type, _arr_idx) state_script_gen_uid(state_script_gen_type_from_str(#_type), _arr_idx)

// @DOC: expands to the type of the given script uid
#define SCRIPT_UID_GET_TYPE(uid)    ( (uid) & SCRIPT_UID_TYPE_MASK )
// @DOC: expands to the array idx of the given script uid
#define SCRIPT_UID_GET_IDX(uid)     ( ((uid) & SCRIPT_UID_ARR_IDX_MASK)>>SCRIPT_UID_TYPE_BIT_COUNT )

// @NOTE: @TODO: have to remove this replaced by script_t->is_dead
// // @DOC: expands to bool indicating whether acript is active, based on uid
// #define SCRIPT_UID_GET_ACTIVE(uid)  (bool)( ( (uid) & SCRIPT_UID_ACTIVE_MASK )>>31 )
// 
// // @DOC: sets uids active bit high 
// #define SCRIPT_UID_ACTIVATE(uid)    uid |= ( 1 << 31 )      
// // @DOC: flip uids active bit
// //       if high->low, if low->high
// #define SCRIPT_UID_FLIP_ACTIVE(uid) uid ^= ( 1 << 31 )      
// // @DOC: sets uids active bit low 
// #define SCRIPT_UID_DEACTIVATE(uid)  uid &= ~( 1 << 31 )      

// @DOC: print script uid
#define P_SCRIPT_UID(v) PF_COLOR(PF_CYAN); printf("script-uid"); PF_STYLE_RESET(); printf(": %s\n", #v);    \
                        printf("\t-> bin:  "); PF_BIN32(v);                                                 \
                        printf("\t-> type: %d\n", SCRIPT_UID_GET_TYPE(v));                                  \
                        printf("\t-> idx:  %d\n", SCRIPT_UID_GET_IDX(v));                                   \
                        printf("\t-> act:  %s\n", STR_BOOL( SCRIPT_UID_GET_ACTIVE(v) ));                    \
                        PF_IF_LOC();


// SCRIPT_GET_MACROS ----------------------------------------------------------------------

// #define SCRIPT_GET_ALIVE(script) ( *( (bool*)(script + sizeof(u32)) ) )
// #define SCRIPT_SET_ALIVE(script) ( (bool*)(script + sizeof(u32)) )

// SCRIPT_ADD -----------------------------------------------------------------------------

// @DOC: call to add script to entity with id _entity_id
//       returns pointer to script
#define SCRIPT_ADD(_name, _entity_id)	scripts_add_##_name(_entity_id)

// @DOC: expands to the name of the function to add script to entity
#define SCRIPT_ADD_NAME_N(_name)		scripts_add_##_name(int entity_id)
#define SCRIPT_ADD_NAME(_type)			SCRIPT_ADD_N(_type)

#define SCRIPT_ADD_FUNC_DECL_N(_type, _name) _type* SCRIPT_ADD_NAME_N(_name)
#define SCRIPT_ADD_FUNC_N(_type, _name, ...)                          \
  SCRIPT_ADD_FUNC_DECL_N(_type, _name)                                \
  {                                                                   \
    /* PF("added script %s\n", #_name); */                            \
    _type script = {__VA_ARGS__}; /* va_args is init values */        \
    script.is_dead = false;                                           \
    int*  entity_id_ptr = (int*)(&script);                            \
    *entity_id_ptr = entity_id;                                       \
    int idx = 0;                                                      \
    /* replace dead script or add new one */                          \
    if (_name##_dead_arr_len > 0)                                     \
    {                                                                 \
      idx = (int)arrpop(_name##_dead_arr);                            \
      _name##_dead_arr_len--;                                         \
      _name##_arr[idx] = script;                                      \
    }                                                                 \
    else                                                              \
    {                                                                 \
      arrput(_name##_arr, script);                                    \
      _name##_arr_len++;                                              \
      idx = (int)_name##_arr_len -1;                                  \
    }                                                                 \
    /* generate uid and give to entity */                             \
    u32 uid = SCRIPT_GEN_UID(_type, (u32)idx);                        \
    entity_t* e = state_entity_get(entity_id);                        \
    ENTITY_ADD_SCRIPT(e, uid);                                        \
    /* PF("added script '%s' to entity: %d\n", #_type, entity_id); */ \
    /* run init */                                                    \
    SCRIPT_INIT_NAME_N(_name)(&_name##_arr[idx]);                     \
    return &_name##_arr[idx];                                         \
  }                                      

// @DOC: pointer to add func used in entity_template.c/entity_table.c
#define SCRIPT_ADD_PTR(_name)	          __scripts_add_##_name##_no_rtn
#define SCRIPT_ADD_PTR_NAME(_name)	    __scripts_add_##_name##_no_rtn(int entity_id)
#define SCRIPT_ADD_PTR_FUNC_DECL(_name) void SCRIPT_ADD_PTR_NAME(_name)
#define SCRIPT_ADD_PTR_FUNC_N(_name)                                  \
  SCRIPT_ADD_PTR_FUNC_DECL(_name)                                     \
  { SCRIPT_ADD(_name, entity_id); }


// SCRIPT_GET -----------------------------------------------------------------------------
// SCRIPT_ENTITY_GET

#define SCRIPT_GET_FUNC_NAME(_type) __scripts_get_script_##_type
// @DOC: get script by type and script uid, can return NULL
#define SCRIPT_GET(_type, uid)      SCRIPT_GET_FUNC_NAME(_type)(uid)
#define SCRIPT_GET_FUNC_DECL(_type) _type* SCRIPT_GET_FUNC_NAME(_type)(u32 uid)
#define SCRIPT_GET_FUNC_N(_type, _name)                                                     \
  SCRIPT_GET_FUNC_DECL(_type)                                                               \
  {                                                                                         \
    u32 type = SCRIPT_UID_GET_TYPE(uid);                                                    \
    u32 idx  = SCRIPT_UID_GET_IDX(uid);                                                     \
    /* check if uid specifies requested type */                                             \
    if (state_script_gen_type_from_str(#_type) != type)                                     \
    {                                                                                       \
      P_ERR("uid given to SCRIPT_GET(%s, %u), didnt match provided type\n", #_type, uid);   \
      return NULL;                                                                          \
    }                                                                                       \
    /* check idx isnt out-of-bounds */                                                      \
    ERR_CHECK(idx < _name##_arr_len,                                                        \
        "idx: '%d' in SCRIPT_GET() not valid, min: 0, max: %d, type: %s\n",                 \
        idx, _name##_arr_len, #_type);                                                      \
    return &_name##_arr[idx];                                                               \
  }
// #define SCRIPT_GET_FUNC(_type)  SCRIPT_GET_FUNC_N(_type, _type)

#define SCRIPT_ENTITY_GET_FUNC_NAME(_type)      __scripts_get_script_entity_##_type
// @DOC: get script by type and entity_t*, can return NULL
#define SCRIPT_ENTITY_GET(_type, e)             SCRIPT_ENTITY_GET_FUNC_NAME(_type)(e)
#define SCRIPT_ENTITY_GET_FUNC_DECL(_type)      _type* SCRIPT_ENTITY_GET_FUNC_NAME(_type)(entity_t* e)
#define SCRIPT_ENTITY_GET_FUNC_N(_type, _name)                                                              \
  SCRIPT_ENTITY_GET_FUNC_DECL(_type)                                                                        \
  {                                                                                                         \
    if (e->script_uids_pos <= 0)                                                                            \
    {                                                                                                       \
      P_ERR("tried to SCRIPT_ENTITY_GET(%s) on entity: %d, but has no scripts attached\n", #_type, e->id);  \
      return NULL;                                                                                          \
    }                                                                                                       \
    for (int i = 0;  i < e->script_uids_pos; ++i)                                                           \
    {                                                                                                       \
      u32 type = SCRIPT_UID_GET_TYPE(e->script_uids[i]);                                                    \
      u32 idx  = SCRIPT_UID_GET_IDX(e->script_uids[i]);                                                     \
      if (state_script_gen_type_from_str(#_type) == type)                                                   \
      {                                                                                                     \
        /* check idx isnt out-of-bounds */                                                                  \
        ERR_CHECK(idx < _name##_arr_len,                                                                    \
            "idx: '%d' in SCRIPT_ENTITY_GET() not valid, min: 0, max: %d, type: %s\n",                      \
            idx, _name##_arr_len, #_type);                                                                  \
        return &_name##_arr[idx];                                                                           \
      }                                                                                                     \
    }                                                                                                       \
    P_ERR("tried to SCRIPT_ENTITY_GET(%s) on entity: %d, but has no script of type '%s'\n",                 \
          #_type, e->id, #_type);                                                                           \
    return NULL;                                                                                            \
  }

// SCRIPT_REMOVE --------------------------------------------------------------------------

#define SCRIPT_REMOVE_FUNC_NAME(_type) scripts_remove_script_##_type
#define SCRIPT_REMOVE(_type, uid) SCRIPT_REMOVE_FUNC_NAME(_type)(uid)
#define SCRIPT_REMOVE_FUNC_N(_type, _name)                                                  \
  bool SCRIPT_REMOVE_FUNC_NAME(_type)(u32 uid)                                              \
  {                                                                                         \
    u32 type = SCRIPT_UID_GET_TYPE(uid);                                                    \
    u32 idx  = SCRIPT_UID_GET_IDX(uid);                                                     \
    /* check if uid specifies requested type */                                             \
    if (state_script_gen_type_from_str(#_type) != type)                                       \
    {                                                                                       \
      P_ERR("uid given to SCRIPT_GET(%s, %u), didnt match provided type\n", #_type, uid);   \
      return false;                                                                         \
    }                                                                                       \
    /* check idx isnt out-of-bounds */                                                      \
    if (idx < _name##_arr_len)                                                              \
    {                                                                                       \
      P_ERR("idx: '%d' in SCRIPT_GET() not valid, min: 0, max: %d, type: %s\n",             \
        idx, _name##_arr_len, #_type);                                                      \
      return false;                                                                         \
    }                                                                                       \
    /* arrdel(_name##_arr, idx); */                                                         \
    /* _name##_arr_len--;        */                                                         \
    _name##_arr[idx].is_dead = true;                                                        \
    arrput(_name##_dead_arr, idx);                                                          \
    _name##_dead_arr_len++;                                                                 \
    return true;                                                                            \
  }

// -- SCRIPT_REMOVE_GENERIC --
        
#define SCRIPT_REMOVE_FUNC_GENERIC_NAME script_remove_generic

// @DOC: generic remove func for when not knowing type
//       ! slower than SCRIPT_REMOVE
#define SCRIPT_REMOVE_GENERIC(uid)    SCRIPT_REMOVE_FUNC_GENERIC_NAME(uid)

#define SCRIPT_REMOVE_FUNC_GENERIC_START()          \
  bool SCRIPT_REMOVE_FUNC_GENERIC_NAME(u32 uid)     \
  {                                                 \
    u32 type = SCRIPT_UID_GET_TYPE(uid);            \
    u32 idx  = SCRIPT_UID_GET_IDX(uid);             
#define SCRIPT_REMOVE_FUNC_GENERIC_SCRIPT_N(_type, _name)                                 \
    if (type == state_script_gen_type_from_str(#_type) )                                  \
    {                                                                                     \
      /* check idx isnt out-of-bounds */                                                  \
      ERR_CHECK(idx < _name##_arr_len,                                                    \
          "idx: '%d' in SCRIPT_REMOVE_GENERIC() not valid, min: 0, max: %d, type: %s\n",  \
          idx, _name##_arr_len, #_type);                                                  \
      /* arrdel(_name##_arr, idx); */                                                     \
      /* _name##_arr_len--;        */                                                     \
      SCRIPT_CLEANUP_NAME_N(_name)(&_name##_arr[idx]);                                    \
      _name##_arr[idx].is_dead = true;                                                    \
      arrput(_name##_dead_arr, idx);                                                      \
      _name##_dead_arr_len++;                                                             \
      return true;                                                                        \
    }
#define SCRIPT_REMOVE_FUNC_GENERIC_END()  \
    /* failed */                          \
    return false;                         \
  }
#define SCRIPT_REMOVE_FUNC_GENERIC_SCRIPT(_type)  SCRIPT_REMOVE_FUNC_GENERIC_SCRIPT_N(_type, _type)

// SCRIPT_GET_TYPE_STR --------------------------------------------------------------------

#define SCRIPT_GET_TYPE_STR_FUNC_NAME script_get_type_generic

// @DOC: get string of type, i.e. "test_script_t" 
#define SCRIPT_GET_TYPE_STR(uid)    SCRIPT_GET_TYPE_STR_FUNC_NAME(uid)

#define SCRIPT_GET_TYPE_STR_FUNC_START()            \
  char* SCRIPT_GET_TYPE_STR_FUNC_NAME(u32 uid)      \
  {                                                 \
    u32 type = SCRIPT_UID_GET_TYPE(uid);            \
    u32 idx  = SCRIPT_UID_GET_IDX(uid);             
#define SCRIPT_GET_TYPE_STR_FUNC_SCRIPT_N(_type, _name)                                   \
    if (type == state_script_gen_type_from_str(#_type) )                                  \
    {                                                                                     \
      /* check idx isnt out-of-bounds */                                                  \
      ERR_CHECK(idx < _name##_arr_len,                                                    \
          "idx: '%d' in SCRIPT_GET_TYPE_STR() not valid, min: 0, max: %d, type: %s\n",    \
          idx, _name##_arr_len, #_type);                                                  \
      return #_type;                                                                      \
    }
#define SCRIPT_GET_TYPE_STR_FUNC_END()                                                    \
    /* failed */                                                                          \
    return "-";                                                                           \
  }
#define SCRIPT_GET_TYPE_STR_FUNC_SCRIPT(_type)  SCRIPT_GET_TYPE_STR_FUNC_SCRIPT_N(_type, _type)

// SCRIPT_REGISTER_CALLBACK ---------------------------------------------------------------

// @NOTE: see above for example

// @DOC: name of function registered as callback in event_sys by SCRIPT_REGISTER_TRIGGER_CALLBACK
#define SCRIPT_REGISTER_TRIGGER_CALLBACK_FUNC_NAME(_type)   _type##_trigger_callback
// @DOC: function registered as callback in event_sys by SCRIPT_REGISTER_TRIGGER_CALLBACK
#define SCRIPT_REGISTER_TRIGGER_CALLBACK_FUNC(_type)        SCRIPT_REGISTER_TRIGGER_CALLBACK_FUNC_NAME(_type)(entity_t* this, entity_t* trigger) 
// @DOC: register script for trigger callback, on entity with id, i.e. script->entity_id
#define SCRIPT_REGISTER_TRIGGER_CALLBACK(_type, id)         event_sys_register_phys_trigger_specific(SCRIPT_REGISTER_TRIGGER_CALLBACK_FUNC_NAME(_type), (id))

// @DOC: name of function registered as callback in event_sys by SCRIPT_REGISTER_COLLISION_CALLBACK
#define SCRIPT_REGISTER_COLLISION_CALLBACK_FUNC_NAME(_type) _type##_collision_callback
// @DOC: function registered as callback in event_sys by SCRIPT_REGISTER_COLLISION_CALLBACK
#define SCRIPT_REGISTER_COLLISION_CALLBACK_FUNC(_type)      SCRIPT_REGISTER_COLLISION_CALLBACK_FUNC_NAME(_type)(entity_t* this, entity_t* collider) 
// @DOC: register script for collision callback, on entity with id, i.e. script->entity_id
#define SCRIPT_REGISTER_COLLISION_CALLBACK(_type, id)       event_sys_register_phys_collision_specific(SCRIPT_REGISTER_COLLISION_CALLBACK_FUNC_NAME(_type), (id))

// SCRIPT_REGISTER ------------------------------------------------------------------------

// @DOC: expands to the array declaration
//       and the funtion that lets u add 
//       a new script, aka. SCRIPT_ADD()
//       SCRIPT_ADD_PTR_NAME func is whats 
//       called when giving SCRIPT_ADD_PTR
//       to entity_template_t in entity_table.c
//       cause it cant have a return type 
#define SCRIPT_REGISTER_N(_type, _name, ...)        \
  _type* _name##_arr = NULL;                        \
  u32    _name##_arr_len = 0;                       \
  u32*   _name##_dead_arr = NULL;                   \
  u32    _name##_dead_arr_len = 0;                  \
  SCRIPT_GET_FUNC_N(_type, _name)                   \
  SCRIPT_ENTITY_GET_FUNC_N(_type, _name)            \
  SCRIPT_REMOVE_FUNC_N(_type, _name)                \
  /* va_args is init value */                       \
  SCRIPT_ADD_FUNC_N(_type, _name, __VA_ARGS__)      \
  SCRIPT_ADD_PTR_FUNC_N(_name)

// va_args is init value
#define SCRIPT_REGISTER(_type, ...)  SCRIPT_REGISTER_N(_type, _type, __VA_ARGS__) 

// SCRIPT_CLEAR ---------------------------------------------------------------------------

#define SCRIPTS_CLEAR_FUNC_NAME scripts_clear

// @DOC: clear all arrays for scripts
//       ! doesnt remove scripts from entities
//         use in state_clear_scene() only, prob.
#define SCRIPTS_CLEAR()  SCRIPTS_CLEAR_FUNC_NAME()

// @DOC: see above ~:100 for example
//       clears all script arrays
//       declared in games/games.h
#define SCRIPTS_CLEAR_FUNC_START()           \
  void SCRIPTS_CLEAR_FUNC_NAME()             \
  { 
#define SCRIPTS_CLEAR_FUNC_SCRIPT_N(_name)            \
  for (int i= 0; i < (int)_name##_arr_len; ++i)       \
  { SCRIPT_CLEANUP_NAME_N(_name)(&_name##_arr[i]); }  \
  _name##_arr = NULL;                                 \
  _name##_arr_len = 0;                                \
  _name##_dead_arr = NULL;                            \
  _name##_dead_arr_len = 0;                 
#define SCRIPTS_CLEAR_FUNC_END()            \
  }
#define SCRIPTS_CLEAR_FUNC_SCRIPT(_type)   SCRIPTS_CLEAR_FUNC_SCRIPT_N(_type)

// SCRIPT_FUNCS ---------------------------------------------------------------------------
//  -> init(), update(), cleanup()

// @DOC: exapnds to the name of the 'init' fucntion
//       for the script
#define SCRIPT_INIT_NAME_N(_name)   scripts_init_##_name
#define SCRIPT_INIT_NAME(_type)     SCRIPT_INIT_NAME_N(_type)
// @DOC: exapnds to the 'init' fucntion for the script
#define SCRIPT_INIT_N(_type, _name)     SCRIPT_INIT_NAME_N(_type)(_type* script)
#define SCRIPT_INIT(_type)              SCRIPT_INIT_N(_type, _type)
#define SCRIPT_INIT_DECL(_type, _name)  void SCRIPT_INIT_N(_type, _name)

// @NOTE: now gets called in SCRIPT_ADD()
// // @DOC: expands to the for-loop in scripts_update() function
// //       that calls SCRIPT_INIT() on all scripts
// #define SCRIPT_RUN_INIT_N(_name)                            \x
// for (u32 i = 0; i < _name##_arr_len; ++i)                   \x
// {                                                           \x
//   SCRIPT_INIT_NAME_N(_name)(&_name##_arr[i]);               \x
// }
// #define SCRIPT_RUN_INIT(_type) SCRIPT_RUN_INIT_N(_type)

// @DOC: exapnds to the name of the 'update' fucntion
//       for the script
#define SCRIPT_UPDATE_NAME_N(_name)   scripts_update_##_name
#define SCRIPT_UPDATE_NAME(_type)     SCRIPT_UPDATE_NAME_N(_type)
// @DOC: exapnds to the 'update' fucntion for the script
#define SCRIPT_UPDATE_N(_type, _name)     SCRIPT_UPDATE_NAME_N(_type)(_type* script)
#define SCRIPT_UPDATE(_type)              SCRIPT_UPDATE_N(_type, _type)
#define SCRIPT_UPDATE_DECL(_type, _name)  void SCRIPT_UPDATE_N(_type, _name)

// @DOC: expands to the for-loop in scripts_update() function
//       that calls SCRIPT_UPDATE() on all scripts
#define SCRIPT_RUN_UPDATE_N(_name)                            \
for (u32 i = 0; i < _name##_arr_len; ++i)                     \
{                                                             \
  if (!_name##_arr[i].is_dead)                                \
  { SCRIPT_UPDATE_NAME_N(_name)(&_name##_arr[i]); }           \
}
#define SCRIPT_RUN_UPDATE(_type) SCRIPT_RUN_UPDATE_N(_type)


// @DOC: exapnds to the name of the 'update' fucntion
//       for the script
#define SCRIPT_CLEANUP_NAME_N(_name)   scripts_cleanup_##_name
#define SCRIPT_CLEANUP_NAME(_type)     SCRIPT_CLEANUP_NAME_N(_type)
// @DOC: exapnds to the 'update' fucntion for the script
#define SCRIPT_CLEANUP_N(_type, _name)     SCRIPT_CLEANUP_NAME_N(_type)(_type* script)
#define SCRIPT_CLEANUP(_type)              SCRIPT_CLEANUP_N(_type, _type)
#define SCRIPT_CLEANUP_DECL(_type, _name)  void SCRIPT_CLEANUP_N(_type, _name)

// @DOC: expands to the for-loop in scripts_update() function
//       that calls SCRIPT_UPDATE() on all scripts
#define SCRIPT_RUN_CLEANUP_N(_name)                            \
for (u32 i = 0; i < _name##_arr_len; ++i)                     \
{                                                             \
  if (!_name##_arr[i].is_dead)                                \
  { SCRIPT_CLEANUP_NAME_N(_name)(&_name##_arr[i]); }           \
}
#define SCRIPT_RUN_CLEANUP(_type) SCRIPT_RUN_UPDATE_N(_type)

// SCRIPT_DECL ----------------------------------------------------------------------------

// @DOC: expands to func declaration for functions created by other macros
#define SCRIPT_DECL_N(_type, _name)             \
SCRIPT_ADD_PTR_FUNC_DECL(_name);                \
SCRIPT_ADD_FUNC_DECL_N(_type, _name);           \
SCRIPT_GET_FUNC_DECL(_type);                    \
SCRIPT_ENTITY_GET_FUNC_DECL(_type);             \
SCRIPT_INIT_DECL(_type, _name);                 \
SCRIPT_UPDATE_DECL(_type, _name);               \
SCRIPT_CLEANUP_DECL(_type, _name)

#define SCRIPT_DECL(_type) SCRIPT_DECL_N(_type, _type)




#endif
