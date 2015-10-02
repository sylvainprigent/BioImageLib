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
## ITK Library
## #################################################################
# Look for ITK. 
find_package ( ITK REQUIRED )
include(${ITK_USE_FILE})
include_directories(${ITK_INCLUDE_DIRS})

## #################################################################
## OpenMP
## #################################################################

# Look for OpenMP. Automatically definecs following values : 
# OPENMP_FOUND     : Bool : true if find, else false
# OpenMP_C_FLAGS   : OpenMP flags for C compiler
# OpenMP_CXX_FLAGS : OpenMP flags for CXX compiler
find_package( OpenMP QUIET )
if (OPENMP_FOUND)
    message(STATUS "OpenMP Found")
    SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif(OPENMP_FOUND)

## #################################################################
## SWIG
## #################################################################
#FIND_PACKAGE(SWIG REQUIRED)
#if (SWIG_FOUND)
#  INCLUDE(${SWIG_USE_FILE})
#
#  FIND_PACKAGE(PythonLibs REQUIRED)
#  INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
#  INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
#  SET(CMAKE_SWIG_FLAGS "-Wall")

#  message(STATUS "Using Swig")

#else( SWIG_FOUND )
#  message(STATUS "NOT Using Swig")
#endif( SWIG_FOUND )

## #################################################################
## Doxygen
## #################################################################

find_package(Doxygen QUIET)
if(${DOXYGEN_FOUND})
  set(bl_USE_DOXYGEN 1)
endif(${DOXYGEN_FOUND})

## #################################################################
## LEMON
## #################################################################
find_package (LEMON QUIET)
if ( LEMON_FOUND )
  message(STATUS "Using Lemon")
  include_directories(${LEMON_INCLUDE_DIRS})
  link_directories(${LEMON_LIBRARY_DIR})
  set(bl_HAVE_LEMON 1)
else( LEMON_FOUND )
  message(STATUS "NOT Using Lemon")
endif( LEMON_FOUND )

