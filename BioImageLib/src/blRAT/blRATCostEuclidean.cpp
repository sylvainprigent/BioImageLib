/// \file blRATCostEuclidean.cpp
/// \brief blRATCostEuclidean class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#include "blRATCostEuclidean.h"

blRATCostEuclidean::blRATCostEuclidean() : blRATCostInterface(){

}

blRATCostEuclidean::~blRATCostEuclidean(){

}

void blRATCostEuclidean::initialize(int frameIdx){
    // not used
}

void blRATCostEuclidean::loadCurentFrameData(unsigned int frameIdx){
    // not used
}

float blRATCostEuclidean::calculateCost(blRATConnection* connection, blRATTrack* track){

    float x1 = connection->getStartState()->getStateAt(0);
    float y1 = connection->getStartState()->getStateAt(1);

    float x2 = connection->getEndState()->getStateAt(0);
    float y2 = connection->getEndState()->getStateAt(1);

    return sqrt( pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void blRATCostEuclidean::setStatesIndexes(std::vector<int> indexes){
    m_indexes = indexes;
}

