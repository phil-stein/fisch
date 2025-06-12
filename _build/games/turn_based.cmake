cmake_minimum_required(VERSION 3.20.1)

set(GAME_SOURCES 
    ${ROOT_DIR}/games/turn_based/entity_tags.h
    ${ROOT_DIR}/games/turn_based/entity_table.c
    ${ROOT_DIR}/games/turn_based/entity_table.h
    ${ROOT_DIR}/games/turn_based/material_table.c
    ${ROOT_DIR}/games/turn_based/material_table.h
    ${ROOT_DIR}/games/turn_based/shader_table.c
    ${ROOT_DIR}/games/turn_based/shader_table.h

    ${ROOT_DIR}/games/turn_based/turn_based.c
    ${ROOT_DIR}/games/turn_based/turn_based.h
    ${ROOT_DIR}/games/turn_based/controller.c
    ${ROOT_DIR}/games/turn_based/controller.h
    ${ROOT_DIR}/games/turn_based/scripts.c
    ${ROOT_DIR}/games/turn_based/scripts.h
    ${ROOT_DIR}/games/turn_based/script_tile.c
    )

  # -DTERRAIN_ADDON
  set(GAME_COMPILE_OPTIONS )

