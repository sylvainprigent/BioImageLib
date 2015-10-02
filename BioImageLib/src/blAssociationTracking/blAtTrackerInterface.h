/// \file blAtTrackerInterface.h
/// \brief blAtTrackerInterface class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2013

#pragma once

#include "blAtCostInterface.h"
#include "blAtTrackSorterInterface.h"
#include "blAtTrack.h"
#include <blCore>

#include "blAssociationTrackingExport.h"

// //////////////////////////////////////////////////////// //
//                   blAtTrackerInterface
// ////////////////////////////////////////////////////// //
/// \class blAtTrackerInterface
/// \brief Abstract class defining common function for association tracker on a movie using object detection on each frames
class BLASSOCIATIONTRACKING_EXPORT blAtTrackerInterface : public blProcess{

public:
    /// \fn blAtTrackerInterface(blAtCostInterface *costFunction, std::vector<string> framesFiles, std::vector<std::vector<std::vector<float> > > objects);
    /// \brief Constructor
    /// \param[in] costFunction  Pointer to the object that calculate the cost function
    /// \param[in] framesFiles List of the files containing the frames
    /// \param[in] objects List of detected objects in each frame object [frame][rank object][object info]
    blAtTrackerInterface(blAtCostInterface *costFunction, std::vector<std::string> framesFiles, std::vector<std::vector<std::vector<float> > > objects);

    /// \fn blAtTrackerInterface();
    /// \brief Constructor
    blAtTrackerInterface();

    /// \fn ~blAtTrackerInterface();
    /// \brief Destructor
    ~blAtTrackerInterface();

public:
    // parameters
    /// \fn void initParametersWithDefaultsValues();
    /// \brief initialize the parameters with default values
    void initParametersWithDefaultsValues();

public:

    // setters
    /// \fn void setTimeSpread(int timeSpread);
    /// \param[in] timeSpread Number of frame the tracker can jump
    ///  to connect two detections.
    ///  ex: Set timeSpread = 1 to connect only consecutive frames
    ///  ex: Set timeSpread = 2 to connect frame n with n+1 and n+2
    void setTimeSpread(int timeSpread);
    /// \fn void setL(float L);
    ///  \param[in] L Maximum spatial distance for two detections
    ///  to be considered as too far to be connected
    void setL(float L);

public:
    // Inputs
    /// \fn void SetTrackSorter(blAtTracksSorterInterface *sorter);
    /// \param[in] sorter Pointer to the track sorter
    void setTrackSorter(blAtTrackSorterInterface *sorter);

    // outputs
    /// \fn std::vector<blAtTrack*> output();
    /// \return list of founded tracks.
    std::vector<blAtTrack*> output();

    /// \fn std::vector<blAtConnection*> getConnections();
    /// \return list of the objects organized like the constructor input "object"
    /// with the last object info corresponding to the rank of the object in the next frame that is linked to the curent object
    std::vector<blAtConnection*> getConnections();

    /// \fn FloatRGBV2DImage::Pointer getTracksRepresentationOnFirstImageImp();
    /// \return a representation of the tracks drown in the first frames
    FloatColor2DImage::Pointer getTracksRepresentationOnFirstImageImp();
    /// \fn blImage* getTracksRepresentationOnFirstImage();
    /// \return a representation of the tracks drown in the first frames
    blImage* getTracksRepresentationOnFirstImage();

    /// \fn void SaveAllConnectionsOnFirstFrame(std::string rootFileName);
    /// \brief Save the founded connections on the first frame
    /// \param[in] rootFileName Name of the file where the result is save
    void saveAllConnectionsOnFirstFrame(std::string rootFileName);

    /// \fn void SaveTimeTracksRepresentation(std::string rootFileName);
    /// \brief Save the tracking result in a list of images representing the time evolution
    /// only one segment of the track is represented at each time
    /// \param[in] rootFileName Name of the file where the result is save
    void saveTimeTracksRepresentation(std::string rootFileName);

    /// \fn void saveTimeTracksRepresentationOneFrame(std::string rootFileName);
    /// \brief Save the tracking result by projecting all the tracks in the first
    ///  frame of the input image sequence
    /// \param[in] rootFileName Name of the file where the result is save
    void saveTimeTracksRepresentationOneFrame(std::string rootFileName);

