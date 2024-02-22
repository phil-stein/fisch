cmake_minimum_required(VERSION 3.20.1)

set(GAME "-" CACHE STRING "" FORCE)
message("GAME: ${GAME}")

set(GAME_NAME "game")

project(${GAME_NAME}
      VERSION 0.1
      DESCRIPTION "game")
      # LANGUAGES CXX)

set(GAME_ROOT_DIR_STEP "../..")
set(GAME_ROOT_DIR      "${CMAKE_SOURCE_DIR}/${GAME_ROOT_DIR_STEP}")

set(ROOT_DIR ${GAME_ROOT_DIR})
include(../compile_options.cmake)

# puts .exe in _bin, but also dll's
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${GAME_ROOT_DIR}/_bin)
# set the output dir for libraries 
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${GAME_ROOT_DIR}/_libs/win64)   # static lib's
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${GAME_ROOT_DIR}/_libs/win64)   # dll's, should prob. be _bin

add_executable(${GAME_NAME}
              ${GAME_ROOT_DIR}/apps/game/app.c
              ${GAME_ROOT_DIR}/apps/game/app.h
              
              ${GAME_ROOT_DIR}/_build/game/game.rc
           )


target_include_directories(${GAME_NAME} PRIVATE ${CUSTOM_INCLUDE_DIRS})
target_compile_features(${GAME_NAME}    PRIVATE ${CUSTOM_COMPILE_FEATURES})
target_compile_options(${GAME_NAME}     PRIVATE ${CUSTOM_COMPILE_OPTIONS})


# -- link libs --
# # set _build/... folder based on generator, i.e. mingw-gcc or vs19
# if(${CMAKE_GENERATOR} STREQUAL "MinGW Makefiles")
#   add_subdirectory(${GAME_ROOT_DIR}/_build/fisch ${GAME_ROOT_DIR}/_build/fisch)
# else() # if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 2016 19")
#   add_subdirectory(${GAME_ROOT_DIR}/_build/fisch ${GAME_ROOT_DIR}/_build/fisch_vs19)
# endif()
# target_link_libraries(${GAME_NAME} fisch)
if(${CMAKE_GENERATOR} STREQUAL "MinGW Makefiles")
  # add_subdirectory(${GAME_ROOT_DIR}/_build/fisch ${GAME_ROOT_DIR}/_build/fisch)
  if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_subdirectory(${GAME_ROOT_DIR}/_build/fisch ${GAME_ROOT_DIR}/_build/fisch/debug)
    message("debug")
  else()
    add_subdirectory(${GAME_ROOT_DIR}/_build/fisch ${GAME_ROOT_DIR}/_build/fisch/release)
    message("release")
  endif()
else() # if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 2016 19")
  add_subdirectory(${GAME_ROOT_DIR}/_build/fisch ${GAME_ROOT_DIR}/_build/fisch/vs19)
endif()
target_link_libraries(${GAME_NAME} fisch)
# target_compile_definitions(bovengine PRIVATE EDITOR)


# --- trying to link .a library ---
# u .a library for mingw-gcc and .lib for vs19 based on generator, i.e. mingw-gcc or vs19
# message("!!! generator: '${CMAKE_GENERATOR}'")
if(${CMAKE_GENERATOR} STREQUAL "MinGW Makefiles")
  target_link_libraries(${GAME_NAME} ${GAME_ROOT_DIR}/_libs/win64/libglfw3.a)
  target_link_libraries(${GAME_NAME} ${GAME_ROOT_DIR}/_libs/win64/freetype.a)
 
  # message("!!! -> used .a")
else() # if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 2016 19")
  target_link_libraries(${GAME_NAME} ${GAME_ROOT_DIR}/_libs/win64/glfw3.lib)
  target_link_libraries(${GAME_NAME} ${GAME_ROOT_DIR}/_libs/win64/freetype.lib)
  # message("!!! -> used .lib")
endif()

# ---------------------------------

