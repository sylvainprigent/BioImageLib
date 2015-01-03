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
project(blCore)

## #################################################################
## Define files
## #################################################################

SET(SRC_FILES
  blColor.cpp
  blData.cpp
  blImage.cpp
  blImageCopy.cpp
  blImageCreate.cpp
  blImageIO.cpp
  blImageIOInfo.cpp
  blImageList.cpp
  blImageSize.cpp
  blMatrix.cpp
  blMatrix3D.cpp
  blPixel.cpp
  blProcess.cpp
  blProcessObserver.cpp
  blStringOperations.cpp
  blTableNumber.cpp
  blVectorNumber.cpp
  blVectorNumber2D.cpp
  blVectorString.cpp
  blVectorString2D.cpp
)

SET(HEADERS_FILES
  blCast.h
  blColor.h
  blData.h
  blException.h
  blImage.h
  blImageCopy.h
  blImageCreate.h
  blImageIO.h
  blImageIOInfo.h
  blImageList.h
  blImageSize.h
  blImageTypes.h
  blMatrix.h
  blMatrix3D.h
  blPixel.h
  blProcess.h
  blProcessObserver.h
  blStringOperations.h
  blTableNumber.h
  blVectorNumber.h
  blVectorNumber2D.h
  blVectorString.h
  blVectorString2D.h
)
  
## #################################################################
## Library target
## #################################################################

add_library(${PROJECT_NAME} ${SRC_FILES} ${HEADERS_FILES})

target_link_libraries(${PROJECT_NAME} 
  blRand
  ${ITK_LIBRARIES})

add_executable(test main.cpp ${SRC_FILES} ${HEADERS_FILES})
target_link_libraries(test blRand ${ITK_LIBRARIES})

## #################################################################
## SWIG export 
## #################################################################
SET_SOURCE_FILES_PROPERTIES(blCore.i PROPERTIES CPLUSPLUS ON)

SWIG_ADD_MODULE(blCore python blCore.i ${SRC_FILES})
SWIG_LINK_LIBRARIES(blCore ${PYTHON_LIBRARIES} blRand ${ITK_LIBRARIES})
