/// \file blRATCostEuclidean.h
/// \brief blRATCostEuclidean class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#pragma once

#include "blRATCostInterface.h"

/// \class blRATCostEuclidean
/// \brief Class allowing to store an object state for RAT tracking
class BLRAT_EXPORT blRATCostEuclidean : public blRATCostInterface
{
public:
    blRATCostEuclidean();
    ~blRATCostEuclidean();

    void initialize(int frameIdx);
    float calculateCost(blRATConnection* connection, blRATTrack* track);
    void setStatesIndexes(std::vector<int> indexes);
    void loadCurentFrameData(unsigned int frameIdx);

private:
    std::vector<int> m_indexes;
};
