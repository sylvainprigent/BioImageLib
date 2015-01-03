/// \file blMppInteractionNoOverlap.cpp
/// \brief blMppInteractionNoOverlap class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppInteractionNoOverlap.h"
#include <blCore>
#include "blMppShape2D.h"
#include "blMppShape3D.h"
#include "blMppShapeCircle.h"
#include "blMppShapeSphere.h"

blMppInteractionNoOverlap::blMppInteractionNoOverlap() : blMppInteraction(){
    m_overlapCoef = 0.0;
    m_minZDistance = 1;
    m_generatorType = 0;
}

std::string blMppInteractionNoOverlap::name(){
    return "blMppInteractionNoOverlap";
}

// blProcess methods
void blMppInteractionNoOverlap::setOverlapCoef(float value){
    //std::cout << "set overlap coef = " << value << std::endl;
    m_overlapCoef = value;
    if (m_overlapCoef < 0 || m_overlapCoef > 1){
        throw blException("blMppInteraction: overlapCoef must be in [0,1]");
    }
}

void blMppInteractionNoOverlap::setMinZDistance(int value){
    m_minZDistance = value;
}

// virtual methods
void blMppInteractionNoOverlap::initialize(){
    if (m_generatorName == "blMppGeneratorShapeCircle"){
        m_generatorType = 2;
    }
    else if (m_generatorName == "blMppGeneratorShapeCircleIn3D"){
        m_generatorType = 3;
    }
    else if (m_generatorName == "blMppGeneratorShapeSphere"){
        m_generatorType = 4;
    }
    else if (m_generatorName == "blMppGeneratorShapeEllipseIn3D"
             || m_generatorName == "blMppGeneratorShapeRectangleIn3D" ){
        m_generatorType = 5;
    }
    else{
        if (m_isGenerator3D){
            m_generatorType = 1;
        }
        else{
            m_generatorType = 0;
        }
    }
    //std::cout << "m_generatorType = " << m_generatorType << std::endl;
}

bool blMppInteractionNoOverlap::isCompatible(std::string){
    return true; // compatible with all the shapes
}

float blMppInteractionNoOverlap::compute(blMppShape* shape1, blMppShape* shape2){

    if (m_generatorType == 0){
        if (dynamic_cast<blMppShape2D*>(shape1)->silhouette()->isOverlaping(dynamic_cast<blMppShape2D*>(shape2)->silhouette(), m_overlapCoef)){
            return -10;
        }
        return 0;
    }
    else if ( m_generatorType == 1){
        //std::cout << "no overlap 3D case" << std::endl;
        if (dynamic_cast<blMppShape3D*>(shape1)->silhouette()->isOverlaping(dynamic_cast<blMppShape3D*>(shape2)->silhouette(), m_overlapCoef)){
            return -10;
        }
        return 0;
    }
    else if (m_generatorType == 2){
        return computeCircleOverlap(shape1, shape2);
    }
    else if (m_generatorType == 3){
        if (abs(shape1->getCenter().z() - shape2->getCenter().z()) >= m_minZDistance){
            return 0;
        }
        else{
            return computeCircleOverlap(shape1, shape2);
        }
    }
    else if (m_generatorType == 4){

        return computeSphereOverlap(shape1, shape2);
        //std::cout << "m_overlapCoef = " << m_overlapCoef << std::endl;
        /*
        if (dynamic_cast<blMppShape3D*>(shape1)->silhouette()->isOverlaping(dynamic_cast<blMppShape3D*>(shape2)->silhouette(), m_overlapCoef)){
            return -10;
        }
        return 0;
        */
    }
    else if (m_generatorType == 5){
        if (abs(shape1->getCenter().z() - shape2->getCenter().z()) >= m_minZDistance){
            return 0;
        }
        else{
            if (dynamic_cast<blMppShape2D*>(shape1)->silhouette()->isOverlaping(dynamic_cast<blMppShape2D*>(shape2)->silhouette(), m_overlapCoef)){
                return -10;
            }
            return 0;
        }
    }
}


float blMppInteractionNoOverlap::computeCircleOverlap(blMppShape* shape1, blMppShape* shape2){
    // compute distance between centers
    float dist = pow(shape1->getCenter().x() - shape2->getCenter().x(),2 )
                 + pow(shape1->getCenter().y() - shape2->getCenter().y(),2 );

    // compute the radius sum
    float refDist = dynamic_cast<blMppShapeCircle*>(shape1)->radius()
                  + dynamic_cast<blMppShapeCircle*>(shape2)->radius();

    // compute interaction
    if (dist >= (1-m_overlapCoef)*refDist*refDist ){
        return 0;
    }
    else{
        return -10;
    }
}

float blMppInteractionNoOverlap::computeSphereOverlap(blMppShape* shape1, blMppShape* shape2){
    // compute distance between centers
    float dist = pow(shape1->getCenter().x() - shape2->getCenter().x(),2 )
                 + pow(shape1->getCenter().y() - shape2->getCenter().y(),2 )
                 + pow(shape1->getCenter().z() - shape2->getCenter().z(),2 );

    // compute the radius sum
    float refDist = dynamic_cast<blMppShapeSphere*>(shape1)->radius()
                  + dynamic_cast<blMppShapeSphere*>(shape2)->radius();

    // compute interaction
    if (dist >= (1-m_overlapCoef)*refDist*refDist ){
        return 0;
    }
    else{
        return -10;
    }
}
