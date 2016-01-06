/// \file blSTracks.h
/// \brief blSTracks class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#pragma once

#include "blSTrack.h"

#include "blTrackingStatsExport.h"
#include "blStatistics/blDistributionInfo.h"
#include <string>

/// \class blSTracks
/// \brief Class containing a group of blSTrack
class BLTRACKINGSTATS_EXPORT blSTracks
{
public:

    /// \brief blSTrack Constructor
    blSTracks();

    blSTracks(std::vector<blSTrack*> tracks);

public:
    // load/save tracks
    void loadTracksFromTxtFile(std::string fileName);
    void saveTracksToTxtFile(std::string fileName);
    void saveTracksToTxtFile(std::vector<blSTrack*> tracks, std::string fileName);
    void saveTracksAllInfoToTxtFile(std::vector<blSTrack *> tracks, std::string fileName);

public:
    // setters
    void addTrack(blSTrack* track);
    void addTrack(std::vector<std::vector<float> > curentpoints);
    void setTracks(std::vector<blSTrack*> tracks);

public:

    // tracks
    std::vector<blSTrack*> tracks(); ///< track list
    int tracksNumber();

    // point by point stats
    std::vector<float> averageDistance(int startFrameIdx, int endFrameIdx);
    std::vector<float> averageSpeed(int startFrameIdx, int endFrameIdx, float frameRate);
    std::vector<float> averageDistanceToRef(float refX, float refY, int startFrameIdx, int endFrameIdx);
    void averageDistanceToEachOthers(int startFrameIdx, int endFrameIdx);
    std::vector<float> averageDistanceToOthers(blSTrack* curentTrack, int startFrameIdx, int endFrameIdx);


    // make groups
    std::vector<blSTracks*> makeGroups(std::vector<std::vector<float> > gpoints);
    void saveTracksGroupsToFile(std::vector<blSTracks*> tracksGroups, std::string rootFileName);

    // global stats
    std::vector<float> groupeDistanceDist(int startFrame, int endFrame);
    blDistributionInfo groupeDistanceStats(int startFrame, int endFrame);
    std::vector<float> groupeStartToEndDistanceDist(int startFrame, int endFrame);
    blDistributionInfo groupeStartToEndDistanceStats(int startFrame, int endFrame);
    std::vector<float> groupeSpeedDist(int startFrame, int endFrame, float frameRate);
    blDistributionInfo groupeSpeedStats(int startFrame, int endFrame, float frameRate);
    std::vector<float> groupeDistanceToRefDist(float refX, float refY, int startFrame, int endFrame);
    blDistributionInfo groupeDistanceToRefStats(float refX, float refY, int startFrame, int endFrame);

private:
    std::vector<blSTrack*> m_tracks; ///< track list
};
