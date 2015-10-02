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
project(blFiltering)

## #################################################################
## Define files
## #################################################################
SET(HEADERS_FILES
  blBilateralFilter.h
  blBinomialBlurFilter.h
  blConvolutionFilter.h
  blCurvatureFlowFilter.h
  blDiscreteGaussianFilter.h
  blFilter.h
#  blFrangiFilter.h
  blKernelConvolution.h
  blMeanFilter.h
  blMedianFilter.h
  blMinMaxCurvatureFlowFilter.h
  blRecursiveGaussianFilter.h
  blVectorCurvatureAnisotropicDiffusionFilter.h
  blVectorGradientAnisotropicDiffusionFilter.h
  )

SET(TEMPLATES_FILES
)

SET(SRCS_FILES
  blBilateralFilter.cpp
  blBinomialBlurFilter.cpp
  blConvolutionFilter.cpp
  blCurvatureFlowFilter.cpp
  blDiscreteGaussianFilter.cpp
  blFilter.cpp
#  blFrangiFilter.cpp
  blKernelConvolution.cpp
  blMeanFilter.cpp
  blMedianFilter.cpp
  blMinMaxCurvatureFlowFilter.cpp
  blRecursiveGaussianFilter.cpp
  blVectorCurvatureAnisotropicDiffusionFilter.cpp
  blVectorGradientAnisotropicDiffusionFilter.cpp

)

## #################################################################
## Lib Bin generation
## #################################################################
add_library(${PROJECT_NAME} ${SRCS_FILES} ${HEADERS_FILES} ${TEMPLATES_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries( ${PROJECT_NAME} 
  blCore
  blCastStacks
  ${ITK_LIBRARIES})

## #################################################################
## Export header file
## #################################################################

add_compiler_export_flags()

generate_export_header(${PROJECT_NAME}
  EXPORT_FILE_NAME "${PROJECT_NAME}Export.h")

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Export.h" "${CMAKE_BINARY_DIR}"
  COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Export.h" "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export")

## #################################################################
## SWIG export
## #################################################################
#SET_SOURCE_FILES_PROPERTIES(blFiltering.i PROPERTIES CPLUSPLUS ON)

#SWIG_ADD_MODULE(blFiltering python ${PROJECT_NAME}.i ${SRCS_FILES})
#SWIG_LINK_LIBRARIES(blFiltering ${PYTHON_LIBRARIES} blCore blCastStacks ${ITK_LIBRARIES})
