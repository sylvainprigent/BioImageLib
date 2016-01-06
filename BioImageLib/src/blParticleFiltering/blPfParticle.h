#pragma once

#include "blPfState.h"
#include "blParticleFilteringExport.h"

/// \class blPfParticle
/// \brief class defining a Particle used in the particle filtering code.
/// it contains all the states of a particle along the tracking process
class BLPARTICLEFILTERING_EXPORT blPfParticle{

public:
    /// \fn blPfParticle(blPfState* initialState);
    /// \brief Constructor
    /// \param[in] initialState initial state of the particule
    blPfParticle(blPfState* initialState);
    /// \fn blPfParticle(std::vector<blPfState*> &particleData);
    /// \brief Constructor
    /// \param[in] particleData States of the particule
    blPfParticle(std::vector<blPfState*> &particleData);
    /// \fn ~blPfParticle();
    /// \brief Destructor
    ~blPfParticle();

    /// \fn void addState(blPfState* state);
    /// \brief add a state to the particule
    /// \param[in] state State to be added
    void addState(blPfState* state);
    /// \fn blPfState* getStateAt(int i);
    /// \param[in] i index of the state to return
    /// \return the state at index i
    blPfState* getStateAt(int i);
    /// \fn void setStateAt(int i, blPfState* state);
    /// \param[in] i Index of the particule
    /// \param[in] state State to be set
    void setStateAt(int i, blPfState* state);
    /// \fn void setStateAt(int i, int posState, float valstate);
    /// \param[in] i Index of the particule
    /// \param[in] posState Attribut of the state to set
    /// \param[in] valstate Value of the attribut to set
    void setStateAt(int i, int posState, float valstate);

    /// \fn void keepOnlyLastnStates(int n);
    /// \brief set the number of states to be kept
    /// usually only last 2 or 3 states are nedded
    /// \param[in] n Number of states to be kept
    void keepOnlyLastnStates(int n);

    /// \fn void removeInitialState();
    /// \brief Remove the initial state
    void removeInitialState();

    /// \fn void deleteStateAt(int i);
    /// \brief remove the state at i. The state is deleted from the memory
    void deleteStateAt(int i);
    /// \fn int size();
    /// \return the number of states of the particule
    int size();

protected:
    std::vector<blPfState*> m_states; ///< list of states
    std::vector<int> m_stateIdxs; ///< index associated to each state
    int m_n; ///< number of states to be kept in memory
    int m_initialFrameIdx; ///< index of the initial state

protected:
    /// \fn int lastIdx();
    /// \return the last state index
    int lastIdx();
    /// \fn int findPos(int i);
    /// \param[in] i Index of the state to find
    int findPos(int i);

};
