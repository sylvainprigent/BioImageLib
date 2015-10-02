#include "blAtTrackSorterInterface.h"

using namespace std;

// ///////////////////////////////////////////////// //
//             blAtTracksSorterInterface
// //////////////////////////////////////////////// //
blAtTrackSorterInterface::blAtTrackSorterInterface(vector<blAtTrack *> tracks) : blProcess(){
    m_tracks = tracks;
}

blAtTrackSorterInterface::blAtTrackSorterInterface(){

}

blAtTrackSorterInterface::~blAtTrackSorterInterface(){

}

// //////////// parameters //////////////////////////////
void blAtTrackSorterInterface::initParametersWithDefaultsValues(){

}

// //////////////////////////////////////////////////////

vector<blAtTrack *> blAtTrackSorterInterface::getoutput(){
    return m_sortedTracks;
}

void blAtTrackSorterInterface::setTracks(vector<blAtTrack *> &tracks){
    m_tracks = tracks;
}

void blAtTrackSorterInterface::setTacksCosts(vector<vector<int> > tracksCosts){
    m_tacksCosts = tracksCosts;
}

void blAtTrackSorterInterface::setMovieImagesPath(vector<string> imagesFiles){
    m_imagesFiles = imagesFiles;
}
