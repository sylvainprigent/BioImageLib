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
## Exe generation
## #################################################################
add_executable( blMedianFilter blMedianFilter.cpp )
target_link_libraries(blMedianFilter blTxtIO blCastStacks blFiltering ${ITK_LIBRARIES} blCore)

add_executable( blMeanFilter blMeanFilter.cpp )
target_link_libraries(blMeanFilter blTxtIO blCastStacks blFiltering ${ITK_LIBRARIES} blCore)

add_executable( blGaussianFilter blGaussianFilter.cpp )
target_link_libraries(blGaussianFilter blTxtIO blCastStacks blFiltering ${ITK_LIBRARIES} blCore)
