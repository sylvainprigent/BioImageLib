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

project(blCoreTest)

enable_testing()

## #################################################################
## Input
## #################################################################

set(${PROJECT_NAME}_MAIN_SOURCES
  blColorTest.cpp
  blMatrixTest.cpp
  blMatrix3DTest.cpp
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
  )

  add_test(${TName} ${EXECUTABLE_OUTPUT_PATH}/${TName})

  set(TEST ${TESTS} ${EXECUTABLE_OUTPUT_PATH}/${TName})
endforeach()

add_custom_target(testCore COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS ${TESTS})


#add_executable(${PROJECT_NAME}
#  ${${PROJECT_NAME}_MAIN_SOURCES})

#add_executable(${PROJECT_NAME}2
#  ${${PROJECT_NAME}_MAIN_SOURCES})


#target_link_libraries(${PROJECT_NAME}2
#  blCore)

#add_test(${PROJECT_NAME} {PROJECT_NAME})
#add_test(${PROJECT_NAME}2 ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}2)

#SET(TESTS ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME})
#SET(TESTS ${TESTS} ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}2)
#add_custom_target(test COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS ${TESTS})

## #################################################################
## Source file layout in development environments like Visual Studio
## #################################################################

SOURCE_GROUP("Header Files" REGULAR_EXPRESSION .*\\.h\$)
SOURCE_GROUP("Source Files" REGULAR_EXPRESSION .*\\.cpp\$)
