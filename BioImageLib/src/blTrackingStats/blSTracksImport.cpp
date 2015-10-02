/// \file blSTracksImport.cpp
/// \brief blSTracksImport class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#include "blSTracksImport.h"

blSTracks* blSTracksImport::importFromblAt(std::string blAtTracksFile){

    blAtTrackerNull tracker;
    tracker.loadTracksFromTxtFile(blAtTracksFile);
    return importFromblAt(tracker.getTracks());
}

blSTracks* blSTracksImport::importFromblAt(std::vector<blAtTrack* > tracks){

    // copy the tracks
    blSTracks* stracks = new blSTracks;
    int frameIdx, x, y;
    for (int i = 0 ; i < tracks.size() ; ++i){

        blAtTrack* atTrack = tracks[i];
        blSTrack *newTrack = new blSTrack;
        for(int p = 0 ; p < atTrack->getTrackSize() ; ++p){

            if (p == 0){
                frameIdx = atTrack->getConnectionAt(p)->startFrameIdx();
                x = atTrack->getConnectionAt(p)->stateStart()[0];
                y = atTrack->getConnectionAt(p)->stateStart()[1];
                blSPoint *newPoint = new blSPoint(frameIdx, x, y);
                newTrack->addPoint(newPoint);
            }
            frameIdx = atTrack->getConnectionAt(p)->endFrameIdx();
            x = atTrack->getConnectionAt(p)->stateEnd()[0];
            y = atTrack->getConnectionAt(p)->stateEnd()[1];
            blSPoint *newPoint = new blSPoint(frameIdx, x, y);
            newTrack->addPoint(newPoint);
        }
        stracks->addTrack(newTrack);
    }
    return stracks;
}
