/// \file blAtConnection.h
/// \brief blAtConnection class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2013

#pragma once

#include <vector>

#include "blAssociationTrackingExport.h"

/// \class blAtConnection
/// \brief Class containing a connection for feature point tracking
class BLASSOCIATIONTRACKING_EXPORT blAtConnection
{
public:
    /// \fn blAtConnection(std::vector<float> stateStart, std::vector<float> stateEnd, int startFrameIdx, int endFrameIdx);
    /// \brief Constructor
    /// \param[in] stateStart State of the connection starting point
    /// \param[in] stateEnd State of the connection ending point
    /// \param[in] startFrameIdx Frame index of the starting Point
    /// \param[in] endFrameIdx Frame index of the ending Point
    blAtConnection(std::vector<float> stateStart, std::vector<float> stateEnd, int startFrameIdx, int endFrameIdx);

    /// \fn std::vector<float> stateStart();
    /// \return State of the starting point
    std::vector<float> stateStart();
    /// \fn std::vector<float> stateEnd();
    /// \return State of the ending point
    std::vector<float> stateEnd();
    /// \fn int startFrameIdx();
    /// \return Frame index of the starting Point
    int startFrameIdx();
    /// \fn int endFrameIdx();
    /// \return Frame index of the ending Point
    int endFrameIdx();
    /// \fn float cost();
    /// \return the cost of the connection
    float cost();
    /// \fn void setCost(float cost);
    /// \param[in] cost the cost of the connection
    void setCost(float cost);

private:
    std::vector<float> m_stateStart; ///< state of the connection starting point
    std::vector<float> m_stateEnd; ///< state of the connection ending point
    int m_startFrameIdx; ///< index of the starting frame index
    int m_endFrameIdx; ///< index of the ending frame index
    float m_cost; ///< cost of the connection
};
