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
	${LEMON_LIBRARIES})

