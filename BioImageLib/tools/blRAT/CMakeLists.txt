############################################################
#
# $Id$
#
# Copyright (c) BioImageLib 2015
#
# AUTHOR:
# Sylvain Prigent (sylvain.prigent@inria.fr)
# 



# Project name
project(rat_composite)

## #################################################################
## Define files
## #################################################################
SET(SRC_FILES blRATFactories.cpp
              )

SET(HEADERS_FILES blRATFactories.h
                  )


## #################################################################
## Lib generation
## #################################################################
					  
add_executable(RAT ${SRC_FILES} ${HEADERS_FILES} main_rat.cpp)

target_link_libraries(RAT
  blRAT
  blCore
  blTxtIO
  blGeometry
  ${ITK_LIBRARIES})
