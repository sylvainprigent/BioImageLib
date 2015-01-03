############################################################
#
# $Id$
#
# Copyright (c) bioImageLib 2014
#
# AUTHOR:
# Sylvain Prigent (sylvain.prigent@inria.fr)
# 

## #################################################################
## Add subdirectory
## #################################################################

# Add tools

# Add blMpp
if(bl_BUILD_MPP)
if(bl_HAVE_LEMON)
  add_subdirectory( blMpp )
endif(bl_HAVE_LEMON)
if(bl_HAVE_Qt4)
  #add_subdirectory( blMppSilhouetteDrawer )
endif(bl_HAVE_Qt4)
endif()
