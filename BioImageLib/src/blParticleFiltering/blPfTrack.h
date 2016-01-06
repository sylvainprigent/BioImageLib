#pragma once

#include "blPfState.h"
#include <vector>
#include "blParticleFilteringExport.h"

/// \class blPfTrack
/// \brief Class defining a track
/// basicaly, a track contain a list of states
class BLPARTICLEFILTERING_EXPORT blPfTrack{

public:
    /// \fn blPfTrack();
    /// \brief Constructor
    blPfTrack();
    /// \fn ~blPfTrack();
    /// \brief Destrucor
    ~blPfTrack();

    /// \fn void addState(ParticleFilteringState* state);
    /// \brief add a state in the track (the track size increase then by 1)
    /// \param[in] state State to add
    void addState(blPfState* state);
    /// \fn vector<ParticleFilteringState*> getStates();
    /// \return the list of all the states contained in the track
    std::vector<blPfState*> getStates();
    /// \fn int getTrackSize();
    /// \return the length of the track
    int getTrackSize();
    /// \fn ParticleFilteringState* getStateAt(int i);
    /// \return the track at position i
    blPfState* getStateAt(int i);
    /// \fn void setStateAt(int i, blPfState*);
    ///
    void setStateAt(int i, blPfState* state);
    /// \fn blPfState* getLastState();
    /// \return the last state in the track
    blPfState* getLastState();

protected:
    std::vector<blPfState*> m_states; ///< particle states
};
