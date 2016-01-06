/// \file blRATCostManager.h
/// \brief blRATCostManager class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#pragma once

#include "blRATObjectsManager.h"
#include "blRATCostInterface.h"

/// \class blRATCostManager
/// \brief Class use by blRATTracker to calculate a connection cost from multiple blRATModels
class BLRAT_EXPORT blRATCostManager
{
public:
    ///
    /// \brief Constructor
    ///
    blRATCostManager(blRATObjectsManager* objectManager);
    ///
    /// \brief Destructor
    ///
    ~blRATCostManager();

public:
    void addCost(blRATCostInterface* cost, float weight);

public:
    void initialize();
    float calculateCost(blRATConnection* connection, blRATTrack* track);

private:
    std::vector<blRATCostInterface*> m_costs;
    std::vector<float> m_costWeights;

    blRATObjectsManager *m_objectManager;

};