    /// \fn void blAtTrackerInterface::SaveTimeTracksRepresentationFull(std::string rootFileName);
    /// \brief Save the tracking result in a list of images representing the time evolution
    /// all the track (before curent time) is represented at each time
    /// \param[in] rootFileName Name of the file where the result is save
    void saveTimeTracksRepresentationFull(std::string rootFileName);

    /// \fn void calculateTracks();
    /// \brief Calculate the tracks from the connection list.
    /// In this version, the output tracks can be only linear
    void calculateTracks();

    /// \fn void saveTracksToTxtFile();
    /// \brief saveTracksToTxtFile save the tracks into a txt file.
    /// Each track is saved as the list objects states followed by the frame index.
    /// Tracks are separated by "track i", where i is the index given to the track
    /// \param[in] fileName Name of the txt file. (full path)
    void saveTracksToTxtFile(std::string fileName);

    /// \fn void loadTracksFromTxtFile(std::string fileName);
    /// \brief loadTracksFromTxtFile Load the tracks from a txt file.
    /// Each track in the file should be a list objects states followed by the frame index.
    /// Tracks are separated by "track i", where i is the index given to the track
    /// \param[in] fileName Name of the txt file. (full path)
    void loadTracksFromTxtFile(std::string fileName);

    /// \fn void setUseSorter(bool useSorter);
    /// \param[in] useSorter True if a sorter (blAtTrackSorterInterface) is
    /// use to select some tracks as a post processing
    void setUseSorter(bool useSorter);

    /// \fn virtual void run() = 0;
    /// \brief Starts the calculation
    virtual void run() = 0;

    /// \fn std::vector<std::vector<int> > getTracksCosts();
    /// \return the costs of all the tracks
    std::vector<std::vector<int> > getTracksCosts();

    /// \fn std::vector<blAtTrack* > getTracks();
    /// \return the tracks
    std::vector<blAtTrack* > getTracks();

    /// \fn void setInputs(blAtCostInterface *costFunction, std::vector<std::string> framesFiles, std::vector<std::vector<std::vector<float> > > objects);
    /// \param[in] costFunction  Pointer to the object that calculate the cost function
    /// \param[in] framesFiles List of the files containing the frames
    /// \param[in] objects List of detected objects in each frame object [frame][rank object][object info]
    void setInputs(blAtCostInterface *costFunction, std::vector<std::string> framesFiles, std::vector<std::vector<std::vector<float> > > objects);

protected:
    // intermediate data
    blAtCostInterface *m_costFunction; ///< Pointer to the cost function
    std::vector<std::string> m_framesFiles; ///< List of the frames names
    unsigned int m_frameNumber; ///< Contain the number of frames
    int m_objectInfoSize; ///< Size of the object state
    std::vector<blAtTrack* > m_tracks; ///< List of the founded tracks
    std::vector<std::vector<int> > m_tracksCosts;
    std::vector<blAtConnection*> m_connections; ///< List of the founded connections
    std::vector<std::vector<std::vector<float> > > m_objects; ///< List of objects in each frames
    bool m_is3D; ///< Allaws to know if the image is 3D
    bool m_tracksAlreadyCalculated; ///< set to true is the tracker calculetes the tracks by itself

    // Settings
    blAtTrackSorterInterface* m_trackSorter; ///< Pointer to the track sorter
    bool m_useSorter; ///< Allows to know if the track sorter has been set
    float m_L; ///< maximum move of an object between two consecutive frames
    int m_timeSpread; ///< number of consecutive frames to annalyse in order to connect an object at curent time

    // methods
    /// \fn int isConnectionStartingPointInTrack(std::vector<blAtTrack*> tracks, blAtConnection* connection);
    /// \brief Check if the connection "connection" goes at the following of one of the tracks in "tracks"
    int isConnectionStartingPointInTrack(std::vector<blAtTrack*> tracks, blAtConnection* connection);

    /// \fn void addTrack(std::vector<std::vector<float> > points);
    /// \brief add a track to the track list
    /// \param[in] points Track to add
    void addTrack(std::vector<std::vector<float> > points);

private:
    bool m_returnedOutput; ///< boolean for destructor
};
