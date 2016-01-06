/// \file blRATCostManager.cpp
/// \brief blRATCostManager class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#include "blRATCostManager.h"

blRATCostManager::blRATCostManager(blRATObjectsManager *objectManager){
    m_objectManager = objectManager;
}

blRATCostManager::~blRATCostManager(){

}

void blRATCostManager::addCost(blRATCostInterface* cost, float weight){
    m_costs.push_back(cost);
    m_costWeights.push_back(weight);
}

void blRATCostManager::initialize(){

    for(int i = 0 ; i < m_costs.size() ; ++i){
        blRATCostInterface *cost = m_costs[i];
        if (!cost->areStatesIndexesInitialized()){
            int numberOfStatesNeeded = cost->numberOfStatesNeeded();
            std::vector<int> idxs; idxs.resize(numberOfStatesNeeded);
            for (unsigned int idx = m_objectManager->stateLength() ; idx < m_objectManager->stateLength()+numberOfStatesNeeded ; ++idx){
                idxs[idx - m_objectManager->stateLength()] = idx;
            }
            cost->setStatesIndexes(idxs);
        }
    }
}

float blRATCostManager::calculateCost(blRATConnection* connection, blRATTrack* track){

    float cost = 1.0;
    for(int i = 0 ; i < m_costs.size() ; ++i){
        cost *= exp(-m_costWeights[i] * m_costs[i]->calculateCost(connection, track));
    }
    return cost;
}
