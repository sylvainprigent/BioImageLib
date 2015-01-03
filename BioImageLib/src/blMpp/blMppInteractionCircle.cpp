/// \file blMppInteractionCircle.cpp
/// \brief blMppInteractionCircle class
/// \author Emmanuel Soubies (Emmanuel.Soubies@inria.fr)
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014


#include "blMppInteractionCircle.h"
#include "blMppShapeSuperquadric2D.h"
#include <blCore>

blMppInteractionCircle::blMppInteractionCircle() : blMppInteraction(){
    m_overlapCoef = 0.0;
}

std::string blMppInteractionCircle::name(){
    return "blMppInteractionCircle";
}

// blProcess methods
void blMppInteractionCircle::setOverlapCoef(float value){
    m_overlapCoef = value;
    if (m_overlapCoef < 0 || m_overlapCoef > 1){
        throw blException("blMppInteraction: overlapCoef must be in [0,1]");
    }
}

// virtual methods
bool blMppInteractionCircle::isCompatible(std::string name){
    if (name == "blMppGeneratorShapeSuperquadric2D"){
        return true;
    }
    return false;
}

float blMppInteractionCircle::compute(blMppShape* shape1, blMppShape* shape2){


    blMppShapeSuperquadric2D *shape1sq = dynamic_cast<blMppShapeSuperquadric2D*>(shape1);
    blMppShapeSuperquadric2D *shape2sq = dynamic_cast<blMppShapeSuperquadric2D*>(shape2);

    float Vatt = 0.1;
    float Vrep = 10;

    float overlap = shape1sq->silhouette()->purcentageOverlaping(shape2sq->silhouette());
    overlap = -Vrep*overlap;
    if (overlap != 0){
        return overlap;
    }


    int distCenter = pow(shape1sq->getCenter().x() - shape2sq->getCenter().x(),2);
    distCenter += pow(shape1sq->getCenter().y() - shape2sq->getCenter().y(),2);
    distCenter = sqrt(distCenter);

    float d_min = shape1sq->b() + shape2sq->b();
    float d_max = d_min + 2;

    float val;
    if ( distCenter < d_max  ){
        val = -Vatt/(d_max - d_min) * (distCenter - d_max);

        //val = V/(d_min - d_max)*distCenter - V/(d_min - d_max)*d_max;

        if (val < 0){
            return 0;
        }
        //std::cout << "val = " << val << std::endl;
        return val;
    }
    return 0;
}
