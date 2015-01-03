############################################################
#
# $Id$
#
# Copyright (c) bioImageLib 2014
#
# AUTHOR:
# Sylvain Prigent
# 

## #################################################################
## Export
## #################################################################

include (GenerateExportHeader)

## #################################################################
## Options
## #################################################################

option(BUILD_SHARED_LIBS   "Build shared libraries"  ON)
option(BUILD_DOCUMENTATION "Use Doxygen to create the HTML based API documentation" OFF)

## #################################################################
## 
## #################################################################

set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Build type" FORCE)

set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

## #################################################################
## Configure path
## #################################################################

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_BINARY_DIR})

set(${PROJECT_NAME}_ARCHIVE_OUTPUT_DIRECTORY lib)
set(${PROJECT_NAME}_RUNTIME_OUTPUT_DIRECTORY bin)
set(${PROJECT_NAME}_LIBRARY_OUTPUT_DIRECTORY lib)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${${PROJECT_NAME}_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${${PROJECT_NAME}_ARCHIVE_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${${PROJECT_NAME}_RUNTIME_OUTPUT_DIRECTORY})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/${${PROJECT_NAME}_RUNTIME_OUTPUT_DIRECTORY})

set(LIBRARY_INSTALL_OUTPUT_PATH ${CMAKE_INSTALL_PREFIX}/${${PROJECT_NAME}_LIBRARY_OUTPUT_DIRECTORY})
set(ARCHIVE_INSTALL_OUTPUT_PATH ${CMAKE_INSTALL_PREFIX}/${${PROJECT_NAME}_ARCHIVE_OUTPUT_DIRECTORY})
set(RUNTIME_INSTALL_OUTPUT_PATH ${CMAKE_INSTALL_PREFIX}/${${PROJECT_NAME}_RUNTIME_OUTPUT_DIRECTORY})
set(EXECUTABLE_INSTALL_OUTPUT_PATH ${CMAKE_INSTALL_PREFIX}/${${PROJECT_NAME}_RUNTIME_OUTPUT_DIRECTORY})

set(${PROJECT_NAME}_INCLUDE_DIR ${${PROJECT_NAME}_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}) 
set(${PROJECT_NAME}_LIBRARY_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(${PROJECT_NAME}_RUNTIME_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(${PROJECT_NAME}_CMAKE_DIR ${PROJECT_SOURCE_DIR}/cmake)
set(${PROJECT_NAME}_USE_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Use.cmake)

## #################################################################
##                    Config file
## #################################################################

## Setup Config File
if(EXISTS ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in)
  configure_file( ## Build tree configure file
    ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    @ONLY IMMEDIATE)
endif()

## Setup use file
if(EXISTS ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Use.cmake.in)
configure_file( ## Common use file
  ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Use.cmake.in
  ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Use.cmake
  @ONLY IMMEDIATE)
endif()



## #################################################################
## Build option
## #################################################################
# Compilation Option
#SET(CPPFLAGS "-D_LINUX_ -D_ANSI_ -UNO_PROTO")
#SET(CFLAGS "-ansi -Wall -fsigned-char -fsigned-bitfields -O")
#
#if(CMAKE_COMPILER_IS_GNUC)
#  add_definitions(${CFLAGS})
#endif(CMAKE_COMPILER_IS_GNUC)
#
#if(CMAKE_COMPILER_IS_GNUCXX)
#  add_definitions(${CPPFLAGS} )
#endif(CMAKE_COMPILER_IS_GNUCXX)
