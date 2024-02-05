cmake_minimum_required(VERSION 3.20.1)

set(SANDBOX_SOURCES 
    ${ROOT_DIR}/games/sandbox/entity_tags.h

    ${ROOT_DIR}/games/sandbox/entity_table.c
    ${ROOT_DIR}/games/sandbox/entity_table.h
    ${ROOT_DIR}/games/sandbox/material_table.c
    ${ROOT_DIR}/games/sandbox/material_table.h
    ${ROOT_DIR}/games/sandbox/shader_table.c
    
    ${ROOT_DIR}/games/sandbox/sandbox.c
    
    ${ROOT_DIR}/games/sandbox/test_comp.c
    ${ROOT_DIR}/games/sandbox/test_comp.h
    )

