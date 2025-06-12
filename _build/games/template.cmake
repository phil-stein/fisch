cmake_minimum_required(VERSION 3.20.1)

set(GAME_SOURCES 
    ${ROOT_DIR}/games/template/entity_tags.h

    ${ROOT_DIR}/games/template/entity_table.c
    ${ROOT_DIR}/games/template/entity_table.h
    ${ROOT_DIR}/games/template/material_table.c
    ${ROOT_DIR}/games/template/material_table.h
    ${ROOT_DIR}/games/template/shader_table.c
    
    ${ROOT_DIR}/games/template/template.c
    ${ROOT_DIR}/games/template/template.h
    ${ROOT_DIR}/games/template/scripts.c
    ${ROOT_DIR}/games/template/scripts.h
    )

# -DTERRAIN_ADDON
set(GAME_COMPILE_OPTIONS )

