/// \file blAtCostEuclidean.h
/// \brief blAtCostEuclidean class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2013

#pragma once

#include "blAtCostInterface.h"
#include "blAssociationTrackingExport.h"

// //////////////////////////////////////////////// //
//             blAtCostEuclidean
// //////////////////////////////////////////////// //
/// \class blAtCostEuclidean
/// \brief Cost function calculating a cost function as the euclidian distance between the objects
class BLASSOCIATIONTRACKING_EXPORT blAtCostEuclidean : public blAtCostInterface{

public:
    /// \fn blAtCostEuclidean();
    /// \brief Constructor
    blAtCostEuclidean();

    /// \fn ~blAtCostEuclidean();
    /// \brief Destructor
    ~blAtCostEuclidean();

public:
    //setters

public:
    // virtuals
    /// \fn void loadDataCurentFrames(int frame1, int frame2);
    ///  \brief reimplement from blAtCostInterface
    void loadDataCurentFrames(int frame1, int frame2);
    /// \fn float calculateCost(blAtConnection* connection);
    ///  \brief reimplement from blAtCostInterface
    float calculateCost(blAtConnection* connection);
    /// \fn bool isProbability();
    ///  \brief reimplement from blAtCostInterface
    bool isProbability();
    /// \fn float calculatePairConnectionCost(blAtConnection *connection1, blAtConnection *connection2);
    ///  \brief reimplement from blAtCostInterface
    float calculatePairConnectionCost(blAtConnection *connection1, blAtConnection *connection2);

private:
    int m_frame1; ///< index of frame 1
    int m_frame2; ///< index of frame 2
};
