cmake_minimum_required(VERSION 3.20.1)



set(TEST_SOURCES 
    ${ROOT_DIR}/games/test/entity_tags.h

    ${ROOT_DIR}/games/test/entity_table.c
    ${ROOT_DIR}/games/test/entity_table.h
    ${ROOT_DIR}/games/test/material_table.c
    ${ROOT_DIR}/games/test/material_table.h
    ${ROOT_DIR}/games/test/shader_table.c
    
    ${ROOT_DIR}/games/test/test.c
    
    # ${ROOT_DIR}/games/test/test_comp.c
    # ${ROOT_DIR}/games/test/test_comp.h
    ${ROOT_DIR}/games/test/scripts.c
    ${ROOT_DIR}/games/test/scripts.h
    ${ROOT_DIR}/games/test/script_player.c
    ${ROOT_DIR}/games/test/script_fps.c
    ${ROOT_DIR}/games/test/script_enemy.c
    )

