############################################################
#
# $Id$
#
# Copyright (c) bioImagelib 2014
#
# AUTHOR:
# Sylvain Prigent (sylvain.prigent@inria.fr)
# 

## #################################################################
## Project name
## #################################################################

project(blMath)

## #################################################################
## Define files
## #################################################################

SET(SRCS_FILES
  blDistance.cpp
  blMath.cpp
  blMathLinearAlgebra.cpp
  blMathVector.cpp
  blMathVectorClustering.cpp
)

SET(HEADERS_FILES
  blDistance.h
  blMath.h
  blMathLinearAlgebra.h
  blMathVector.h
  blMathVectorClustering.h
)

SET(TEMPLATE_FILES
	blMathVector.tpp
)
## #################################################################
## Lib generation
## #################################################################

add_library(${PROJECT_NAME} ${SRCS_FILES} ${HEADERS_FILES} ${TEMPLATE_FILES})
target_link_libraries(${PROJECT_NAME} 
	blCore
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
