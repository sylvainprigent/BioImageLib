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
if(bl_HAVE_Qt4)
  #add_subdirectory( blMppSilhouetteDrawer )
endif(bl_HAVE_Qt4)
endif()

if(bl_BUILD_AT)
if(bl_HAVE_LEMON)
  add_subdirectory( blAssociationTracking )
endif(bl_HAVE_LEMON)
endif()

add_subdirectory( blFishScript )
