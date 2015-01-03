### CMakeLists.txt --- 
##
######################################################################
## 
### Commentary: 
## 
######################################################################
## 
### Change log:
## 
######################################################################

project(blMppTest)

enable_testing()

## #################################################################
## Input
## #################################################################

set(${PROJECT_NAME}_MAIN_SOURCES
  blMppEllipseDetectionPIBTest.cpp
  blMppCircleMbcrNoOverlapTest.cpp
  blMppEllipseMbcrNoOverlapTest.cpp
  blMppRectangleMbcrNoOverlapTest.cpp
  blMppSuperquadric2DMbcrNoOverlapTest.cpp
  blMppEllipseMbcrNoOverlapTestGrad.cpp
  blMppEllipseMbcrNoOverlapTestGradNorm.cpp
  blMppSphereMbcrNoOverlapTest.cpp
  blMppSuperquadric3DMbcrNoOverlapTest.cpp
  blMppSphereMbcrNoOverlapTestGrad.cpp
  blMppSphereMbcrNoOverlapTestGradNorm.cpp
  blMppEllipseIn3DMbcrNoOverlapTest.cpp
  blMppRectangleIn3DMbcrNoOverlapTest.cpp
  blMppSuperquadric2DIn3DMbcrNoOverlapTest.cpp
  blMppCircleIn3DMbcrNoOverlapTest.cpp
  blMppEllipseIn3DMbcrNoOverlapPartBhattTest.cpp
)

## ###################################################################
## Configure file
## ###################################################################

if(EXISTS ${PROJECT_SOURCE_DIR}/imageTest/sperm0006.tif)
  set(BLMPPTESTIMAGE ${PROJECT_SOURCE_DIR}/imageTest/sperm0006.tif)
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/imageTest/cell.tif)
  set(BLMPPCELLTESTIMAGE ${PROJECT_SOURCE_DIR}/imageTest/cell.tif)
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/imageTest/cell.tif)
  set(BLMPPCELLBMTESTIMAGE ${PROJECT_SOURCE_DIR}/imageTest/cell_bm.tif)
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/imageTest/embryon3D.tif)
  set(BLMPPEMBRYONTESTIMAGE ${PROJECT_SOURCE_DIR}/imageTest/embryon3D.tif)
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/imageTest/embryon3D_bm.tif)
  set(BLMPPEMBRYONBMTESTIMAGE ${PROJECT_SOURCE_DIR}/imageTest/embryon3D_bm.tif)
endif()


if(EXISTS ${PROJECT_SOURCE_DIR}/blTestConfig.h.in)
  configure_file(blTestConfig.h.in ${CMAKE_BINARY_DIR}/blTestConfig.h)
endif()


## #################################################################
## Build rules
## #################################################################

SET(TESTS "")

foreach (test ${${PROJECT_NAME}_MAIN_SOURCES})
  get_filename_component(TName ${test} NAME_WE)

  add_executable(${TName} ${test})
  target_link_libraries(${TName}
    blCore
    blCastStacks
    blMpp)

  add_test(${TName} ${EXECUTABLE_OUTPUT_PATH}/${TName})

  set(TEST ${TESTS} ${EXECUTABLE_OUTPUT_PATH}/${TName})
endforeach()

add_custom_target(testMpp COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS ${TESTS})

## #################################################################
## Source file layout in development environments like Visual Studio
## #################################################################
SOURCE_GROUP("Header Files" REGULAR_EXPRESSION .*\\.h\$)
SOURCE_GROUP("Source Files" REGULAR_EXPRESSION .*\\.cpp\$)
