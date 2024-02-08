#ifndef CORE_STATE_SCRIPTS_H
#define CORE_STATE_SCRIPTS_H

#include "global/global.h"
#include "core/types/types.h"

// scripts ---------------------------------------------------
// @DOC: masks for extracting the values from uids
#define SCRIPT_UID_TYPE_BIT_COUNT    13
#define SCRIPT_UID_ARR_IDX_BIT_COUNT 18
#define SCRIPT_UID_TYPE_MASK    0b00000000000000000001111111111111
#define SCRIPT_UID_ARR_IDX_MASK 0b01111111111111111110000000000000
#define SCRIPT_UID_ACTIVE_MASK  0b10000000000000000000000000000000
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
INLINE u32 ecs_script_gen_type_from_str(const char* str)
{
  u32 type = 0;
  for (u32 i = 0; i < strlen(str); ++i)
  { type += str[i]; }
  ERR_CHECK(type < SCRIPT_UID_TYPE_MAX, 
      "ecs_script_gen_uid type too big, max is: %d\n\tuse shorter string/type-name", SCRIPT_UID_TYPE_MAX);
  return type;
}
// @DOC: generate script-uid, see above
INLINE u32 ecs_script_gen_uid(u32 type, u32 arr_idx)
{
  ERR_CHECK(type < SCRIPT_UID_TYPE_MAX, 
      "ecs_script_gen_uid type too big, max is: %d\n", SCRIPT_UID_TYPE_MAX);
  u32 uid = type; // comp_id
  
  ERR_CHECK(arr_idx < SCRIPT_UID_ARR_IDX_MAX, 
      "ecs_script_gen_uid arr_idx too big, max is: %d\n", SCRIPT_UID_ARR_IDX_MAX);
  uid |= arr_idx << SCRIPT_UID_TYPE_BIT_COUNT;

  uid |= 1 << 31;     // active

  return uid;
}
// @DOC: generate uid from script-struct-type and its array index
//       f.e. u32 uid = SCRIPT_GEN_UID(script_test_t, 3);
#define SCRIPT_GEN_UID(_type, _arr_idx) ecs_script_gen_uid(ecs_script_gen_type_from_str(#_type), _arr_idx)

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

// --- script macros ---
// example:
//  in script_file.h
//  | typedef struct
//  | {
//  |   u32 entity_id;  // required
//  |   bool is_dead;   // required
//  |   ...
//  | }test_script_t;
//  in script_file.c
//  | SCRIPT_REGISTER(test_script_t);
//  |
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
//  | // {
//  | //   SCRIPT_RUN_INIT(test_script_t);
//  | // }
//  | void scripts_update()
//  | {
//  |   SCRIPT_RUN_UPDATE(test_script_t);
//  | }
//  | 
//  | void SCRIPT_INIT(test_script_t)
//  | {
//  |   entity_t* e = ecs_entity_get(script->entity_id);
//  |   PF("test_script_t on entity: %d\n", e->id);
//  | }
//  | void SCRIPT_UPDATE(test_script_t)
//  | {
//  |   entity_t* e = ecs_entity_get(script->entity_id);
//  |   PF("test_script_t on entity: %d\n", e->id);
//  | }
//  in script_file.h
//  | typedef struct
//  | {
//  |   u32 entity_id;  // required, needs to be first
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
//  |
//  |    // remove script, SCRIPT_REMOVE() is faster
//  |    bool succses = SCRIPT_REOMVE(projectile_script_t, projectile->script_uids[0]);
//  |    or
//  |    bool succses = SCRIPT_REOMVE_GENERIC(projectile->script_uids[0]);

// SCRIPT_GET_MACROS ----------------------------------------------------------------------

// #define SCRIPT_GET_ALIVE(script) ( *( (bool*)(script + sizeof(u32)) ) )
// #define SCRIPT_SET_ALIVE(script) ( (bool*)(script + sizeof(u32)) )

// SCRIPT_ADD -----------------------------------------------------------------------------

// @DOC: call to add script to entity with id _entity_id
//       returns pointer to script
#define SCRIPT_ADD(_name, _entity_id)	scripts_add_##_name(_entity_id)
// @DOC: pointer to add func used in entity_template.c/entity_table.c
#define SCRIPT_ADD_PTR(_name)	      scripts_add_##_name##_no_rtn
#define SCRIPT_ADD_PTR_NAME(_name)	scripts_add_##_name##_no_rtn(u32 entity_id)

// @DOC: expands to the name of the function to add script to entity
#define SCRIPT_ADD_NAME_N(_name)		scripts_add_##_name(u32 entity_id)
#define SCRIPT_ADD_NAME(_type)			SCRIPT_ADD_N(_type)

