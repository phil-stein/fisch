cmake_minimum_required(VERSION 3.20.1)

set(GAME "test")

include(../games/test.cmake)
include(../games/sandbox.cmake)



message("building GAME: ${GAME}")
if(${GAME} STREQUAL "-")
  # message("You need to pass a GAME value, f.e. 'sandbox', i.e. -DGAME:STRING=sandbox" )
  message(FATAL_ERROR "You need to pass a GAME value, f.e. 'sandbox', i.e. -DGAME:STRING=sandbox" )
elseif(${GAME} STREQUAL "test")
  set(GAME_SOURCES ${TEST_SOURCES})
  set(GAME_COMPILE_OPTIONS ${TEST_COMPILE_OPTIONS})
elseif(${GAME} STREQUAL "sandbox")
  set(GAME_SOURCES ${SANDBOX_SOURCES})
  set(GAME_COMPILE_OPTIONS ${SANDBOX_COMPILE_OPTIONS})
endif()

