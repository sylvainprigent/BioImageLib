############################################################
#
# $Id$
#
# Copyright (c) BioImageLib 2014
#
# AUTHOR:
# Sylvain Prigent
# 

## #################################################################
## Options
## #################################################################
option(bl_BUILD_MPP "Build mpp" false)

## #################################################################
## Add subdirectory
## #################################################################

# Add blCore
add_subdirectory( blMath )
add_subdirectory( blRand )
add_subdirectory( blCore )
add_subdirectory( blComparisonImage )

# default toolboxes
add_subdirectory( blCastStacks )
add_subdirectory( blFiltering )
add_subdirectory( blGeometry )
add_subdirectory( blTxtIO )

# aptional toolboxes
if(bl_BUILD_MPP)
if(bl_HAVE_LEMON)
  add_subdirectory( blMpp )
endif(bl_HAVE_LEMON)
endif()
