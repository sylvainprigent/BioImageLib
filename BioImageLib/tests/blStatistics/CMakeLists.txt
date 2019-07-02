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

project(blStatisticsTest)

enable_testing()

## #################################################################
## Input
## #################################################################

set(${PROJECT_NAME}_MAIN_SOURCES
  blWilcoxonTest.cpp
  blStudentTest.cpp
  )

## #################################################################
## Build rules
## #################################################################

SET(TESTS "")

foreach (test ${${PROJECT_NAME}_MAIN_SOURCES})
  get_filename_component(TName ${test} NAME_WE)

  add_executable(${TName} ${test})
  target_link_libraries(${TName}
    blCore
    blTxtIO
    blStatistics
  )

  add_test(${TName} ${EXECUTABLE_OUTPUT_PATH}/${TName})

  set(TEST ${TESTS} ${EXECUTABLE_OUTPUT_PATH}/${TName})
endforeach()

add_custom_target(testStatistics COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS ${TESTS})


if(EXISTS ${PROJECT_SOURCE_DIR}/data/-x.csv)
  set(XDISTRIBUTION ${PROJECT_SOURCE_DIR}/data/-x.csv)
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/data/-y.csv)
  set(YDISTRIBUTION ${PROJECT_SOURCE_DIR}/data/-y.csv)
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/blStatisticsTestConfig.h.in)
  configure_file(blStatisticsTestConfig.h.in ${CMAKE_BINARY_DIR}/blStatisticsTestConfig.h)
endif()

## #################################################################
## Source file layout in development environments like Visual Studio
## #################################################################

SOURCE_GROUP("Header Files" REGULAR_EXPRESSION .*\\.h\$)
SOURCE_GROUP("Source Files" REGULAR_EXPRESSION .*\\.cpp\$)
