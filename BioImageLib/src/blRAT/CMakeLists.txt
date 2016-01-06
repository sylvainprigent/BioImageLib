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
project(blRAT)

## #################################################################
## Define files
## #################################################################

SET(SRC_FILES
    blRATTrack.cpp
    blRATState.cpp
    blRATConnection.cpp
    blRATCostInterface.cpp
    blRATCostEuclidean.cpp
    blRATCostManager.cpp
    blRATObjectsManager.cpp
    blRATFramesLoader.cpp
    blRATTracker.cpp
)

SET(HEADERS_FILES
    blRATTrack.h
    blRATState.h
    blRATConnection.h
    blRATCostInterface.h
    blRATCostEuclidean.h
    blRATCostManager.h
    blRATObjectsManager.h
    blRATFramesLoader.h
    blRATTracker.h
)

## #################################################################
## Lib generation
## #################################################################
add_library(${PROJECT_NAME} ${SRC_FILES} ${HEADERS_FILES})

target_link_libraries(${PROJECT_NAME}  
	blCore 
        blGeometry
	${LEMON_LIBRARIES})

## #################################################################
## Export header file
## #################################################################

add_compiler_export_flags()

generate_export_header(${PROJECT_NAME}
  EXPORT_FILE_NAME "${PROJECT_NAME}Export.h")

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Export.h" "${CMAKE_BINARY_DIR}"
  COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Export.h" "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export")