#define SCRIPT_ADD_FUNC_N(_type, _name)                               \
  _type* SCRIPT_ADD_NAME_N(_name)                                     \
  {                                                                   \
    _type script;                                                     \
    script.is_dead = false;                                           \
    u32*  entity_id_ptr = (u32*)(&script);                            \
    *entity_id_ptr = entity_id;                                       \
    u32 idx = -1;                                                     \
    /* replace dead script or add new one */                          \
    if (_name##_dead_arr_len > 0)                                     \
    {                                                                 \
      idx = arrpop(_name##_dead_arr);                                 \
      _name##_dead_arr_len--;                                         \
      _name##_arr[idx] = script;                                      \
    }                                                                 \
    else                                                              \
    {                                                                 \
      arrput(_name##_arr, script);                                    \
      _name##_arr_len++;                                              \
      idx = _name##_arr_len -1;                                       \
    }                                                                 \
    /* generate uid and give to entity */                             \
    u32 uid = SCRIPT_GEN_UID(_type, idx);                             \
    entity_t* e = ecs_entity_get(entity_id);                          \
    ENTITY_ADD_SCRIPT(e, uid);                                        \
    PF("added script '%s' to entity: %d\n", #_type, entity_id);       \
    /* run init */                                                    \
    SCRIPT_INIT_NAME_N(_name)(&_name##_arr[idx]);                     \
    return &_name##_arr[idx];                                         \
  }                                      

#define SCRIPT_ADD_PTR_FUNC_N(_name)                                  \
  void SCRIPT_ADD_PTR_NAME(_name)                                     \
  { SCRIPT_ADD(_name, entity_id); }


// SCRIPT_GET -----------------------------------------------------------------------------

#define SCRIPT_GET_FUNC_NAME(_type) scripts_get_script_##_type
#define SCRIPT_GET(_type, uid)      SCRIPT_GET_FUNC_NAME(_type)(uid)
#define SCRIPT_GET_FUNC_N(_type, _name)                                                     \
  _type* SCRIPT_GET_FUNC_NAME(_type)(u32 uid)                                               \
  {                                                                                         \
    u32 type = SCRIPT_UID_GET_TYPE(uid);                                                    \
    u32 idx  = SCRIPT_UID_GET_IDX(uid);                                                     \
    /* check if uid specifies requested type */                                             \
    if (ecs_script_gen_type_from_str(#_type) != type)                                       \
    {                                                                                       \
      P_ERR("uid given to SCRIPT_GET(%s, %u), didnt match provided type\n", #_type, uid);   \
      return NULL;                                                                          \
    }                                                                                       \
    /* check idx isnt out-of-bounds */                                                      \
    ERR_CHECK(idx >= 0 && idx < _name##_arr_len,                                            \
        "idx: '%d' in SCRIPT_GET() not valid, min: 0, max: %d, type: %s\n",                 \
        idx, _name##_arr_len, #_type);                                                      \
    return &_name##_arr[idx];                                                               \
  }
#define SCRIPT_GET_FUNC(_type)  SCRIPT_GET_FUNC_N(_type, _type)


// SCRIPT_REMOVE --------------------------------------------------------------------------

#define SCRIPT_REMOVE_FUNC_NAME(_type) scripts_remove_script_##_type
#define SCRIPT_REMOVE(_type, uid) SCRIPT_REMOVE_FUNC_NAME(_type)(uid)
#define SCRIPT_REMOVE_FUNC_N(_type, _name)                                                  \
  bool SCRIPT_REMOVE_FUNC_NAME(_type)(u32 uid)                                              \
  {                                                                                         \
    u32 type = SCRIPT_UID_GET_TYPE(uid);                                                    \
    u32 idx  = SCRIPT_UID_GET_IDX(uid);                                                     \
    /* check if uid specifies requested type */                                             \
    if (ecs_script_gen_type_from_str(#_type) != type)                                       \
    {                                                                                       \
      P_ERR("uid given to SCRIPT_GET(%s, %u), didnt match provided type\n", #_type, uid);   \
      return false;                                                                         \
    }                                                                                       \
    /* check idx isnt out-of-bounds */                                                      \
    if (idx >= 0 && idx < _name##_arr_len)                                                  \
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
    if (type == ecs_script_gen_type_from_str(#_type) )                                    \
    {                                                                                     \
      /* check idx isnt out-of-bounds */                                                  \
      ERR_CHECK(idx >= 0 && idx < _name##_arr_len,                                        \
          "idx: '%d' in SCRIPT_REMOVE_GENERIC() not valid, min: 0, max: %d, type: %s\n",  \
          idx, _name##_arr_len, #_type);                                                  \
      /* arrdel(_name##_arr, idx); */                                                     \
      /* _name##_arr_len--;        */                                                     \
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
    if (type == ecs_script_gen_type_from_str(#_type) )                                    \
    {                                                                                     \
      /* check idx isnt out-of-bounds */                                                  \
      ERR_CHECK(idx >= 0 && idx < _name##_arr_len,                                        \
          "idx: '%d' in SCRIPT_GET_TYPE_STR() not valid, min: 0, max: %d, type: %s\n",    \
          idx, _name##_arr_len, #_type);                                                  \
      return #_type;                                                                      \
    }
#define SCRIPT_GET_TYPE_STR_FUNC_END()                                                    \
    /* failed */                                                                          \
    return "-";                                                                           \
  }
#define SCRIPT_GET_TYPE_STR_FUNC_SCRIPT(_type)  SCRIPT_GET_TYPE_STR_FUNC_SCRIPT_N(_type, _type)

// SCRIPT_REGISTER ------------------------------------------------------------------------

// @DOC: expands to the array declaration
//       and the funtion that lets u add 
//       a new script, aka. SCRIPT_ADD()
//       SCRIPT_ADD_PTR_NAME func is whats 
//       called when giving SCRIPT_ADD_PTR
//       to entity_template_t in entity_table.c
//       cause it cant have a return type 
#define SCRIPT_REGISTER_N(_type, _name)  \
  _type* _name##_arr = NULL;             \
  u32    _name##_arr_len = 0;            \
  u32*   _name##_dead_arr = NULL;        \
  u32    _name##_dead_arr_len = 0;       \
  SCRIPT_GET_FUNC_N(_type, _name);       \
  SCRIPT_REMOVE_FUNC_N(_type, _name);    \
  SCRIPT_ADD_FUNC_N(_type, _name);       \
  SCRIPT_ADD_PTR_FUNC_N(_name);

#define SCRIPT_REGISTER(_type)  SCRIPT_REGISTER_N(_type, _type) 

// SCRIPT_FUNCS ---------------------------------------------------------------------------

// @DOC: exapnds to the name of the 'init' fucntion
//       for the script
#define SCRIPT_INIT_NAME_N(_name)   scripts_init_##_name
#define SCRIPT_INIT_NAME(_type)     SCRIPT_INIT_NAME_N(_type)
// @DOC: exapnds to the 'init' fucntion for the script
#define SCRIPT_INIT_N(_type, _name) SCRIPT_INIT_NAME_N(_type)(_type* script)
#define SCRIPT_INIT(_type)          SCRIPT_INIT_N(_type, _type)

// @DOC: exapnds to the name of the 'update' fucntion
//       for the script
#define SCRIPT_UPDATE_NAME_N(_name)   scripts_update_##_name
#define SCRIPT_UPDATE_NAME(_type)     SCRIPT_UPDATE_NAME_N(_type)
// @DOC: exapnds to the 'update' fucntion for the script
#define SCRIPT_UPDATE_N(_type, _name) SCRIPT_UPDATE_NAME_N(_type)(_type* script)
#define SCRIPT_UPDATE(_type)          SCRIPT_UPDATE_N(_type, _type)

// @NOTE: now gets called in SCRIPT_ADD()
// // @DOC: expands to the for-loop in scripts_update() function
// //       that calls SCRIPT_INIT() on all scripts
// #define SCRIPT_RUN_INIT_N(_name)                            \x
// for (u32 i = 0; i < _name##_arr_len; ++i)                   \x
// {                                                           \x
//   SCRIPT_INIT_NAME_N(_name)(&_name##_arr[i]);               \x
// }
// #define SCRIPT_RUN_INIT(_type) SCRIPT_RUN_INIT_N(_type)

// @DOC: expands to the for-loop in scripts_update() function
//       that calls SCRIPT_UPDATE() on all scripts
#define SCRIPT_RUN_UPDATE_N(_name)                            \
for (u32 i = 0; i < _name##_arr_len; ++i)                     \
{                                                             \
  if (!_name##_arr[i].is_dead)                                \
  { SCRIPT_UPDATE_NAME_N(_name)(&_name##_arr[i]); }           \
}
#define SCRIPT_RUN_UPDATE(_type) SCRIPT_RUN_UPDATE_N(_type)

// SCRIPT_DECL ----------------------------------------------------------------------------

// @DOC: expands to func declaration for functions created by other macros
#define SCRIPT_DECL_N(_type, _name)             \
void   scripts_add_##_name##_no_rtn();          \
_type* scripts_add_##_name();                   \
void   SCRIPT_INIT_N(_type, _name);             \
void   SCRIPT_UPDATE_N(_type, _name); 
#define SCRIPT_DECL(_type) SCRIPT_DECL_N(_type, _type)




#endif
