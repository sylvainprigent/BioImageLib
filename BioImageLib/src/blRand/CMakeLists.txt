############################################################
#
# $Id$
#
# Copyright (c) bioImageLib 2014
#
# AUTHOR:
# Sylvain Prigent
# 

# Project name
project(blRand)

## #################################################################
## Define files
## #################################################################
SET(HEADERS_FILES
  blRandomGeneratorPM.h
  blRandomGeneratorPMS.h
)

SET(TEMPLATES_FILES
)

SET(SRCS_FILES
  blRandomGeneratorPM.cpp
  blRandomGeneratorPMS.cpp
)

## #################################################################
## Lib Bin generation
## #################################################################
add_library(${PROJECT_NAME} ${SRCS_FILES} ${HEADERS_FILES} ${TEMPLATES_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries( ${PROJECT_NAME})
