############################################################
#
# $Id$
#
# Copyright (c) BimImageLib 2015
#
# AUTHOR:
# Sylvain Prigent
# 

# Project name
project(blMppLibraryGenerator)

## #################################################################
## Define files
## #################################################################
SET(SRC_FILES      
    blClickPixel.cpp
    blClosableWidget.cpp
    blGridClick.cpp
    blMppShapeDrawer.cpp
    blMppShapeGridList.cpp
    blFlowLayout.cpp
    main.cpp
)

## #################################################################
## Lib generation
## #################################################################
## #################################################################
## Exe generation
## #################################################################
SET(NAME blMppLibraryGenerator)
SET(FILE ${SRC_FILES} ${MOC_OUTFILES} main.cpp )
add_executable( ${NAME} ${FILE} )
target_link_libraries(${NAME} blMpp ${ITK_LIBRARIES} Qt5::Widgets blCore blMpp)




