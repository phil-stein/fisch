
# set(_ROOT_DIR_ ..)

# set(CMAKE_C_STANDARD 99)
# set(CMAKE_C_STANDARD_REQUIRED TRUE)

set(DEBUG_FLAGS   "-g -DDebug -Ddebug -DDEBUG -DGLOBAL_DEBUG -DDEBUG_TIMER -DDEBUG_OPENGL -DDEBUG_DRAW -DPHYS_DEBUG")
set(RELEASE_FLAGS "-O3 -DRelease -Drelease -DRELEASE")

set(CMAKE_CXX_FLAGS_DEBUG_INIT   ${DEBUG_FLAGS})
set(CMAKE_CXX_FLAGS_RELEASE_INIT ${RELEASE_FLAGS})

set(CMAKE_C_FLAGS_DEBUG_INIT   ${DEBUG_FLAGS})
set(CMAKE_C_FLAGS_RELEASE_INIT ${RELEASE_FLAGS})

# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
# SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
# SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

# puts .exe in _bin, but also dll's
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ROOT_DIR}/_bin)
# set the output dir for libraries 
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ROOT_DIR}/_libs/win64)   # static lib's
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ROOT_DIR}/_libs/win64)   # dll's, should prob. be _bin
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ROOT_DIR}/_bin)   # dll's

if(${CMAKE_GENERATOR} STREQUAL "MinGW Makefiles")
  # set(CUSTOM_C_VERSION -std=c17)
  set(CUSTOM_C_VERSION )  # setting c version in gcc causes errors
else()
  set(CUSTOM_C_VERSION /std:c17)
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
  # set(CMAKE_BUILD_TYPE Release)
endif()
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  if(${CMAKE_GENERATOR} STREQUAL "MinGW Makefiles")
    set(BUILD_TYPE_FLAGS
      -g  # debug info
      -pg # generate instumentation info for gprof
      -no-pie # needed for -pg, bc. gcc bugging
      )
    set(BUILD_TYPE_LINK_FLAGS
      -g  # debug info
      -pg # generate instumentation info for gprof
      -no-pie # needed for -pg, bc. gcc bugging
      )
  else()
    set(BUILD_TYPE_FLAGS ) 
    set(BUILD_TYPE_LINK_FLAGS )
  endif()
else()
  set(BUILD_TYPE_FLAGS ) 
  set(BUILD_TYPE_LINK_FLAGS )
endif()

set(CUSTOM_LINK_OPTIONS  ${BUILD_TYPE_LINK_FLAGS})
set(CUSTOM_LINK_OPTIONS_EDITOR ${CUSTOM_LINK_OPTIONS})

if(${CMAKE_GENERATOR} STREQUAL "MinGW Makefiles")
set(WARNING_FLAGS   
  -Wall
  -Wextra
  -Wshadow
  -Wfloat-equal
  -Wtype-limits
  -Wconversion
  -Wsign-conversion
  # -pedantic-errors
  -Wpedantic
  -fmax-errors=10
  -Werror
  -Wno-override-init
  -Wno-strict-prototypes
  )
else() # if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 2016 19")
  set(WARNING_FLAGS -Wall) 
endif()

# -std=c99
set(CUSTOM_COMPILE_OPTIONS
  ${CUSTOM_C_VERSION}
  ${BUILD_TYPE_FLAGS}
  ${WARNING_FLAGS}
  -D_CRT_SECURE_NO_WARNINGS
  # -DGLOBAL_DEBUG 
  -DASSETM_NO_ZIP 
  -DASSET_PATH=\"/Workspace/C/fisch/_assets/\"
  )
set(CUSTOM_COMPILE_OPTIONS_EDITOR
  ${CUSTOM_COMPILE_OPTIONS}

  -DEDITOR -DINCLUDE_PLAY_MODE
  # -DGLOBAL_DEBUG -DDEBUG_TIMER -DDEBUG_OPENGL -DDEBUG_DRAW -DPHYS_DEBUG  
  -DPF_PRINT_LOCATION 
  # -DTRACE_PRINT_LOCATION -DTRACE_LOG_PATH=\"trace.log\" -DTRACE_LOG_MAX_LINES=20 
  )

set(CUSTOM_COMPILE_FEATURES
  # cxx_std_11
  # c_std_99
  )

set(CUSTOM_INCLUDE_DIRS
  ${ROOT_DIR}/src 
  ${ROOT_DIR}/apps 
  ${ROOT_DIR}/games 
  ${ROOT_DIR}/_external 
  ${ROOT_DIR}/_external/FREETYPE
  )

