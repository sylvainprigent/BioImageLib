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
project(blMppTools)

## #################################################################
## Define files
## #################################################################
include_directories(${BL_MPP_INCLUDE_DIRS})
include_directories(${BL_CORE_INCLUDE_DIR})

SET(SRC_FILES  blMppFactories.cpp
			   )

## #################################################################
## Exe generation
## #################################################################
SET(NAME blMppMain)
SET(FILE ${SRC_FILES} main.cpp )
add_executable( ${NAME} ${FILE} )
target_link_libraries(${NAME} blMpp blTxtIO blCastStacks ${ITK_LIBRARIES} blCore)


## #################################################################
## Exe generation
## #################################################################
SET(NAME blMppCmd)
SET(FILE ${SRC_FILES} cmd.cpp )
add_executable( ${NAME} ${FILE} )
target_link_libraries(${NAME} blMpp blTxtIO blCastStacks ${ITK_LIBRARIES} blCore)
