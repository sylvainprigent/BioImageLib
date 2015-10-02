/// \file blAtCostEuclidean.cpp
/// \brief blAtCostEuclidean class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2013

#include "blAtCostEuclidean.h"
#include "math.h"

// //////////////////////////////////////// //
//          FTP_CostEuclidian
// /////////////////////////////////////// //
blAtCostEuclidean::blAtCostEuclidean() : blAtCostInterface(){

}

blAtCostEuclidean::~blAtCostEuclidean(){

}

// /////////// setters //////////////////////

// //////////////////////////////////////////
bool blAtCostEuclidean::isProbability(){
    return false;
}

void blAtCostEuclidean::loadDataCurentFrames(int frame1, int frame2){
    m_frame1 = frame1;
    m_frame2 = frame2;
}

float blAtCostEuclidean::calculateCost(blAtConnection *connection){

    connection->stateStart().at(0);

    float euclX = connection->stateStart().at(0) - connection->stateEnd().at(0);
    float euclY = connection->stateStart().at(1) - connection->stateEnd().at(1);
    float dist = euclX*euclX + euclY*euclY;

    if (dist < m_L*m_L){
        dist = dist;
    }
    else{
        dist = m_L*m_L;
    }
    return dist;
}

float blAtCostEuclidean::calculatePairConnectionCost(blAtConnection* connection1, blAtConnection* connection2){
    float xStart = connection1->stateStart().at(0);
    float yStart = connection1->stateStart().at(1);

    float xMiddle = connection1->stateEnd().at(0);
    float yMiddle = connection1->stateEnd().at(1);

    float xEnd = connection2->stateStart().at(0);
    float yEnd = connection2->stateStart().at(1);

    float norm1 = sqrt(pow(xStart - xMiddle,2) + pow(yStart-yMiddle, 2));
    float norm2 = sqrt(pow(xEnd - xMiddle,2) + pow(yEnd-yMiddle, 2));

    float angle = acos(( (xStart-xMiddle)*(xEnd-xMiddle) + (yStart-yMiddle)*(yEnd-yMiddle)  ) / (norm1*norm2));
    angle = angle*180.0/3.14159265;
    return 180-angle;
}
