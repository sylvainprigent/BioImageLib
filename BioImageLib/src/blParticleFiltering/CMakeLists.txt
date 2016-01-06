############################################################
#
# $Id$
#
# Copyright (c) BioImageLib 2014
#
# AUTHOR:
# Sylvain Prigent
# 

# Project name
project(blParticleFiltering)

## #################################################################
## Define files
## #################################################################
include_directories(${BL_OPTIMIZATION_INCLUDE_DIR})
include_directories(${BL_CORE_INCLUDE_DIR})
include_directories(${BL_IMAGE_PROCESSING_INCLUDE_DIR})

FILE(GLOB SRC_FILES "*.cpp" "*.h")

## #################################################################
## Lib generation
## #################################################################
add_library(${PROJECT_NAME} ${SRC_FILES} )
target_link_libraries(${PROJECT_NAME} blCore blGeometry blCastStacks)

## #################################################################
## Export header file
## #################################################################

add_compiler_export_flags()

generate_export_header(${PROJECT_NAME}
  EXPORT_FILE_NAME "${PROJECT_NAME}Export.h")

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Export.h" "${CMAKE_BINARY_DIR}"
  COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Export.h" "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export")
