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
option(bl_BUILD_RAT "Build blRAT" true)
option(bl_BUILD_AT "Build association tracking" false)
option(bl_BUILD_PARTICLE_FILTERING "Build particle filtering" true)
option(bl_BUILD_RAT "Build recursive association tracking" false)

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
add_subdirectory( blStatistics )
add_subdirectory( blTrackingStats )

# optional toolboxes
message(STATUS "check use blMpp")
if(bl_BUILD_MPP)
if(bl_HAVE_LEMON)
  add_subdirectory( blMpp )
  message(STATUS "use mpp")
endif(bl_HAVE_LEMON)
endif(bl_BUILD_MPP)

message(STATUS "check use blRAT")
if(bl_BUILD_RAT)
if(bl_HAVE_LEMON)
  add_subdirectory( blRAT )
  message(STATUS "use blRAT")
endif(bl_HAVE_LEMON)
endif(bl_BUILD_RAT)

message(STATUS "check use association tracking")
if(bl_BUILD_AT)
if(bl_HAVE_LEMON)
  add_subdirectory( blAssociationTracking )
  message(STATUS "use association tracking")
endif(bl_HAVE_LEMON)
endif(bl_BUILD_AT)


# Add particle filtering
if(bl_BUILD_PARTICLE_FILTERING)
add_subdirectory( blParticleFiltering )
endif()

