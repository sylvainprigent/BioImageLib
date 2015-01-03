/// \file blMppInteractionAttraction.cpp
/// \brief blMppInteractionAttraction class
/// \author Yellamraju Tarun (yellamraju.tarun@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppInteractionAttraction.h"
#include "blMppShapeSuperquadric2D.h"
#include <blCore>
#include <cstdlib>
#include <cmath>
#include <math.h>
#include <iostream>

blMppInteractionAttraction::blMppInteractionAttraction() : blMppInteraction(){
    m_overlapCoef = 0.0;
}

std::string blMppInteractionAttraction::name(){
    return "blMppInteractionAttraction";
}

// blProcess methods
void blMppInteractionAttraction::setOverlapCoef(float value){
    m_overlapCoef = value;
    if (m_overlapCoef < 0 || m_overlapCoef > 1){
        throw blException("blMppInteraction: overlapCoef must be in [0,1]");
    }
}

// virtual methods
bool blMppInteractionAttraction::isCompatible(std::string){
    return true; // compatible with all the shapes
}

float blMppInteractionAttraction::compute(blMppShape* shape1, blMppShape* shape2){

    blMppShapeSuperquadric2D *shape1sq = dynamic_cast<blMppShapeSuperquadric2D*>(shape1);
    blMppShapeSuperquadric2D *shape2sq = dynamic_cast<blMppShapeSuperquadric2D*>(shape2);

    //Parameters of the Ellipses
    float Cx1 = shape1sq->getCenter().x();
    float Cy1 = shape1sq->getCenter().y();
    float Cx2 = shape2sq->getCenter().x();
    float Cy2 = shape2sq->getCenter().y();
    float a1 = shape1sq->a();
    float b1 = shape1sq->b();
    float a2 = shape2sq->a();
    float b2 = shape2sq->b();
    float theta1 = shape1sq->theta();
    float theta2 = shape2sq->theta();

    //Conversion to radians
    theta1 = (theta1 * 180)/3.14;
    theta2 = (theta2 * 180)/3.14;

    float Z = sqrt(pow(Cx1 - Cx2,2)+pow(Cy1 - Cy2,2));      //Distance between Centers

    //std::cout << " Z = "<< Z << std::endl;

    //Repulsion term based on amount of overlap
    float CommonArea = shape1sq->silhouette()->purcentageOverlaping(shape2sq->silhouette());
    //std::cout<<"Common Area"<<CommonArea<<std::endl;
    //std::cout<<"Point Reached"<<std::endl;
    /*if (CommonArea > m_overlapCoef)
        return -999999;
    else if(CommonArea > 0)
        return -0.01;*/

    if(CommonArea > m_overlapCoef)
        return -1;

    float Angle = fabs(fmod((theta1 - theta2),180));//   abs((theta1 - theta2)%180);   //Orientation difference between the 2 ellipses
    //std::cout<<Angle<<std::endl;

    float D = 30;   //Threshold : Maximum distance over which attraction is considered ; User choice
    D = 10;

    if (Z >= D)
    {
        //std::cout << "object too far" << std::endl;
        return 0;
    }

    float Attraction = 0;   //Energy due to attraction

    //Threshold : Maximum misalignment between centres for considering it to be
    //aligned perfectly like in the case of rice
    float AlignAngle = 15;    //User Choice

    if ((Angle <= AlignAngle) || (Angle >= 180 - AlignAngle))
        if ((Z >= fmin(a1+a2,b1+b2)) && (Z<=D))
            Attraction = exp(-Z/100);
    else
        if((Z >= a1 + fmax(a2*abs(cos(2*3.14*Angle)/360), b2*sin(2*3.14*Angle)/360)))       //EDIT: Changed angles to radians
            Attraction = exp(-Z/100);

    //if (Z >= a1 + a2)
        //Attraction = exp(-Z/100);

    //Attraction = exp(-Z/100);

    //Weighting Parameters : User Choice 20 100 15
    float scale = 1000;
    float K1 =300/scale;       //250
    float K2 = 1000/scale;
    float K3 = 15/scale;
    K3 = 0;

    /*if(CommonArea < 0.25)
        K2 = 100;
    else
        return -100;*/

    //K1 = 1000000/scale;

    //Penalising energy based on orientation
    float AlignmentCompensation = sin((2*3.14*Angle)/360);

    //Final Attraction Energy
    float AttractionEnergy = K1 * Attraction - K3 * AlignmentCompensation - K2 * CommonArea;

    //if(AttractionEnergy < 0 )
      //  std::cout<<"There is repulsion"<<std::endl;

    //std::cout<<"Attraction only : "<<AttractionEnergy<<std::endl;

    return AttractionEnergy;

}

