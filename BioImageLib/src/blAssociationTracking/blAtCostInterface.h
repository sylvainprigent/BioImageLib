/// \file blAtCostInterface.h
/// \brief blAtCostInterface class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2013

#pragma once

#include "blCore/blProcess.h"
#include "blAtConnection.h"
#include "blAssociationTrackingExport.h"

// ///////////////////////////////////////////////// //
//              blAtCostInterface
// //////////////////////////////////////////////// //
/// \class blAtCostInterface
/// \brief Virtual class defining the interface of a cost calculator for association tracking
class BLASSOCIATIONTRACKING_EXPORT blAtCostInterface : public blProcess{

public:
    /// \fn blAtCostInterface(mppParameterManager* paramManager);
    /// \brief Constructor
    blAtCostInterface();

    /// \fn ~blAtCostInterface();
    /// \brief Destructor
    ~blAtCostInterface();

    // --------------------- Virtuals function -----------------
    /// \fn virtual void loadDataCurentFrames(int frame1, int frame2);
    /// \brief Function to load the data needed for the calculation of the cost between two frames
    /// \param[in] frame1 Index of the first frame
    /// \param[in] frame2 Index of the second frame
    virtual void loadDataCurentFrames(int frame1, int frame2) = 0;

    /// \fn virtual float calculateCost(int frame1, int frame2, int objectIdxFrame1, int objectIdxFrame2);
    /// \brief Calculate the cost of a connection
    /// \param[in] connection Connection to evaluate
    virtual float calculateCost(blAtConnection* connection) = 0;

    /// \fn float calculatePairConnectionCost(blAtConnection* connection1, blAtConnection* connection2);
    /// \brief calculate the cost of connecting two consecutive connections
    /// \param[in] connection1 Connection to evaluate
    /// \param[in] connection2 Connection to evaluate
    /// \return the connection cost
    float calculatePairConnectionCost(blAtConnection *connection1, blAtConnection *connection2);

    /// \fn virtual bool isProbability() = 0;
    /// \brief Method used to specify the optimizer is the cost is a probability (in [0 1])
    /// or else, any positive number
    /// \return true if the cost is a probability
    virtual bool isProbability() = 0;

    // -------------------- Setters -----------------------------
    /// \fn void setObjects(std::vector<std::vector<std::vector<float> > > &objects);
    /// \brief Function used to load the objects (called by the optimizer)
    void setObjects(std::vector<std::vector<std::vector<float> > > &objects);

    /// \fn void SetFramesPath(std::vector<std::string> &framesFiles);
    /// \brief Function used to load the frames (called by the optimizer)
    /// \param[in] framesFiles list of the frames names
    void setFramesPath(std::vector<std::string> &framesFiles);

    /// \fn void setLParameterFromOptimizer(float L);
    /// Set the L parameter from the association tracker
    void setLParameterFromOptimizer(float L);


protected:
   std::vector<std::vector<std::vector<float> > >  m_objects; ///< List of objects to be tracked

   float m_L; ///< maximum movement allowed
   std::vector<std::string> m_framesFiles; ///< Frames files (full path)
};
