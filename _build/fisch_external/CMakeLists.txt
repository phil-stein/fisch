cmake_minimum_required(VERSION 3.20.1)
include(../compile_options.cmake)

set(EXT_NAME    "fisch_external")
project(${EXT_NAME}
      VERSION 0.1
      DESCRIPTION "fisch external dependencies")

set(ROOT_DIR ../..)


# compile externals seperately to disable warnings
add_library(${EXT_NAME}
               ${ROOT_DIR}/_external/GLAD/glad.c
               ${ROOT_DIR}/_external/GLAD/glad.h

               ${ROOT_DIR}/_external/zip/zip.c
               ${ROOT_DIR}/_external/zip/zip.h
               
               ${ROOT_DIR}/_external/stb/stb_ds.h
               ${ROOT_DIR}/_external/stb/stb_image.h
               
               ${ROOT_DIR}/_external/ufbx/ufbx.c
               ${ROOT_DIR}/_external/ufbx/ufbx.h
              )
target_include_directories(${EXT_NAME} PRIVATE ${ROOT_DIR}/_external)
target_compile_options(${EXT_NAME}     PRIVATE -g -w) # -w disables all warnings for externals

