############################################################
#
# $Id$
#
# Copyright (c) Khimbio 2014
#
# AUTHOR:
# Sylvain Prigent (sylvain.prigent@inria.fr)
# From Etienne Delclaux (etienne.delclaux@inria.fr)
# 

# Project name
project(blTxtIO)

## #################################################################
## Define files
## #################################################################
SET(HEADERS_FILES 
  blCmdParser.h
  blCsvIO.h
  blProcessParameters.h
  blTxtIO.h
)

SET(TEMPLATES_FILES
)

SET(SRCS_FILES
  blCmdParser.cpp
  blCsvIO.cpp
  blProcessParameters.cpp
  blTxtIO.cpp
 )

## #################################################################
## Lib Bin generation
## #################################################################
add_library(${PROJECT_NAME} ${SRCS_FILES} ${HEADERS_FILES} ${TEMPLATES_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries( ${PROJECT_NAME} 
  blCore
  ${ITK_LIBRARIES})

## #################################################################
## Export header file
## #################################################################

add_compiler_export_flags()

generate_export_header(${PROJECT_NAME}
  EXPORT_FILE_NAME "${PROJECT_NAME}Export.h")

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Export.h" "${CMAKE_BINARY_DIR}"
  COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Export.h" "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export")
