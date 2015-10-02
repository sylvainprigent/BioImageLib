############################################################
#
# $Id$
#
# Copyright (c) bioImageLib 2015
#
# AUTHOR:
# Sylvain Prigent
# 

# Project name
project(blFilteringTools)

## #################################################################
## Define files
## #################################################################
include_directories(${BL_CORE_INCLUDE_DIR})

SET(SRC_FILES  
			   )

## #################################################################
## Exe generation
## #################################################################
SET(NAME blMedianFilter)
SET(FILE ${SRC_FILES} blMedianFilter.cpp )
add_executable( ${NAME} ${FILE} )
target_link_libraries(${NAME} blTxtIO blCastStacks blFiltering ${ITK_LIBRARIES} blCore)
