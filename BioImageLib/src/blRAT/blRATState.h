/// \file blRATState.h
/// \brief blRATState class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#pragma once

#include "blRATExport.h"
#include <vector>

/// \class blRATState
/// \brief Class allowing to store an object state for RAT tracking
class BLRAT_EXPORT blRATState
{
public:
    /// \fn blRATState();
    /// \brief Constructor
    blRATState();
    /// \fn blRATState(std::vector<float> states);
    /// \brief Constructor
    /// \param[in] states List of states
    blRATState(std::vector<float> states);
    /// \fn ~blRATState();
    /// \brief Destructor
    ~blRATState();

    ///
    /// \brief get the states size
    /// \return states size
    ///
    int size();
    ///
    /// \brief set states content
    /// \param states
    ///
    void setStates(std::vector<float> states);
    ///
    /// \brief set state value at index idx
    /// \param idx Index of the state to set
    /// \param value Value to set to the state
    ///
    void setStateAt(int idx, float value);
    ///
    /// \brief get state value at index idx
    /// \param idx Index of the state to get
    /// \return the state value
    ///
    float getStateAt(int idx);
    ///
    /// \brief resize the state content
    /// \param size New size of the state content
    ///
    void resize(int size);

private:
    std::vector<float> m_states; ///< list of the object states
};
