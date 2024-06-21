cmake_minimum_required(VERSION 3.20.1)

set(FISCH_ROOT_DIR "${CMAKE_SOURCE_DIR}/../..")

set(ROOT_DIR ${FISCH_ROOT_DIR})
include(../compile_options.cmake)
include(../fisch.cmake)
include(../games/games.cmake)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# set(GAME "-" CACHE STRING "required!, sets the game sources to be compiled")
# set(GAME "-" CACHE STRING "" FORCE)
set(GAME "test")

# set(EXT_NAME    "fisch_external")
# project(${EXT_NAME}
#       VERSION 0.1
#       DESCRIPTION "fisch external dependencies")

set(HOT_RELOAD )

if(${DEFINE_EDITOR})
  message("DEFINE_EDITOR")
  set(GAME_NAME  "fisch_editor_game")
  project(${GAME_NAME}
        VERSION 0.1
        DESCRIPTION "fisch game sources, editor")
  
  set(LIB_NAME    "fisch_editor")
  project(${LIB_NAME}
        VERSION 0.1
        DESCRIPTION "3d game engine, editor")
else()
  set(GAME_NAME    "fisch_game_${GAME}")
  project(${GAME_NAME}
        VERSION 0.1
        DESCRIPTION "fisch game sources")
  
  set(LIB_NAME    "fisch")
  project(${LIB_NAME}
        VERSION 0.1
        DESCRIPTION "3d game engine")
        # LANGUAGES CXX)
endif()



# message("building GAME: ${GAME}")
# if(${GAME} STREQUAL "-")
#   # message("You need to pass a GAME value, f.e. 'sandbox', i.e. -DGAME:STRING=sandbox" )
#   message(FATAL_ERROR "You need to pass a GAME value, f.e. 'sandbox', i.e. -DGAME:STRING=sandbox" )
# elseif(${GAME} STREQUAL "test")
#   set(GAME_SOURCES ${TEST_SOURCES})
# elseif(${GAME} STREQUAL "sandbox")
#   set(GAME_SOURCES ${SANDBOX_SOURCES})
# endif()

# add_library(${GAME_NAME} 
add_library(${GAME_NAME} 
               ${FISCH_ROOT_DIR}/games/games.h
               ${GAME_SOURCES}
               )
add_library(${LIB_NAME} ${FISCH_SOURCE_FILES})


# target_include_directories(${EXT_NAME} PRIVATE ${FISCH_ROOT_DIR}/_external)

target_include_directories(${LIB_NAME}   PRIVATE ${CUSTOM_INCLUDE_DIRS})
target_include_directories(${GAME_NAME}  PRIVATE ${CUSTOM_INCLUDE_DIRS})

target_compile_features(${LIB_NAME}      PRIVATE ${CUSTOM_COMPILE_FEATURES})
target_compile_features(${GAME_NAME}     PRIVATE ${CUSTOM_COMPILE_FEATURES})

if(${DEFINE_EDITOR})
  target_compile_options(${LIB_NAME}     PRIVATE ${CUSTOM_COMPILE_OPTIONS_EDITOR} ${GAME_COMPILE_OPTIONS})
  target_compile_options(${GAME_NAME}    PRIVATE ${CUSTOM_COMPILE_OPTIONS_EDITOR} ${GAME_COMPILE_OPTIONS})

  target_link_options(${LIB_NAME}        PRIVATE ${CUSTOM_LINK_OPTIONS_EDITOR})
  target_link_options(${GAME_NAME}       PRIVATE ${CUSTOM_LINK_OPTIONS_EDITOR})
else()
  target_compile_options(${LIB_NAME}     PRIVATE ${CUSTOM_COMPILE_OPTIONS_GAME} ${GAME_COMPILE_OPTIONS})
  target_compile_options(${GAME_NAME}    PRIVATE ${CUSTOM_COMPILE_OPTIONS_GAME} ${GAME_COMPILE_OPTIONS})

  target_link_options(${LIB_NAME}        PRIVATE ${CUSTOM_LINK_OPTIONS})
  target_link_options(${GAME_NAME}       PRIVATE ${CUSTOM_LINK_OPTIONS})
endif()
 
# link external files and game with fisch
target_link_libraries(${GAME_NAME} ${LIB_NAME})
target_link_libraries(${LIB_NAME}  ${GAME_NAME})

if(${CMAKE_GENERATOR} STREQUAL "MinGW Makefiles")
  if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_subdirectory(${FISCH_ROOT_DIR}/_build/fisch_external ${FISCH_ROOT_DIR}/_build/fisch_external/debug)
  else()
    add_subdirectory(${FISCH_ROOT_DIR}/_build/fisch_external ${FISCH_ROOT_DIR}/_build/fisch_external/release)
  endif()
else() # if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 2016 19")
  add_subdirectory(${FISCH_ROOT_DIR}/_build/fisch_external ${FISCH_ROOT_DIR}/_build/fisch_external/vs19)
endif()
target_link_libraries(${LIB_NAME} fisch_external)
# target_link_libraries(${DLL_NAME} fisch_external)

# need to link freetype for some reasin, idk
if(${CMAKE_GENERATOR} STREQUAL "MinGW Makefiles")
  # target_link_libraries(${EDITOR_NAME} ${EDITOR_ROOT_DIR}/_libs/win64/libglfw3.a)
  target_link_libraries(${LIB_NAME} ${FISCH_ROOT_DIR}/_libs/win64/freetype.a)
  # message("!!! -> used .a")
else() # if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 2016 19")
  target_link_libraries(${LIB_NAME} ${FISCH_ROOT_DIR}/_libs/win64/freetype.lib)
  # message("!!! -> used .lib")
endif()

# generate_export_header(${DLL_NAME})

# if(${CMAKE_GENERATOR} STREQUAL "MinGW Makefiles")
#   # target_link_libraries(${EDITOR_NAME} ${FISCH_ROOT_DIR}/_libs/win64/libglfw3.a)
#   target_link_libraries(${LIB_NAME} ${FISCH_ROOT_DIR}/_libs/win64/libfreetype.a)
#   # message("!!! -> used .a")
# else() # if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 2016 19")
#   # target_link_libraries(${EDITOR_NAME} ${FISCH_ROOT_DIR}/_libs/win64/glfw3.lib)
#   target_link_libraries(${LIB_NAME} ${FISCH_ROOT_DIR}/_libs/win64/freetype.lib)
#   # message("!!! -> used .lib")
# endif()



