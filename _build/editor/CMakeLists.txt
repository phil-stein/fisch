cmake_minimum_required(VERSION 3.20.1)
include(../compile_options.cmake)

set(GAME "-" CACHE STRING "" FORCE)
message("GAME: ${GAME}")

set(EDITOR_NAME "editor")

project(${EDITOR_NAME}
      VERSION 0.1
      DESCRIPTION "3d game engine")
      # LANGUAGES CXX)

set(EDITOR_ROOT_DIR_STEP "../..")
set(EDITOR_ROOT_DIR      "${CMAKE_SOURCE_DIR}/${EDITOR_ROOT_DIR_STEP}")

# puts .exe in _bin, but also dll's
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EDITOR_ROOT_DIR}/_bin)
# set the output dir for libraries 
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${EDITOR_ROOT_DIR}/_libs/win64)   # static lib's
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${EDITOR_ROOT_DIR}/_libs/win64)   # dll's, should prob. be _bin

add_executable(${EDITOR_NAME}
              ${EDITOR_ROOT_DIR}/apps/editor/app.c
              ${EDITOR_ROOT_DIR}/apps/editor/app.h
              ${EDITOR_ROOT_DIR}/apps/editor/editor_save.c
              ${EDITOR_ROOT_DIR}/apps/editor/editor_save.h
              ${EDITOR_ROOT_DIR}/apps/editor/gizmo.c
              ${EDITOR_ROOT_DIR}/apps/editor/gizmo.h
              ${EDITOR_ROOT_DIR}/apps/editor/operation.c
              ${EDITOR_ROOT_DIR}/apps/editor/operation.h
              ${EDITOR_ROOT_DIR}/apps/editor/stylesheet.h
              ${EDITOR_ROOT_DIR}/apps/editor/terrain_edit.c
              ${EDITOR_ROOT_DIR}/apps/editor/terrain_edit.h

              ${EDITOR_ROOT_DIR}/apps/editor/gui/gui.c
              ${EDITOR_ROOT_DIR}/apps/editor/gui/gui.h
              ${EDITOR_ROOT_DIR}/apps/editor/gui/gui_core_data.c
              ${EDITOR_ROOT_DIR}/apps/editor/gui/gui_properties.c
              ${EDITOR_ROOT_DIR}/apps/editor/gui/gui_template_browser.c
              ${EDITOR_ROOT_DIR}/apps/editor/gui/gui_struct_browser.c
              ${EDITOR_ROOT_DIR}/apps/editor/gui/gui_style.h
              ${EDITOR_ROOT_DIR}/apps/editor/gui/gui_top_bar.c
              ${EDITOR_ROOT_DIR}/apps/editor/gui/gui_operations.c
              ${EDITOR_ROOT_DIR}/apps/editor/gui/gui_entity_hierarchy.c
              ${EDITOR_ROOT_DIR}/apps/editor/gui/gui_light_hierarchy.c
              
              ${EDITOR_ROOT_DIR}/_build/editor/editor.rc
           )


target_include_directories(${EDITOR_NAME} PRIVATE ${CUSTOM_INCLUDE_DIRS})
target_compile_features(${EDITOR_NAME}    PRIVATE ${CUSTOM_COMPILE_FEATURES})
target_compile_options(${EDITOR_NAME}     PRIVATE ${CUSTOM_COMPILE_OPTIONS_EDITOR})


# -- link libs --
# set _build/... folder based on generator, i.e. mingw-gcc or vs19
# if(${CMAKE_GENERATOR} STREQUAL "MinGW Makefiles")
#   add_subdirectory(${EDITOR_ROOT_DIR}/_build/fisch ${EDITOR_ROOT_DIR}/_build/fisch)
# else() # if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 2016 19")
#   add_subdirectory(${EDITOR_ROOT_DIR}/_build/fisch ${EDITOR_ROOT_DIR}/_build/fisch_vs19)
# endif()
set(DEFINE_EDITOR ON)
if(${CMAKE_GENERATOR} STREQUAL "MinGW Makefiles")
  if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    # add_subdirectory(${EDITOR_ROOT_DIR}/_build/fisch_editor ${EDITOR_ROOT_DIR}/_build/fisch_editor/debug)
    add_subdirectory(${EDITOR_ROOT_DIR}/_build/fisch ${EDITOR_ROOT_DIR}/_build/fisch_editor/debug)
    message("debug")
  else()
    add_subdirectory(${EDITOR_ROOT_DIR}/_build/fisch ${EDITOR_ROOT_DIR}/_build/fisch_editor/release)
    message("release")
  endif()
else() # if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 2016 19")
  add_subdirectory(${EDITOR_ROOT_DIR}/_build/fisch ${EDITOR_ROOT_DIR}/_build/fisch_editor/vs19)
endif()
target_link_libraries(${EDITOR_NAME} fisch_editor)

# --- trying to link .a library ---
# u .a library for mingw-gcc and .lib for vs19 based on generator, i.e. mingw-gcc or vs19
# message("!!! generator: '${CMAKE_GENERATOR}'")
if(${CMAKE_GENERATOR} STREQUAL "MinGW Makefiles")
  target_link_libraries(${EDITOR_NAME} ${EDITOR_ROOT_DIR}/_libs/win64/libglfw3.a)
  target_link_libraries(${EDITOR_NAME} ${EDITOR_ROOT_DIR}/_libs/win64/freetype.a)
  # message("!!! -> used .a")
else() # if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 2016 19")
  target_link_libraries(${EDITOR_NAME} ${EDITOR_ROOT_DIR}/_libs/win64/glfw3.lib)
  target_link_libraries(${EDITOR_NAME} ${EDITOR_ROOT_DIR}/_libs/win64/freetype.lib)
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

