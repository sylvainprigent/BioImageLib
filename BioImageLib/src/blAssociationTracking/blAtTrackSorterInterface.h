/// \file blAtTrackSorterInterface.h
/// \brief blAtTrackSorterInterface class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2013

#pragma once

#include "blCore/blProcess.h"
#include "blAtTrack.h"
#include "blAssociationTrackingExport.h"

// ///////////////////////////////////////////////// //
//              blAtTrackSorterInterface
// //////////////////////////////////////////////// //
/// \class blAtTrackSorterInterface
/// \brief Virtual class defining the interface of a track sorter to eliminate useless tracks
class BLASSOCIATIONTRACKING_EXPORT blAtTrackSorterInterface : public blProcess{

public:
    /// \fn blAtTracksSorterInterface(std::vector<blAtTrack*> tracks);
    /// \brief Constructor
    /// \param[in] tracks Tracks to be sorted
    blAtTrackSorterInterface(std::vector<blAtTrack*> tracks);
    /// \fn blAtTrackSorterInterface();
    /// \brief Constructor
    blAtTrackSorterInterface();
    /// \fn ~blAtTracksSorterInterface();
    /// \brief Destructor
    ~blAtTrackSorterInterface();

public:
    // parameters
    /// \fn void initParametersWithDefaultsValues();
    /// \brief initialize the parameters with default values
    void initParametersWithDefaultsValues();

public:
    // virtual
    /// \fn virtual void run() = 0;
    /// \brief run the sorter
    virtual void run() = 0;

public:
    // setters
    /// \fn void SetTracks(std::vector<std::vector<std::vector<float> > > tracks);
    /// \param[in] tracks Tracks to sort
    void setTracks(std::vector<blAtTrack *> &tracks);

    /// \fn std::vector<std::vector<std::vector<float> > > Getoutput();
    /// \return the sorted tracks
    std::vector<blAtTrack*> getoutput();

    /// \fn void setTacksCosts(std::vector<std::vector<int> > tracksCosts);
    /// \param[in] tracksCosts Costs of all the tracks
    void setTacksCosts(std::vector<std::vector<int> > tracksCosts);

    /// \fn void setMovieImagesPath(std::vector<std::string> imagesFiles);
    /// \param[in] imagesFiles Image sequence to process (stored as individual frame files)
    void setMovieImagesPath(std::vector<std::string> imagesFiles);

protected:
   std::vector<blAtTrack* > m_tracks; ///< List of input tracks
   std::vector<blAtTrack* > m_sortedTracks; ///< List of remaining tracks after sorting
   std::vector<std::vector<int> > m_tacksCosts; ///< Costs of all the tracks
   std::vector<std::string> m_imagesFiles; ///< Image sequence to process (stored as individual frame files)
};
