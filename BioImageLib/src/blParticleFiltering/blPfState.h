#pragma once

#include <vector>
#include "blParticleFilteringExport.h"

/// \class blPfState
/// \brief class defining a state used in the particle filtering code "ParticleModel"
/// this class define two function to serialize the state into a vector<float>
class BLPARTICLEFILTERING_EXPORT blPfState{

public:
    /// \fn blPfState(int stateSize);
    /// \brief Constructor
    /// \param[in] stateSize Number of attributs af the state
    blPfState(int stateSize);
    /// \fn blPfState(std::vector<float> stateData);
    /// \brief Constructor
    /// \param[in] stateData State attributs
    blPfState(std::vector<float> stateData);

    /// \fn void setState(std::vector<float> state);
    /// \param[in] state State attributs
    void setState(std::vector<float> state);
    /// \fn std::vector<float> getState();
    /// \return the state attributs
    std::vector<float> getState();
    /// \fn void setStateAt(int i, float stateEntry);
    /// \param[in] i attribut index
    /// \param[in] stateEntry attribut value
    void setStateAt(int i, float stateEntry);
    /// \fn float getStateAt(int i);
    /// \param[in] i attribut index
    /// \return attribut value
    float getStateAt(int i);
    /// \fn int size();
    /// \return the number of attributs af the state
    int size();
    /// \fn blPfState* copy();
    /// \return a copy of the state
    blPfState* copy();

protected:
    std::vector<float> m_state; ///< attributs of the state
};
