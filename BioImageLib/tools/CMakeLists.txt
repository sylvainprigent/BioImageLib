############################################################
#
# $Id$
#
# Copyright (c) bioImageLib 2014
#
# AUTHOR:
# Sylvain Prigent
# 

## #################################################################
## Add subdirectory
## #################################################################

# Add tools
add_subdirectory( blFiltering )

# Add blMpp
if(bl_BUILD_MPP)
if(bl_HAVE_LEMON)
  add_subdirectory( blMpp )
endif(bl_HAVE_LEMON)
if(bl_HAVE_Qt5)
  add_subdirectory( blMppSilhouetteDrawer )
endif(bl_HAVE_Qt5)
endif()

if(bl_BUILD_AT)
if(bl_HAVE_LEMON)
  add_subdirectory( blAssociationTracking )
endif(bl_HAVE_LEMON)
endif()

if(bl_BUILD_RAT)
if(bl_HAVE_LEMON)
  add_subdirectory( blRAT )
endif(bl_HAVE_LEMON)
endif(bl_BUILD_RAT)

# Add particle filtering
if(bl_BUILD_PARTICLE_FILTERING)
add_subdirectory( blParticleFiltering )
endif(bl_BUILD_PARTICLE_FILTERING)

add_subdirectory( blFishScript )
