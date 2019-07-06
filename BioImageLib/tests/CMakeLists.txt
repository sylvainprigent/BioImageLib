############################################################
#
#
# Copyright (c) bioImageLib 2014
#
# AUTHOR:
# Sylvain Prigent (sylvain.prigent@inria.fr)
# 
## ###############################################################

enable_testing()

#include(CTest)

#configure_file(${CMAKE_SOURCE_DIR}/cmake/blSetTestEnvironment.cmake.in "${CMAKE_BINARY_DIR}/blSetTestEnvironment.cmake" @ONLY)

#set_directory_properties(PROPERTIES TEST_INCLUDE_FILE "${CMAKE_BINARY_DIR}/blSetTestEnvironment.cmake")

add_subdirectory(blCore)
add_subdirectory( blFiltering )
add_subdirectory( blGeometry )
add_subdirectory( blStatistics )

## blMpp test are rand dependent: a rand generator from file values list must
## be created to run test
#if(bl_BUILD_MPP)
#if(bl_HAVE_LEMON)
#  add_subdirectory( blMpp )
#endif(bl_HAVE_LEMON)
#endif()
