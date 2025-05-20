cmake_minimum_required(VERSION 3.20.1)





set(GAME "template")
message("building GAME: ${GAME}")
# include(../games/template.cmake)
# include(../games/test.cmake)
# include(../games/sandbox.cmake)
# include(../games/turn_based.cmake)
include(../games/puzzle_game.cmake)

# old:
# if(${GAME} STREQUAL "test")
#   set(GAME_SOURCES ${TEST_SOURCES})
#   set(GAME_COMPILE_OPTIONS ${TEST_COMPILE_OPTIONS})
# elseif(${GAME} STREQUAL "template")
#   include(../games/template.cmake)
# elseif(${GAME} STREQUAL "sandbox")
#   set(GAME_SOURCES ${SANDBOX_SOURCES})
#   set(GAME_COMPILE_OPTIONS ${SANDBOX_COMPILE_OPTIONS})
# else()
#   message(FATAL_ERROR "You need to pass a GAME value, f.e. 'sandbox', i.e. -DGAME:STRING=sandbox" )
# endif()

