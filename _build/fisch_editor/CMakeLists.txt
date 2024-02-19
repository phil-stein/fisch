cmake_minimum_required(VERSION 3.20.1)
include(../compile_options.cmake)
include(../fisch.cmake)

# set(GAME "-" CACHE STRING "required!, sets the game sources to be compiled")
# set(GAME "-" CACHE STRING "" FORCE)
set(GAME "test")

set(GAME_EDITOR_NAME  "fisch_editor_game_${GAME}")
project(${GAME_EDITOR_NAME}
      VERSION 0.1
      DESCRIPTION "fisch game sources, editor")

set(LIB_EDITOR_NAME    "fisch_editor")
project(${LIB_EDITOR_NAME}
      VERSION 0.1
      DESCRIPTION "3d game engine, editor")

set(FISCH_EDITOR_ROOT_DIR ../..)

set(ROOT_DIR ../..)
include(../games/games.cmake)

add_library(${GAME_EDITOR_NAME}
               ${FISCH_EDITOR_ROOT_DIR}/games/games.h
               ${GAME_SOURCES}
               )
add_library(${LIB_EDITOR_NAME} ${FISCH_SOURCE_FILES})


target_include_directories(${LIB_EDITOR_NAME} PRIVATE ${CUSTOM_INCLUDE_DIRS})

target_include_directories(${GAME_EDITOR_NAME} PRIVATE ${CUSTOM_INCLUDE_DIRS})

target_compile_features(${LIB_EDITOR_NAME}  PRIVATE ${CUSTOM_COMPILE_FEATURES})
target_compile_features(${GAME_EDITOR_NAME} PRIVATE ${CUSTOM_COMPILE_FEATURES})

target_compile_options(${LIB_EDITOR_NAME}   PRIVATE ${CUSTOM_COMPILE_OPTIONS_EDITOR})
target_compile_options(${GAME_EDITOR_NAME}  PRIVATE ${CUSTOM_COMPILE_OPTIONS_EDITOR})
 
# link external files and game with fisch
target_link_libraries(${GAME_EDITOR_NAME} ${LIB_EDITOR_NAME})
target_link_libraries(${LIB_EDITOR_NAME}  ${GAME_EDITOR_NAME})

# add_subdirectory(${FISCH_EDITOR_ROOT_DIR}/_build/fisch_external ${FISCH_EDITOR_ROOT_DIR}/_build/fisch_external)
if(${CMAKE_GENERATOR} STREQUAL "MinGW Makefiles")
  add_subdirectory(${FISCH_EDITOR_ROOT_DIR}/_build/fisch_external ${FISCH_EDITOR_ROOT_DIR}/_build/fisch_external)
else() # if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 2016 19")
  add_subdirectory(${FISCH_EDITOR_ROOT_DIR}/_build/fisch_external ${FISCH_EDITOR_ROOT_DIR}/_build/fisch_external_vs19)
endif()
target_link_libraries(${LIB_EDITOR_NAME} fisch_external)


# if(${CMAKE_GENERATOR} STREQUAL "MinGW Makefiles")
#   # target_link_libraries(${EDITOR_NAME} ${FISCH_EDITOR_ROOT_DIR}/_libs/win64/libglfw3.a)
#   target_link_libraries(${LIB_NAME} ${FISCH_EDITOR_ROOT_DIR}/_libs/win64/libfreetype.a)
#   # message("!!! -> used .a")
# else() # if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 2016 19")
#   # target_link_libraries(${EDITOR_NAME} ${FISCH_EDITOR_ROOT_DIR}/_libs/win64/glfw3.lib)
#   target_link_libraries(${LIB_NAME} ${FISCH_EDITOR_ROOT_DIR}/_libs/win64/freetype.lib)
#   # message("!!! -> used .lib")
# endif()



