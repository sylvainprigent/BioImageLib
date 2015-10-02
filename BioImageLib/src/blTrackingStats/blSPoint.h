/// \file blSPoint.h
/// \brief blSPoint class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#pragma once

#include <vector>

#include "blTrackingStatsExport.h"

/// \class blSPoint
/// \brief Class containing statistics about a point a track
class BLTRACKINGSTATS_EXPORT blSPoint
{
public:

    /// \brief blTrackingBasicStats Constructor
    /// \param tracks List of all the tracks
    /// \param x X position of the point in the image
    /// \param y Y position of the point in the image
    /// \param frameIdx Frame index of the point
    blSPoint(int frameIdx, float x, float y);

public:
    // calculation
    float calculateLocalSpeed(blSPoint* pointBefore, float frameRate);
    float calculateDistance(blSPoint* pointBefore);
    float calculateDistanceToRefPoint(float refX, float refY);

    void setDistanceToOthers(float dist);

    // getters
    float distance();
    float localSpeed();
    float distanceToRefPoint();
    float refPointX();
    float refPointY();
    float x();
    float y();
    int frameIdx();
    float distanceToOthers();

private:
    float m_x; ///<  X position of the point in the image
    float m_y; ///<  Y position of the point in the image
    int m_frameIdx;
    float m_distance; ///< distance to the point before in the track
    float m_localSpeed; ///< local speed calculated using the point before in the track
    float m_distanceToRefPoint; ///< distance to a reference point
    float m_distanceToOthers; ///< distance to others objects at the same frame
    float m_refPointX;
    float m_refPointY;
};
