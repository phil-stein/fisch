cmake_minimum_required(VERSION 3.20.1)

set(GAME_SOURCES 
    ${ROOT_DIR}/games/puzzle_game/entity_tags.h

    ${ROOT_DIR}/games/puzzle_game/entity_table.c
    ${ROOT_DIR}/games/puzzle_game/entity_table.h
    ${ROOT_DIR}/games/puzzle_game/material_table.c
    ${ROOT_DIR}/games/puzzle_game/material_table.h
    ${ROOT_DIR}/games/puzzle_game/shader_table.c
    ${ROOT_DIR}/games/puzzle_game/shader_table.h
    
    ${ROOT_DIR}/games/puzzle_game/puzzle_game.c
    ${ROOT_DIR}/games/puzzle_game/puzzle_game.h
    ${ROOT_DIR}/games/puzzle_game/scripts.c
    ${ROOT_DIR}/games/puzzle_game/scripts.h
    ${ROOT_DIR}/games/puzzle_game/script_fps.c
    ${ROOT_DIR}/games/puzzle_game/script_power_lever.c
    )

  # -DTERRAIN_ADDON
  # -DRRENDERER_EXTRA  // mouse_pick & outline
  set(
      GAME_COMPILE_OPTIONS
      -DRENDERER_EXTRA       
      -DGLOBAL_DEBUG
      -DOUTLINE
     )

