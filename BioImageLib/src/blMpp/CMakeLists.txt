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
## Project name
## #################################################################

project(blMpp)

## #################################################################
## Define files
## #################################################################

#include_directories(${BL_CORE_INCLUDE_DIR})
#include_directories(${BL_IMAGE_PROCESSING_INCLUDE_DIR})

SET(SRCS_FILES
  blMppAlgorithm.cpp
  blMppAlgorithmMBCAR.cpp
  blMppAlgorithmMBCARCluster.cpp
  blMppAlgorithmMBCR.cpp
  blMppAlgorithmMBCRParallel.cpp

  blMppCluster.cpp
  blMppContainerConfigShape2D.cpp
  blMppContainerSilhouette.cpp
  blMppContainerSilhouette3D.cpp

  blMppDataTerm.cpp
  blMppDataTermBhattacharyya.cpp
  blMppDataTermBhattacharyyaCircleConstraint.cpp
  blMppDataTermGradient.cpp
  blMppDataTermGradientCompt.cpp
  blMppDataTermGradientNormalized.cpp
  blMppDataTermMeanIntensity.cpp
  blMppDataTermPartBhatt.cpp

  blMppGeneratorShape.cpp
  blMppGeneratorShapeCircle.cpp
  blMppGeneratorShapeCircleIn3D.cpp
  blMppGeneratorShapeEllipse.cpp
  blMppGeneratorShapeEllipseIn3D.cpp
  blMppGeneratorShapeRectangle.cpp
  blMppGeneratorShapeRectangleIn3D.cpp
  blMppGeneratorShapeSilhouette2D.cpp
  blMppGeneratorShapeSilhouetteTemplate2D.cpp
  blMppGeneratorShapeSphere.cpp
  blMppGeneratorShapeSuperquadric2D.cpp
  blMppGeneratorShapeSuperquadric2DIn3D.cpp
  blMppGeneratorShapeSuperquadric3D.cpp
  blMppGeneratorShapeTestClusters.cpp

  blMppInteraction.cpp
  blMppInteractionAlignedSQ2D.cpp
  blMppInteractionAttraction.cpp
  blMppInteractionCircle.cpp
  blMppInteractionNoOverlap.cpp

  blMppResultRepresentation.cpp

  blMppShape.cpp
  blMppShape2D.cpp
  blMppShape3D.cpp
  blMppShapeCircle.cpp
  blMppShapeEllipse.cpp
  blMppShapeGraph.cpp
  blMppShapeRectangle.cpp
  blMppShapeSilhouette.cpp
  blMppShapeSphere.cpp
  blMppShapeSuperquadric2D.cpp
  blMppShapeSuperquadric3D.cpp

)

SET(HEADERS_FILES
  blMppAlgorithm.h
  blMppAlgorithmMBCAR.h
  blMppAlgorithmMBCARCluster.h
  blMppAlgorithmMBCR.h
  blMppAlgorithmMBCRParallel.h

  blMppCluster.h
  blMppContainerConfigShape2D.h
  blMppContainerSilhouette.h
  blMppContainerSilhouette3D.h

  blMppDataTerm.h
  blMppDataTermBhattacharyya.h
  blMppDataTermBhattacharyyaCircleConstraint.h
  blMppDataTermGradient.h
  blMppDataTermGradientCompt.h
  blMppDataTermGradientNormalized.h
  blMppDataTermMeanIntensity.h
  blMppDataTermPartBhatt.h

  blMppGeneratorShape.h
  blMppGeneratorShapeCircle.h
  blMppGeneratorShapeCircleIn3D.h
  blMppGeneratorShapeEllipse.h
  blMppGeneratorShapeEllipseIn3D.h
  blMppGeneratorShapeRectangle.h
  blMppGeneratorShapeRectangleIn3D.h
  blMppGeneratorShapeSilhouette2D.h
  blMppGeneratorShapeSilhouetteTemplate2D.h
  blMppGeneratorShapeSphere.h
  blMppGeneratorShapeSuperquadric2D.h
  blMppGeneratorShapeSuperquadric2DIn3D.h
  blMppGeneratorShapeSuperquadric3D.h
  blMppGeneratorShapeTestClusters.h

  blMppInteraction.h
  blMppInteractionAlignedSQ2D.h
  blMppInteractionAttraction.h
  blMppInteractionCircle.h
  blMppInteractionNoOverlap.h

  blMppResultRepresentation.h

  blMppShape.h
  blMppShape2D.h
  blMppShape3D.h
  blMppShapeCircle.h
  blMppShapeEllipse.h
  blMppShapeGraph.h
  blMppShapeRectangle.h
  blMppShapeSilhouette.h
  blMppShapeSphere.h
  blMppShapeSuperquadric2D.h
  blMppShapeSuperquadric3D.h

)

## #################################################################
## Lib generation
## #################################################################

add_library(${PROJECT_NAME} ${SRCS_FILES} ${HEADERS_FILES} )
target_link_libraries(${PROJECT_NAME} 
	blCore 
        blMath
        blGeometry
        blCastStacks
        blRand
	${LEMON_LIBRARIES})
