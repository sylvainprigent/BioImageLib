/// \file blRATCostInterface.cpp
/// \brief blRATCostInterface class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#include "blRATCostInterface.h"

blRATCostInterface::blRATCostInterface(){
    m_numberOfStatesNeeded = 0;
    m_areStatesIndexesInitialized = false;
}

blRATCostInterface::~blRATCostInterface(){

}

int blRATCostInterface::numberOfStatesNeeded(){
    return m_numberOfStatesNeeded;
}

bool blRATCostInterface::areStatesIndexesInitialized(){
    return m_areStatesIndexesInitialized;
}

std::vector<int> blRATCostInterface::statesIndexes(){
    return m_StatesIndexes;
}

std::vector<std::string> blRATCostInterface::statesNames(){
    return m_StatesNames;
}
