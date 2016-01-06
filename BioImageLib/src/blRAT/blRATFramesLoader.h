/// \file blRATFramesLoader.h
/// \brief blRATFramesLoader class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#pragma once

#include "blRATTrack.h"
#include "blCore/blImage.h"
#include <vector>

/// \class blRATFramesLoader
/// \brief Class allowing to load frames images and plot the tracks on it
class BLRAT_EXPORT blRATFramesLoader
{
public:
    /// \fn blRATFramesLoader();
    /// \brief Constructor
    blRATFramesLoader();
    /// \fn ~blRATFramesLoader();
    /// \brief Destructor
    ~blRATFramesLoader();

    void setFramesUrls(std::vector<std::string> framesUrls);
    unsigned int frameNumber();

    void setTracks(std::vector<blRATTrack*> tracks);
    void saveTimeTracksRepresentation(std::string dirURL);

private:
    FloatColor2DImage::Pointer getIndividualRepresentationImage(std::string imageType, int nl, int nc, int i);

private:
    // features files
    std::vector<std::string> m_framesUrls;
    std::vector<blRATTrack*> m_tracks;
};
