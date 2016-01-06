/// \file blRATConnection.h
/// \brief blRATConnection class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#pragma once

#include "blRATState.h"
#include "blRATExport.h"


/// \class blRATConnection
/// \brief Class allowing to store an object state for RAT tracking
class BLRAT_EXPORT blRATConnection
{
public:
    ///
    /// \brief Constructor
    ///
    blRATConnection();
    ///
    /// \brief Destructor
    ///
    ~blRATConnection();

    void setStartState(blRATState* state, unsigned int frameIdx);
    void setEndState(blRATState* state, unsigned int frameIdx);

    blRATState* getStartState();
    blRATState* getEndState();
    unsigned int getStartFrameIdx();
    unsigned int getEndFrameIdx();

    void setEnergy(float energy);
    float energy();

private:
    blRATState* m_startState;
    blRATState* m_endState;
    unsigned int m_startFrameIdx;
    unsigned int m_endFrameIdx;
    float m_energy;
};
