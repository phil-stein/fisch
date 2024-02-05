cmake_minimum_required(VERSION 3.20.1)

set(LIB_NAME "bov_externals")

project(${LIB_NAME}
      VERSION 0.1
      DESCRIPTION "bovengine exeternals")
      # LANGUAGES CXX)

set(ROOT_DIR ../..)

add_library(${LIB_NAME}
              ${ROOT_DIR}/_external/GLAD/glad.c
              ${ROOT_DIR}/_external/GLAD/glad.h

               ${ROOT_DIR}/_external/zip/zip.c
               ${ROOT_DIR}/_external/zip/zip.h
               ${ROOT_DIR}/
               ${ROOT_DIR}/_external/stb/stb_ds.h
               ${ROOT_DIR}/_external/stb/stb_image.h
               ${ROOT_DIR}/
               ${ROOT_DIR}/_external/ufbx/ufbx.c
               ${ROOT_DIR}/_external/ufbx/ufbx.h
               )

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -std=gnu99")

target_include_directories(${LIB_NAME} PRIVATE ${ROOT_DIR}/src)
target_include_directories(${LIB_NAME} PRIVATE ${ROOT_DIR}/apps)
target_include_directories(${LIB_NAME} PRIVATE ${ROOT_DIR}/_external)
target_compile_features(${LIB_NAME} PRIVATE cxx_std_11)
target_compile_options(${LIB_NAME} PRIVATE -Wall)

