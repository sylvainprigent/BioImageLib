/// \file blAtCostInterface.h
/// \brief blAtCostInterface class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2013

#include "blAtCostInterface.h"

using namespace std;

// //////////////////////////////////////// //
//          AssociationTrackerCost
// /////////////////////////////////////// //
blAtCostInterface::blAtCostInterface() : blProcess(){

}


blAtCostInterface::~blAtCostInterface(){

}

float blAtCostInterface::calculatePairConnectionCost(blAtConnection* connection1, blAtConnection* connection2){
    return 0;
}

// -------------------- Getters -----------------------------
void blAtCostInterface::setObjects(vector<vector<vector<float> > > &objects){
    m_objects = objects;
}

void blAtCostInterface::setFramesPath(vector<string> &framesFiles){
    m_framesFiles = framesFiles;
}

void blAtCostInterface::setLParameterFromOptimizer(float L){
    m_L = L;
}
