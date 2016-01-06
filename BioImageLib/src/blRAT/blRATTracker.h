/// \file blRATTracker.h
/// \brief blRATTracker class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#pragma once

#include "blRATCostManager.h"
#include "blRATFramesLoader.h"
#include <vector>
#include "blCore/blProcess.h"

/// \class blRATTracker
/// \brief Class that implement the recursive association tracking optimize
class BLRAT_EXPORT blRATTracker : public blProcess
{
public:
    /// \fn blRATTracker(blRATCostManager *costManager);
    /// \brief Constructor
    blRATTracker(blRATCostManager *costManager, unsigned int framesNumber, blRATObjectsManager* objectsManager);
    /// \fn ~blRATState();
    /// \brief Destructor
    ~blRATTracker();

    void initialize();
    void run();
    void saveTracksToTxtFile(std::string fileURL);
    std::vector<blRATTrack*> getTracks();

public:
    //parameters
    void setMaxMove(float maxMove);
    void setIntegerCoeff(float integerCoeff);
    void setIterationsVerboseRatio(int iterationsVerboseRatio);

private:
    void addConnectionsToTracks(std::vector<blRATConnection*> connections, unsigned int frameIdx);
    void oneStep(unsigned int frameIdx);

private:
    void findAllPossibleConnections(unsigned int frameIdx);
    void generateNewRandConfiguration(unsigned int nbConnections);
    void cut(unsigned int iter, unsigned int maxIter);
    bool isConflict(blRATConnection* conn1, blRATConnection* conn2);
    bool isPrimConflict(blRATConnection* connection);
    blRATTrack* findAssociatedTrack(blRATConnection* newConnection);

private:
    // classes
    blRATCostManager *m_costManager;
    unsigned int m_framesNumber;
    blRATObjectsManager* m_objectsManager;

private:
    float m_maxMove;
    float m_integerCoeff;
    int m_iterationsVerboseRatio;

private:
    // outputs
    std::vector<blRATTrack*> m_tracks;

private:
    // for mpp one step optimisation
    std::vector<blRATConnection*> m_candidatedConnections;
    std::vector<blRATConnection*> m_configPrim;
    std::vector<blRATConnection*> m_config;
};
