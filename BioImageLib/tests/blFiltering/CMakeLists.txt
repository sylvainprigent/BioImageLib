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

project(blFilteringTest)

enable_testing()

## #################################################################
## Input
## #################################################################

set(${PROJECT_NAME}_MAIN_SOURCES
  blBilateralFilterTest.cpp
  blBinomialBlurFilterTest.cpp
  blConvolutionFilterTest.cpp
  blCurvatureFlowFilterTest.cpp
  blDiscreteGaussianFilterTest.cpp
#  blFrangiFilter.cpp
  blKernelConvolutionTest.cpp
  blMeanFilterTest.cpp
  blMedianFilterTest.cpp
  blMinMaxCurvatureFlowFilterTest.cpp
  blRecursiveGaussianFilterTest.cpp
  blVectorCurvatureAnisotropicDiffusionFilterTest.cpp
  blVectorGradientAnisotropicDiffusionFilterTest.cpp
  )

## ###################################################################
## Configure file
## ###################################################################

if(EXISTS ${PROJECT_SOURCE_DIR}/../blTestData/orchide.tif)
  set(BLORCHIDEIMAGE ${PROJECT_SOURCE_DIR}/../blTestData/orchide.tif)
endif()


if(EXISTS ${PROJECT_SOURCE_DIR}/../blTestData/orchideBilateral.tif)
  set(BLORCHIDEIMAGEBILATERAL ${PROJECT_SOURCE_DIR}/../blTestData/orchideBilateral.tif)
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/../blTestData/orchideColor.tif)
  set(BLORCHIDECOLORIMAGE ${PROJECT_SOURCE_DIR}/../blTestData/orchideColor.tif)
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/../blTestData/orchideBilateralColor.tif)
  set(BLORCHIDECOLORIMAGEBILATERAL ${PROJECT_SOURCE_DIR}/../blTestData/orchideBilateralColor.tif)
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/../blTestData/orchideBinomialBlur.tif)
  set(BLORCHIDEIMAGEBILATERALB ${PROJECT_SOURCE_DIR}/../blTestData/orchideBinomialBlur.tif)
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/../blTestData/orchideBinomialBlurColor.tif)
  set(BLORCHIDECOLORIMAGEBILATERALB ${PROJECT_SOURCE_DIR}/../blTestData/orchideBinomialBlurColor.tif)
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/../blTestData/orchideConvolution.tif)
  set(BLORCHIDEIMAGECONV ${PROJECT_SOURCE_DIR}/../blTestData/orchideConvolution.tif)
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/../blTestData/orchideCurvatureFlow.tif)
  set(BLORCHIDEIMAGECF ${PROJECT_SOURCE_DIR}/../blTestData/orchideCurvatureFlow.tif)
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/../blTestData/orchideCurvatureFlowColor.tif)
  set(BLORCHIDECOLORIMAGECF ${PROJECT_SOURCE_DIR}/../blTestData/orchideCurvatureFlowColor.tif)
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/../blTestData/orchideDiscreteGaussian.tif)
  set(BLORCHIDEIMAGEDG ${PROJECT_SOURCE_DIR}/../blTestData/orchideDiscreteGaussian.tif)
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/../blTestData/frangi.tif)
  set(BLFRANGIIMAGE ${PROJECT_SOURCE_DIR}/../blTestData/frangi.tif)
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/../blTestData/Sidestream_dark_field_image_Frangi.tif)
  set(BLFRANGIIMAGERES ${PROJECT_SOURCE_DIR}/../blTestData/Sidestream_dark_field_image_Frangi.tif)
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/../blTestData/orchideMean.tif)
  set(BLORCHIDEIMAGEMEAN ${PROJECT_SOURCE_DIR}/../blTestData/orchideMean.tif)
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/../blTestData/orchideMeanColor.tif)
  set(BLORCHIDECOLORIMAGEMEAN ${PROJECT_SOURCE_DIR}/../blTestData/orchideMeanColor.tif)
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/../blTestData/orchideMedian.tif)
  set(BLORCHIDEIMAGEMED ${PROJECT_SOURCE_DIR}/../blTestData/orchideMedian.tif)
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/../blTestData/orchideMedianColor.tif)
  set(BLORCHIDECOLORIMAGEMED ${PROJECT_SOURCE_DIR}/../blTestData/orchideMedianColor.tif)
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/../blTestData/orchideMinMaxCurvatureFlow.tif)
  set(BLORCHIDEIMAGEMMCF ${PROJECT_SOURCE_DIR}/../blTestData/orchideMinMaxCurvatureFlow.tif)
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/../blTestData/orchideMinMaxCurvatureFlowColor.tif)
  set(BLORCHIDECOLORIMAGEMMCF ${PROJECT_SOURCE_DIR}/../blTestData/orchideMinMaxCurvatureFlowColor.tif)
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/../blTestData/orchideRecursiveGaussian.tif)
  set(BLORCHIDEIMAGERGF ${PROJECT_SOURCE_DIR}/../blTestData/orchideRecursiveGaussian.tif)
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/../blTestData/orchideRecursiveGaussianColor.tif)
  set(BLORCHIDECOLORIMAGERGF ${PROJECT_SOURCE_DIR}/../blTestData/orchideRecursiveGaussianColor.tif)
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/../blTestData/orchideVectorCurvatureAnisotropicDiffusionColor.tif)
  set(BLORCHIDEIMAGEVCAD ${PROJECT_SOURCE_DIR}/../blTestData/orchideVectorCurvatureAnisotropicDiffusionColor.tif)
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/../blTestData/orchideVectorGradientAnisotropicDiffusionColor.tif)
  set(BLORCHIDECOLORIMAGEVGAD ${PROJECT_SOURCE_DIR}/../blTestData/orchideVectorGradientAnisotropicDiffusionColor.tif)
endif()


if(EXISTS ${PROJECT_SOURCE_DIR}/blFilteringTestConfig.h.in)
  configure_file(blFilteringTestConfig.h.in ${CMAKE_BINARY_DIR}/blFilteringTestConfig.h)
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
    blFiltering
    blComparisonImage)

  add_test(${TName} ${EXECUTABLE_OUTPUT_PATH}/${TName})

  set(TEST ${TESTS} ${EXECUTABLE_OUTPUT_PATH}/${TName})
endforeach()

add_custom_target(testFiltering COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS ${TESTS})


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
