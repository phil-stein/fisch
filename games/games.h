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


#ifdef __cplusplus
}   // extern c
#endif

#endif  // __GAMES__H
