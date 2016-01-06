/// \file blRATTrack.h
/// \brief blRATTrack class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#pragma once

#include "blRATState.h"

/// \class blAtTrack
/// \brief Class allowing to store a track. a track is a list of blRATStates
class BLRAT_EXPORT blRATTrack
{
public:
    ///
    /// \brief Constructor
    ///
    blRATTrack();
    ///
    /// \brief Destructor
    ///
    ~blRATTrack();
    ///
    /// \brief Construcor
    /// \param initialState first state of the track
    ///
    blRATTrack(blRATState *initialState, int frameIdx);

public:
    ///
    /// \brief add a to the track
    /// \param state State to
    /// \param frameIdx index of the frame where the object is
    ///
    void addState(blRATState *state, int frameIdx);
    ///
    /// \brief get the last state of the track
    /// \return the last state of the track
    ///
    blRATState* getLastState();
    ///
    /// \brief get the state at index idx
    /// \param idx Index of the state to get
    /// \return pointer to the state
    ///
    blRATState* getStateAt(int idx);
    ///
    /// \brief get the state at the frame index frameIdx
    /// \param frameIdx Index of the frame
    /// \return pointer to the state
    ///
    blRATState* getStateAtFrame(int frameIdx);

    unsigned int getTrackSize();
    unsigned int getFrameIdxAt(unsigned int idx);

private:
    std::vector<blRATState*> m_states; ///< list of the states in the track
    std::vector<int> m_framesIdx; ///< list of the frames idxs in the track

};
