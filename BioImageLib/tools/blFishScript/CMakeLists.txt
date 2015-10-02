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
project(blFishScript)

## #################################################################
## Define files
## #################################################################
include_directories(${BL_CORE_INCLUDE_DIR})

SET(SRC_FILES  
			   )

## #################################################################
## Exe generation
## #################################################################
SET(NAME blFishScript)
SET(FILE ${SRC_FILES} blFishScript.cpp )
add_executable( ${NAME} ${FILE} )
target_link_libraries(${NAME} blTxtIO blCastStacks blTrackingStats blStatistics ${ITK_LIBRARIES} blCore)
