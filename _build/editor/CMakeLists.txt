cmake_minimum_required(VERSION 3.20.1)
include(../compile_options.cmake)

set(GAME "-" CACHE STRING "" FORCE)
message("GAME: ${GAME}")

set(EDITOR_NAME "editor")

project(${EDITOR_NAME}
      VERSION 0.1
      DESCRIPTION "3d game engine")
      # LANGUAGES CXX)

set(ROOT_DIR_STEP "../..")
set(ROOT_DIR      "${CMAKE_SOURCE_DIR}/${ROOT_DIR_STEP}")

# puts .exe in _bin, but also dll's
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ROOT_DIR}/_bin)
# set the output dir for libraries 
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ROOT_DIR}/_libs/win64)   # static lib's
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ROOT_DIR}/_libs/win64)   # dll's, should prob. be _bin

add_executable(${EDITOR_NAME}
              ${ROOT_DIR}/apps/editor/app.c
              ${ROOT_DIR}/apps/editor/app.h
              ${ROOT_DIR}/apps/editor/editor_save.c
              ${ROOT_DIR}/apps/editor/editor_save.h
              ${ROOT_DIR}/apps/editor/gizmo.c
              ${ROOT_DIR}/apps/editor/gizmo.h
              ${ROOT_DIR}/apps/editor/operation.c
              ${ROOT_DIR}/apps/editor/operation.h
              ${ROOT_DIR}/apps/editor/stylesheet.h
              ${ROOT_DIR}/apps/editor/terrain_edit.c
              ${ROOT_DIR}/apps/editor/terrain_edit.h

              ${ROOT_DIR}/apps/editor/gui/gui.c
              ${ROOT_DIR}/apps/editor/gui/gui.h
              ${ROOT_DIR}/apps/editor/gui/gui_core_data.c
              ${ROOT_DIR}/apps/editor/gui/gui_properties.c
              ${ROOT_DIR}/apps/editor/gui/gui_template_browser.c
              ${ROOT_DIR}/apps/editor/gui/gui_struct_browser.c
              ${ROOT_DIR}/apps/editor/gui/gui_style.h
              ${ROOT_DIR}/apps/editor/gui/gui_top_bar.c
              ${ROOT_DIR}/apps/editor/gui/gui_operations.c
              # ${ROOT_DIR}/apps/editor/gui/gui_entity_hierarchy.c
              # ${ROOT_DIR}/apps/editor/gui/gui_light_hierarchy.c
           )


target_include_directories(${EDITOR_NAME} PRIVATE ${ROOT_DIR}/src)
target_include_directories(${EDITOR_NAME} PRIVATE ${ROOT_DIR}/apps)
target_include_directories(${EDITOR_NAME} PRIVATE ${ROOT_DIR}/games)
target_include_directories(${EDITOR_NAME} PRIVATE ${ROOT_DIR}/_external)
# target_include_directories(${EDITOR_NAME} PRIVATE ${ROOT_DIR}/_external/FREETYPE)
# target_include_directories(${EDITOR_NAME} PRIVATE ${ROOT_DIR}/_external/freetype2)

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -std=gnu99")
target_compile_features(${EDITOR_NAME} PRIVATE cxx_std_11)
target_compile_options(${EDITOR_NAME} PRIVATE ${CUSTOM_COMPILE_OPTIONS})


# -- link libs --
# set _build/... folder based on generator, i.e. mingw-gcc or vs19
if(${CMAKE_GENERATOR} STREQUAL "MinGW Makefiles")
  add_subdirectory(${ROOT_DIR}/_build/fisch ${ROOT_DIR}/_build/fisch)
else() # if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 2016 19")
  add_subdirectory(${ROOT_DIR}/_build/fisch ${ROOT_DIR}/_build/fisch_vs19)
endif()
target_link_libraries(${EDITOR_NAME} fisch)
# target_compile_definitions(bovengine PRIVATE EDITOR)


# --- trying to link .a library ---
# u .a library for mingw-gcc and .lib for vs19 based on generator, i.e. mingw-gcc or vs19
# message("!!! generator: '${CMAKE_GENERATOR}'")
if(${CMAKE_GENERATOR} STREQUAL "MinGW Makefiles")
  target_link_libraries(${EDITOR_NAME} ${ROOT_DIR}/_libs/win64/libglfw3.a)
  # target_link_libraries(${EDITOR_NAME} ${ROOT_DIR}/_libs/win64/freetype/libfreetype.dll.a)
  
  # target_link_libraries(${EDITOR_NAME} ${ROOT_DIR}/_libs/win64/freetype/libfreetype.a)
  # target_link_libraries(${EDITOR_NAME} ${ROOT_DIR}/_libs/win64/freetype/libbrotlidec.a)
  # target_link_libraries(${EDITOR_NAME} ${ROOT_DIR}/_libs/win64/freetype/libbz2.a)
  # target_link_libraries(${EDITOR_NAME} ${ROOT_DIR}/_libs/win64/freetype/libharfbuzz.a)
  # target_link_libraries(${EDITOR_NAME} ${ROOT_DIR}/_libs/win64/freetype/libpng16.a)
  
  # message("!!! -> used .a")
else() # if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 2016 19")
  target_link_libraries(${EDITOR_NAME} ${ROOT_DIR}/_libs/win64/glfw3.lib)
  # target_link_libraries(${EDITOR_NAME} ${ROOT_DIR}/_libs/win64/freetype.lib)
  # message("!!! -> used .lib")
endif()

# SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -L${CMAKE_SOURCE_DIR}/_libs/win64 -lglfw3")

# # add_library(libglfw STATIC IMPORTED)
# add_library(libglfw SHARED IMPORTED)
# # set_target_properties(libglfw PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/_libs/win64/libglfw.a)
# set_target_properties(libglfw PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/_build/glfw3.dll)
# # set_target_properties(libglfw PROPERTIES INTERFACE_INCLUDE_DIRECTORIES _external/GLFW)
# target_link_libraries(${EDITOR_NAME} libglfw)

# SET(GCC_COVERAGE_LINK_FLAGS "-l${CMAKE_SOURCE_DIR}/_libs/win64/libglfw.a")

# target_compile_options(${EDITOR_NAME} PRIVATE -L${CMAKE_SOURCE_DIR}/_libs/win64 -llibglfw.a) 

# add_library(libglfw STATIC IMPORTED)
# set_target_properties(libglfw PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/_libs/win64/libglfw.a)
# # set_target_properties(libglfw PROPERTIES INTERFACE_INCLUDE_DIRECTORIES _external/GLFW)
# target_link_libraries(${EDITOR_NAME} libglfw)
# install(TARGETS ${EDITOR_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})

# add_subdirectory(_external/GLFW)
# find_package(glfw3 3.3 REQUIRED)
# target_link_libraries(${EDITOR_NAME} glfw)
# find_package(OpenGL REQUIRED)
# target_link_libraries(${EDITOR_NAME} OpenGL::GL)

# ---------------------------------

