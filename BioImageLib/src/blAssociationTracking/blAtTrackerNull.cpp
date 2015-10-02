/// \file blAtTrackerNull.cpp
/// \brief blAtTrackerNull class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2013

#include "blAtTrackerNull.h"
#include "blAtCostInterface.h"

using namespace std;

blAtTrackerNull::blAtTrackerNull(blAtCostInterface *costFunction, std::vector<std::string> framesFiles, std::vector<std::vector<std::vector<float> > > objects) : blAtTrackerInterface(costFunction, framesFiles, objects){

}

blAtTrackerNull::blAtTrackerNull() : blAtTrackerInterface(){

}

blAtTrackerNull::~blAtTrackerNull(){

}

// ///////////////////// parameters /////////////////////////////
void blAtTrackerNull::initParametersWithDefaultsValues(){

}

// //////////////////////////////////////////////////////////////
void blAtTrackerNull::run(){

}
