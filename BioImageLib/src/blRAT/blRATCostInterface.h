/// \file blRATCostInterface.h
/// \brief blRATCostInterface class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#pragma once

#include "blRATTrack.h"
#include "blRATConnection.h"
#include "blCore/blImage.h"

/// \class blRATCostInterface
/// \brief Class allowing to store an object state for RAT tracking
class BLRAT_EXPORT blRATCostInterface
{
public:
    blRATCostInterface();
    ~blRATCostInterface();

    virtual void initialize(int frameIdx) = 0;
    virtual float calculateCost(blRATConnection* connection, blRATTrack* track) = 0;
    virtual void setStatesIndexes(std::vector<int> indexes) = 0;
    virtual void loadCurentFrameData(unsigned int frameIdx) = 0;

    int numberOfStatesNeeded();
    bool areStatesIndexesInitialized();
    std::vector<int> statesIndexes();
    std::vector<std::string> statesNames();

private:
    int m_numberOfStatesNeeded;
    bool m_areStatesIndexesInitialized;
    std::vector<int> m_StatesIndexes;
    std::vector<std::string> m_StatesNames;
    std::string m_featureDir;
    std::string m_featureFileExtension;
};
