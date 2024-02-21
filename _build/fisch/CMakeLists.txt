cmake_minimum_required(VERSION 3.20.1)
include(../compile_options.cmake)

# set(GAME "-" CACHE STRING "required!, sets the game sources to be compiled")
# set(GAME "-" CACHE STRING "" FORCE)
set(GAME "test")

# set(EXT_NAME    "fisch_external")
# project(${EXT_NAME}
#       VERSION 0.1
#       DESCRIPTION "fisch external dependencies")

set(GAME_NAME    "fisch_game_${GAME}")
project(${GAME_NAME}
      VERSION 0.1
      DESCRIPTION "fisch game sources")

set(LIB_NAME    "fisch")
project(${LIB_NAME}
      VERSION 0.1
      DESCRIPTION "3d game engine")
      # LANGUAGES CXX)

set(FISCH_GAME_ROOT_DIR ../..)

set(ROOT_DIR ../..)
include(../fisch.cmake)
include(../games/games.cmake)



# message("building GAME: ${GAME}")
# if(${GAME} STREQUAL "-")
#   # message("You need to pass a GAME value, f.e. 'sandbox', i.e. -DGAME:STRING=sandbox" )
#   message(FATAL_ERROR "You need to pass a GAME value, f.e. 'sandbox', i.e. -DGAME:STRING=sandbox" )
# elseif(${GAME} STREQUAL "test")
#   set(GAME_SOURCES ${TEST_SOURCES})
# elseif(${GAME} STREQUAL "sandbox")
#   set(GAME_SOURCES ${SANDBOX_SOURCES})
# endif()

add_library(${GAME_NAME}
               ${FISCH_GAME_ROOT_DIR}/games/games.h
               ${GAME_SOURCES}
               )
add_library(${LIB_NAME} ${FISCH_SOURCE_FILES})


# target_include_directories(${EXT_NAME} PRIVATE ${FISCH_GAME_ROOT_DIR}/_external)

target_include_directories(${LIB_NAME}        PRIVATE ${CUSTOM_INCLUDE_DIRS})

target_include_directories(${GAME_NAME}        PRIVATE ${CUSTOM_INCLUDE_DIRS})

target_compile_features(${LIB_NAME}         PRIVATE ${CUSTOM_COMPILE_FEATURES})
target_compile_features(${GAME_NAME}        PRIVATE ${CUSTOM_COMPILE_FEATURES})

target_compile_options(${LIB_NAME}          PRIVATE ${CUSTOM_COMPILE_OPTIONS})
target_compile_options(${GAME_NAME}         PRIVATE ${CUSTOM_COMPILE_OPTIONS})
 
# link external files and game with fisch
target_link_libraries(${GAME_NAME} ${LIB_NAME})
target_link_libraries(${LIB_NAME}  ${GAME_NAME})

if(${CMAKE_GENERATOR} STREQUAL "MinGW Makefiles")
  add_subdirectory(${FISCH_GAME_ROOT_DIR}/_build/fisch_external ${FISCH_GAME_ROOT_DIR}/_build/fisch_external)
else() # if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 2016 19")
  add_subdirectory(${FISCH_GAME_ROOT_DIR}/_build/fisch_external ${FISCH_GAME_ROOT_DIR}/_build/fisch_external_vs19)
endif()
target_link_libraries(${LIB_NAME} fisch_external)


# if(${CMAKE_GENERATOR} STREQUAL "MinGW Makefiles")
#   # target_link_libraries(${EDITOR_NAME} ${FISCH_GAME_ROOT_DIR}/_libs/win64/libglfw3.a)
#   target_link_libraries(${LIB_NAME} ${FISCH_GAME_ROOT_DIR}/_libs/win64/libfreetype.a)
#   # message("!!! -> used .a")
# else() # if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 2016 19")
#   # target_link_libraries(${EDITOR_NAME} ${FISCH_GAME_ROOT_DIR}/_libs/win64/glfw3.lib)
#   target_link_libraries(${LIB_NAME} ${FISCH_GAME_ROOT_DIR}/_libs/win64/freetype.lib)
#   # message("!!! -> used .lib")
# endif()



