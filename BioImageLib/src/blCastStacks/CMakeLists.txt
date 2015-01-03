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
project(blCastStacks)

## #################################################################
## Define files
## #################################################################
SET(HEADERS_FILES
  blCastImage.h
  blCastImageToFloat.h
  blCastImageToInt.h
  blColorImageToGray.h
  blColorImageToLuminance.h
  blProjectionImage.h
  blResizeImage.h
  blComposeColorImage.h
  blExtractComponentColorImage.h

  )

SET(TEMPLATES_FILES   
)

SET(SRCS_FILES
  blCastImage.cpp
  blCastImageToFloat.cpp
  blCastImageToInt.cpp
  blColorImageToGray.cpp
  blColorImageToLuminance.cpp
  blProjectionImage.cpp
  blResizeImage.cpp
  blComposeColorImage.cpp
  blExtractComponentColorImage.cpp
  )

## #################################################################
## Lib Bin generation
## #################################################################
add_library(${PROJECT_NAME} ${SRCS_FILES} ${HEADERS_FILES} ${TEMPLATES_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries( ${PROJECT_NAME} 
  blCore
  blMath
  ${ITK_LIBRARIES})
