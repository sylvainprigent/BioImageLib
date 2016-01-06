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
