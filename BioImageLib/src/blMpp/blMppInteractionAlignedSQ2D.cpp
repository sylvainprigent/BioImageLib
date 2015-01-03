/// \file blMppInteractionAlignedSQ2D.cpp
/// \brief blMppInteractionAlignedSQ2D class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppInteractionAlignedSQ2D.h"
#include "blMppShapeSuperquadric2D.h"
#include <blCore>
#include "blMppShape2D.h"

blMppInteractionAlignedSQ2D::blMppInteractionAlignedSQ2D() : blMppInteraction(){
    m_overlapCoef = 0.0;
}

std::string blMppInteractionAlignedSQ2D::name(){
    return "blMppInteractionAlignedSQ2D";
}

// blProcess methods
void blMppInteractionAlignedSQ2D::setOverlapCoef(float value){
    m_overlapCoef = value;
    if (m_overlapCoef < 0 || m_overlapCoef > 1){
        throw blException("blMppInteraction: overlapCoef must be in [0,1]");
    }
}

// virtual methods
bool blMppInteractionAlignedSQ2D::isCompatible(std::string name){
    if (name == "blMppGeneratorShapeSuperquadric2D"){
        return true;
    }
    return false;
}

float blMppInteractionAlignedSQ2D::compute(blMppShape* shape1, blMppShape* shape2){

    if (dynamic_cast<blMppShape2D*>(shape1)->silhouette()->isOverlaping(dynamic_cast<blMppShape2D*>(shape2)->silhouette(), m_overlapCoef)){
        return -919191;
    }
    blMppShapeSuperquadric2D *shape1sq = dynamic_cast<blMppShapeSuperquadric2D*>(shape1);
    blMppShapeSuperquadric2D *shape2sq = dynamic_cast<blMppShapeSuperquadric2D*>(shape2);
    int distCenter = pow(shape1sq->getCenter().x() - shape2sq->getCenter().x(),2);
    distCenter += pow(shape1sq->getCenter().y() - shape2sq->getCenter().y(),2);
    distCenter = sqrt(distCenter);

    float d_min = sqrt(pow(shape1sq->b() + shape2sq->b(),2));
    float d_max = d_min + 2;
    float V = -0.1;
    float val;
    if ( distCenter < d_max  ){
        val = V/(d_min - d_max)*distCenter - V/(d_min - d_max)*d_max;
        if (val > 0){
            return 0;
        }
        //std::cout << "val = " << val << std::endl;
        return -val;
    }
    return 0;
}
