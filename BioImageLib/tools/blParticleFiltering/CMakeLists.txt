############################################################
#
# $Id$
#
# Copyright (c) BioImageLib 2014
#
# AUTHOR:
# Sylvain Prigent
# 

# Project name
project(blParticleFilteringTools)

## #################################################################
## Define files
## #################################################################

include_directories(${BL_OPTIMIZATION_INCLUDE_DIR})
include_directories(${BL_CORE_INCLUDE_DIR})

include_directories(${BL_PARTICLE_FILTERING_INCLUDE_DIRS})

SET(SRC_FILES blPfModelsFactory.h
              blPfModelsFactory.cpp
)

## #################################################################
## Exe generation
## #################################################################

SET(NAME single_pf)
SET(FILE ${SRC_FILES} main_single_pf.cpp)
add_executable( ${NAME} ${FILE} )
target_link_libraries(${NAME} blParticleFiltering ${ITK_LIBRARIES} blCore blTxtIO)

SET(NAME multiple_particle_filtering)
SET(FILE ${SRC_FILES} main_multiple_pf.cpp)
add_executable( ${NAME} ${FILE} )
target_link_libraries(${NAME} blParticleFiltering ${ITK_LIBRARIES} blCore blTxtIO)

SET(NAME multiple_particle_filtering_with_detection)
SET(FILE ${SRC_FILES} main_multiple_pf_with_detection.cpp)
add_executable( ${NAME} ${FILE} )
target_link_libraries(${NAME} blParticleFiltering ${ITK_LIBRARIES} blCore blTxtIO)

SET(NAME multiple_particle_filtering_multiple_start)
SET(FILE ${SRC_FILES} main_multiple_pf_multiple_start.cpp)
add_executable( ${NAME} ${FILE} )
target_link_libraries(${NAME} blParticleFiltering ${ITK_LIBRARIES} blCore blTxtIO)
