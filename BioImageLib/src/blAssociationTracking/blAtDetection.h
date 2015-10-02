/// \file blAtDetection.h
/// \brief blAtDetection class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2013

#pragma once

#include <vector>
#include "blAssociationTrackingExport.h"

/// \class blAtDetection
/// \brief Class containing a detection for association tracking
class BLASSOCIATIONTRACKING_EXPORT blAtDetection
{
public:
    /// \fn blAtDetection(vector<float> state, int frameIdx, int objectIdx);
    /// \brief Constructor
    /// \param[in] state State of the object
    /// \param[in] frameIdx Frame index of the starting Point
    /// \param[in] objectIdx Index of the object in frame frameIdx
    blAtDetection(std::vector<float> state, int frameIdx, int objectIdx);

    /// \fn vector<float> state();
    /// \return State of the object
    std::vector<float> state();
    /// \fn int frameIdx();
    /// \return Frame index of the object
    int frameIdx();
    /// \fn int objectIdx();
    /// \return Index of the object in frame
    int objectIdx();

private:
    std::vector<float> m_state; ///< state of the connection starting point
    int m_frameIdx; ///< state of the connection ending point
    int m_objectIdx; ///< index of the starting frame index
};
