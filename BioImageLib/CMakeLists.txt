############################################################
#
#
# Copyright (c) BioImageLib 2014
#
# AUTHOR:
# Sylvain Prigent
# 

## #################################################################
## Header
## #################################################################

cmake_minimum_required(VERSION 2.8)

project(bl)

set(${PROJECT_NAME}_VERSION_MAJOR 0 CACHE STRING "bl major version number." FORCE)
set(${PROJECT_NAME}_VERSION_MINOR 0 CACHE STRING "bl minor version number." FORCE)
set(${PROJECT_NAME}_VERSION_BUILD 1 CACHE STRING "bl build version number." FORCE)
set(${PROJECT_NAME}_VERSION
  "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD}")

mark_as_advanced(${PROJECT_NAME}_VERSION_MAJOR)
mark_as_advanced(${PROJECT_NAME}_VERSION_MINOR)
mark_as_advanced(${PROJECT_NAME}_VERSION_BUILD)

## #################################################################
## Config file
## #################################################################

# Cmake config file directory path
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(blBuild)

set(bl_USE_DOXYGEN 0)
set(bl_HAVE_LEMON 0)

include(blDependencies)

## #################################################################
## Define Internal Architecture (for includes)
## #################################################################
option(bl_BUILD_EXAMPLES "Build Examples" OFF)
option(bl_BUILD_TESTS "Build Tests" OFF)

add_subdirectory( src )
if(bl_BUILD_EXAMPLES)
  add_subdirectory( examples )
endif(bl_BUILD_EXAMPLES)
if(bl_BUILD_TESTS)
  add_subdirectory( tests )
endif(bl_BUILD_TESTS)

## #################################################################
## Build Documentation
## #################################################################
IF ( ${bl_USE_DOXYGEN} )
  message(STATUS "Using Doxygen")
  SET( EXCLUDE_FROM_DOC_PATTERNS "*/external/*" )
  SET( DOC_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/doc)
  SET( DOC_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )
  
  configure_file( cmake/Doxyfile.in 
    ${DOC_OUTPUT_DIR}/Doxyfile
    @ONLY
    )
  
  add_custom_target( doc
    ${DOXYGEN_EXECUTABLE} ${DOC_OUTPUT_DIR}/Doxyfile
    COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
ELSE( ${bl_USE_DOXYGEN} )
  message(STATUS "Not Using Doxygen")
ENDIF( ${bl_USE_DOXYGEN} )
