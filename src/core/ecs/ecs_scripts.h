#ifndef CORE_STATE_SCRIPTS_H
#define CORE_STATE_SCRIPTS_H

#include "global/global.h"
#include "core/types/types.h"

// scripts ---------------------------------------------------
// @DOC: masks for extracting the values from uids
#define SCRIPT_UID_TYPE_MASK    0b00000000000000000000000011111111
#define SCRIPT_UID_ARR_IDX_MASK 0b01111111111111111111111100000000
#define SCRIPT_UID_ACTIVE_MASK  0b10000000000000000000000000000000
#define SCRIPT_UID_TYPE_MAX     1 << 8
#define SCRIPT_UID_ARR_IDX_MAX  1 << 23
#define SCRIPT_UID_ACTIVE_MAX   1
// script_uids:
//  00000000000000000000000000000000
//  ||---------------------||--8----|
//  ||---------23----------||comp_id|
//  ||-----comp_arr_idx----| 
//  |--1---
//  |active
INLINE u32 ecs_script_gen_uid(u32 type, u32 arr_idx)
{
  ERR_CHECK(type < SCRIPT_UID_TYPE_MAX, 
      "ecs_script_gen_uid type too big, max is: %d\n", SCRIPT_UID_TYPE_MAX);
  u32 uid = type; // comp_id
  
  ERR_CHECK(arr_idx < SCRIPT_UID_ARR_IDX_MAX, 
      "ecs_script_gen_uid arr_idx too big, max is: %d\n", SCRIPT_UID_ARR_IDX_MAX);
  uid |= arr_idx << 8;

  uid |= 1 << 31;     // active

  return uid;
}
INLINE bool ecs_script_active(u32 uid)
{
  uid &= SCRIPT_UID_ACTIVE_MASK;
  return (bool)(uid >> 31);
}


// --- script macros ---
// example:
//  in script_file.c
//  | SCRIPT_REGISTER(test_script_t);
//  | 
//  | void scripts_update()
//  | {
//  |   SCRIPT_RUN(test_script_t);
//  | }
//  | 
//  | void SCRIPT_FUNC(test_script_t)
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

// @DOC: call to add script to entity with id _entity_id
//       returns pointer to script
#define SCRIPT_ADD(_name, _entity_id)	scripts_add_##_name(_entity_id)

// @DOC: expands to the name of the function to add script to entity
#define SCRIPT_ADD_NAME_N(_name)		scripts_add_##_name(u32 entity_id)
#define SCRIPT_ADD_NAME(_type)			SCRIPT_ADD_N(_type)

// @DOC: expands to the array declaration
//       and the funtion that lets u add 
//       a new script, aka. SCRIPT_ADD()
#define SCRIPT_REGISTER_N(_type, _name)     \
_type* _name##_arr = NULL;                  \
u32    _name##_arr_len = 0;                 \
_type* SCRIPT_ADD_NAME_N(_name)             \
{                                           \
  _type script;                             \
  u32*  entity_id_ptr = (u32*)(&script);    \
  *entity_id_ptr = entity_id;               \
  arrput(_name##_arr, script);              \
  _name##_arr_len++;                        \
  /*u32 uid = ecs_script_gen_uid(@TODO:);*/     \
  entity_t* e = ecs_entity_get(entity_id);  \
  ENTITY_ADD_SCRIPT(e, 0/*uid*/);                \
  return &_name##_arr[_name##_arr_len -1];  \
}
#define SCRIPT_REGISTER(_type)  SCRIPT_REGISTER_N(_type, _type) 

// @DOC: exapnds to the name of the 'run' fucntion
//       for the script
#define SCRIPT_FUNC_NAME_N(_name)   scripts_run_##_name
#define SCRIPT_FUNC_NAME(_type)     SCRIPT_FUNC_NAME_N(_type)


// @DOC: exapnds to the 'run' fucntion for the script
#define SCRIPT_FUNC_N(_type, _name) SCRIPT_FUNC_NAME_N(_type)(_type* script)
#define SCRIPT_FUNC(_type)          SCRIPT_FUNC_N(_type, _type)

// @DOC: expands to the for-loop in scripts_update() function
//       that calls SCRIPT_FUNC() on all scripts
#define SCRIPT_RUN_N(_name)                               \
for (u32 i = 0; i < _name##_arr_len; ++i)                 \
{                                                         \
  /*PASTE_2(SCRIPT_FUNC_NAME_N(_name), (&_name##_arr[i]));*/  \
  SCRIPT_FUNC_NAME_N(_name)(&_name##_arr[i]);             \
}
#define SCRIPT_RUN(_type) SCRIPT_RUN_N(_type)

// @DOC: expands to func declaration for functions created by other macros
#define SCRIPT_DECL_N(_type, _name)   \
_type* scripts_add_##_name();         \
void   SCRIPT_FUNC(_name); 
#define SCRIPT_DECL(_type) SCRIPT_DECL_N(_type, _type)


#endif
