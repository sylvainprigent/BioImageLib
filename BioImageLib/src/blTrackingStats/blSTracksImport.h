/// \file blSTracksImport.h
/// \brief blSTracksImport class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#pragma once

#include <blAssociationTracking>
#include "blSTracks.h"

/// \class blSTracks
/// \brief Class containing a group of blSTrack
class BLTRACKINGSTATS_EXPORT blSTracksImport
{
public:

    static blSTracks* importFromblAt(std::string blAtTracksFile);
    static blSTracks* importFromblAt(std::vector<blAtTrack* > tracks);
};
