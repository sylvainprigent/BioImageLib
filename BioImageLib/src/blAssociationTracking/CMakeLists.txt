############################################################
#
# $Id$
#
# Copyright (c) INRIA 2013
#
# AUTHOR:
# Etienne Delclaux (etienne.delclaux@inria.fr)
# From Sylvain Prigent (sylvain.prigent@inria.fr)
# 

# Project name
project(blAssociationTracking)

## #################################################################
## Define files
## #################################################################
include_directories(${BL_OPTIMIZATION_INCLUDE_DIR})
include_directories(${BL_CORE_INCLUDE_DIR})
include_directories(${BL_IMAGE_PROCESSING_INCLUDE_DIR})

SET(SRC_FILES
    blAtCostInterface.cpp
    blAtConnection.cpp
    blAtCostEuclidean.cpp
    blAtCostLine.cpp
    blAtCostLineSize.cpp
    blAtDetection.cpp
    blAtTrack.cpp
    blAtTrackerGraphDetections.cpp
    blAtTrackerInterface.cpp
    blAtTrackerNull.cpp
    blAtTrackSorterInterface.cpp
    blAtCostLineEuclidean.cpp
)

SET(HEADERS_FILES
    blAtCostInterface.h
    blAtConnection.h
    blAtCostEuclidean.h
    blAtCostLine.h
    blAtCostLine.h
    blAtDetection.h
    blAtTrack.h
    blAtTrackerGraphDetections.h
    blAtTrackerInterface.h
    blAtTrackerNull.h
    blAtTrackSorterInterface.h
    blAtCostLineEuclidean.h
)

## #################################################################
## Lib generation
## #################################################################
add_library(${PROJECT_NAME} ${SRC_FILES} ${HEADERS_FILES})

target_link_libraries(${PROJECT_NAME}  
	blCore 
        blCastStacks
        blGeometry
        blFiltering
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

