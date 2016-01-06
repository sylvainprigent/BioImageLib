/// \file blSTrack.h
/// \brief blSTrack class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#pragma once

#include "blSPoint.h"

#include "blTrackingStatsExport.h"

/// \class blSTrack
/// \brief Class containing a track as a point lists, and methods to calculate statistics on the track
class BLTRACKINGSTATS_EXPORT blSTrack
{
public:

    /// \brief blSTrack Constructor
    blSTrack();

    blSTrack(std::vector<blSPoint*> points);

public:
    // setters
    void addPoint(blSPoint* point);
    void setPoints(std::vector<blSPoint*> points);
    int size();
    blSPoint* pointAt(int i);

    // calculation
    /// \brief calculate the total distance of the track
    /// \return the calculated distance
    float calculateTotalDistance();

    /// \brief calculate the total distance of in a segment of the track
    /// \param[in] startFrame Index of the frame where the calculation starts
    /// \param[in] endFrame Index of the frame where the calculation ends
    /// \return the calculated distance
    float calculateTotalDistance(int startFrame, int endFrame);

    /// \brief calculate the distance between the starting point and the ending point of the track
    /// \return the calculated distance
    float calculateStartEndDistance();

    /// \brief calculate the distance between the starting point and the ending point of the track
    /// \param[in] startFrame Index of the frame where the calculation starts
    /// \param[in] endFrame Index of the frame where the calculation ends
    /// \return the calculated distance
    float calculateStartEndDistance(int startFrame, int endFrame);

    /// \brief calculate the average speed along the track
    /// \param[in] frameRate Time between two frames
    /// \return the calculated speed
    float calculateAverageSpeed(float frameRate);

    /// \brief calculate the average speed along a segment of the track
    /// \param[in] startFrame Index of the frame where the calculation starts
    /// \param[in] endFrame Index of the frame where the calculation ends
    /// \param[in] frameRate Time between two frames
    /// \return the calculated speed
    float calculateAverageSpeed(int startFrame, int endFrame, float frameRate);

    /// \brief Calculate the average distance to a reference point
    /// \param[in] refX X position of the reference point
    /// \param[in] refY Y position of the reference point
    float calculateAverageDistanceToRefPoint(float refX, float refY);

    /// \brief Calculate the average distance to a reference point
    /// \param[in] refX X position of the reference point
    /// \param[in] refY Y position of the reference point
    /// \param[in] startFrame Index of the frame where the calculation starts
    /// \param[in] endFrame Index of the frame where the calculation ends
    float calculateAverageDistanceToRefPoint(float refX, float refY, int startFrame, int endFrame);

private:
    std::vector<blSPoint*> m_track; ///< track points
};
