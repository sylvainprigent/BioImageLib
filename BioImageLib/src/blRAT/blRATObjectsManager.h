/// \file blRATObjectsManager.h
/// \brief blRATObjectsManager class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#pragma once

#include "blRATState.h"
#include <vector>
#include <string>

/// \class blRATObjectsManager
/// \brief Class allowing to store an object state for RAT tracking
class BLRAT_EXPORT blRATObjectsManager
{
public:
    /// \fn blRATState();
    /// \brief Constructor
    blRATObjectsManager();
    /// \fn ~blRATState();
    /// \brief Destructor
    ~blRATObjectsManager();

    void loadFramesStates(std::vector<std::string> filesURL);
    void loadFrameStates(std::string fileURL);

    unsigned int stateLength();
    void setStateLength(unsigned int length);
    std::vector<blRATState*> getObjectsAt(unsigned int frameIdx);

private:
    std::vector<std::vector<blRATState*> > m_states; ///< list of the object states
    unsigned int m_stateLength;
};
