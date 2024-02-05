cmake_minimum_required(VERSION 3.20.1)

set(GAME_NAME     "game")


project(${GAME_NAME}
      VERSION 0.1
      DESCRIPTION "3d game engine")
      # LANGUAGES CXX)

set(ROOT_DIR_STEP "../..")
set(ROOT_DIR      "${CMAKE_SOURCE_DIR}/${ROOT_DIR_STEP}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ROOT_DIR}/_bin)

add_executable(${GAME_NAME}
                # ${ROOT_DIR}/apps/game/main.cpp
                ${ROOT_DIR}/apps/game/app.c
                ${ROOT_DIR}/apps/game/app.h
           )
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -std=gnu99")

target_include_directories(${GAME_NAME} PRIVATE ${ROOT_DIR}/src)
target_include_directories(${GAME_NAME} PRIVATE ${ROOT_DIR}/apps)
target_include_directories(${GAME_NAME} PRIVATE ${ROOT_DIR}/games)
target_include_directories(${GAME_NAME} PRIVATE ${ROOT_DIR}/_external)
target_compile_features(   ${GAME_NAME} PRIVATE cxx_std_11)
target_compile_options(    ${GAME_NAME} PRIVATE -Wall -DGLOBAL_DEBUG -DASSETM_NO_ZIP -DDEBUG_TIMER -DDEBUG_OPENGL -DDEBUG_DRAW -DASSET_PATH=\"/Workspace/C/fisch/_assets/\")


# -- link libs --
# add_subdirectory(${ROOT_DIR}/_external ${ROOT_DIR}/_build/external)
# target_link_libraries(${GAME_NAME} bov_externals)
# target_link_libraries(${GAME_NAME} ${ROOT_DIR}/_libs/win64/libbov_externals.a)

# set _build/... folder based on generator, i.e. mingw-gcc or vs19
if(${CMAKE_GENERATOR} STREQUAL "MinGW Makefiles")
  add_subdirectory(${ROOT_DIR}/_build/bovengine ${ROOT_DIR}/_build/bovengine)
else() # if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 2016 19")
  add_subdirectory(${ROOT_DIR}/_build/bovengine ${ROOT_DIR}/_build/bovengine_vs19)
endif()
target_link_libraries(${GAME_NAME} bovengine)

# --- trying to link .a library ---
# message("!!! generator: '${CMAKE_GENERATOR}'")
if(${CMAKE_GENERATOR} STREQUAL "MinGW Makefiles")
  target_link_libraries(${GAME_NAME} ${ROOT_DIR}/_libs/win64/libglfw3.a)
  # message("!!! -> used .a")
else() # if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 2016 19")
  target_link_libraries(${GAME_NAME} ${ROOT_DIR}/_libs/win64/glfw3.lib)
  # message("!!! -> used .lib")
endif()


# SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -L${CMAKE_SOURCE_DIR}/_libs/win64 -lglfw3")

# # add_library(libglfw STATIC IMPORTED)
# add_library(libglfw SHARED IMPORTED)
# # set_target_properties(libglfw PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/_libs/win64/libglfw.a)
# set_target_properties(libglfw PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/_build/glfw3.dll)
# # set_target_properties(libglfw PROPERTIES INTERFACE_INCLUDE_DIRECTORIES _external/GLFW)
# target_link_libraries(${GAME_NAME} libglfw)

# SET(GCC_COVERAGE_LINK_FLAGS "-l${CMAKE_SOURCE_DIR}/_libs/win64/libglfw.a")

# target_compile_options(${GAME_NAME} PRIVATE -L${CMAKE_SOURCE_DIR}/_libs/win64 -llibglfw.a) 

# add_library(libglfw STATIC IMPORTED)
# set_target_properties(libglfw PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/_libs/win64/libglfw.a)
# # set_target_properties(libglfw PROPERTIES INTERFACE_INCLUDE_DIRECTORIES _external/GLFW)
# target_link_libraries(${GAME_NAME} libglfw)
# install(TARGETS ${GAME_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})

# add_subdirectory(_external/GLFW)
# find_package(glfw3 3.3 REQUIRED)
# target_link_libraries(${GAME_NAME} glfw)
# find_package(OpenGL REQUIRED)
# target_link_libraries(${GAME_NAME} OpenGL::GL)

# ---------------------------------

