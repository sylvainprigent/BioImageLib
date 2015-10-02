############################################################
#
# $Id$
#
# Copyright (c) INRIA 2013
#
# AUTHOR:
# Etienne Delclaux (etienne.delclaux@inria.fr)
# From Sylvain Prigent (sylvain.prigent@inria.fr)
# 



# Project name
project(at_composite)

## #################################################################
## Define files
## #################################################################
include_directories(${BL_CORE_INCLUDE_DIR})
include_directories(${BL_ASSOCIATION_TRACKING_INCLUDE_DIRS})

SET(SRC_FILES blAtFactories.cpp
              )

SET(HEADERS_FILES blAtFactories.h
                  )


## #################################################################
## Lib generation
## #################################################################
					  
add_executable(AssociationTracking ${SRC_FILES} ${HEADERS_FILES} main_association_tracking.cpp)

target_link_libraries(AssociationTracking
  blAssociationTracking
  blCore
  blTxtIO
  ${ITK_LIBRARIES})

SET(FILE )
add_executable(SortTracks ${SRC_FILES} ${HEADERS_FILES} main_sort_tracks.cpp )
target_link_libraries(SortTracks 
  blCore
  blAssociationTracking
  blTxtIO
  ${ITK_LIBRARIES})
