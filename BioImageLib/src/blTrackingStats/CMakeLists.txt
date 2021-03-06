############################################################
#
# $Id$
#
# Copyright (c) BioImageLib 2015
#
# AUTHOR:
# Sylvain Prigent
# 

# Project name
project(blTrackingStats)

## #################################################################
## Define files
## #################################################################
SET(SRC_FILES
    blSPoint.cpp
    blSTrack.cpp
    blSTracks.cpp
   )

SET(HEADERS_FILES
    blSPoint.h
    blSTrack.h
    blSTracks.h
   )

## #################################################################
## Lib generation
## #################################################################
add_library(${PROJECT_NAME} ${SRC_FILES} ${HEADERS_FILES})

target_link_libraries(${PROJECT_NAME}  
	blCore 
	blStatistics
	)

## #################################################################
## Export header file
## #################################################################

add_compiler_export_flags()

generate_export_header(${PROJECT_NAME}
  EXPORT_FILE_NAME "${PROJECT_NAME}Export.h")

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Export.h" "${CMAKE_BINARY_DIR}"
  COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Export.h" "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export")

