#pragma once

#include "blPfOptimizerInterface.h"
#include "blParticleFilteringExport.h"

/// \class blPfMultipleObject
/// \brief Class the allows to do tracking on several object using particle filtering.
/// This class just call blPfSingleObject for each of the input object individually
class BLPARTICLEFILTERING_EXPORT blPfOptimizerMultipleObjectMultipleStart : public blProcess{

public:
    /// \fn blPfMultipleObject(blPfModelInterface *particleModel,int startingFrameIdx = 0);
    /// \brief Constructor
    /// \param[in] particleModel Pointer to the class containing the model
    /// \param[in] startingFrameIdx Index of the frame where to start the tracking
    blPfOptimizerMultipleObjectMultipleStart(blPfModelInterface *particleModel, int startingFrameIdx = 0);

    /// \fn blPfMultipleObject(blPfModelInterface *particleModel, int particleNumber, int resamplingThreshold, std::string resamplingMethod, int startingFrameIdx = 0);
    /// \brief Constructor
    /// \param[in] particleModel Pointer to the class containing the model
    /// \param[in] particleNumber Number of particules to use
    /// \param[in] resamplingThreshold Treshold for resampling decision
    /// \param[in] resamplingMethod name of the choosen method
    /// \param[in] startingFrameIdx Index of the frame where to start the tracking
    blPfOptimizerMultipleObjectMultipleStart(blPfModelInterface *particleModel, int particleNumber, int resamplingThreshold, std::string resamplingMethod, int startingFrameIdx = 0);

    /// \fn void setalreadyUsedNeighborThreshold(float alreadyUsedNeighborThreshold);
    /// Set the maximum neighborhood square euclidian distance for an object and it neighbors to be considered
    /// as the same.
    /// \param[in] alreadyUsedNeighborThreshold square euclidian distance
    void setalreadyUsedNeighborThreshold(float alreadyUsedNeighborThreshold);

    /// \fn void Update();
    /// \brief Run the calculation
    void run();

    /// \fn  std::vector<blPfTrack*> GetTracks();
    /// \brief Function that return the obtained tracks
    /// \return the obtained tracks
    std::vector<blPfTrack*> getTracks();

    void saveTrackRepresentation(std::string imagePath);
    FloatColor2DImage::Pointer getTracksRepresentationOnLastImage();
    void saveTracksToTxtFile(std::string fileName);

public:
    // parameters
    /// \fn void setParticleNumber(int particleNumber);
    /// \param[in] particleNumber Number of particules to use
    void setParticleNumber(int particleNumber);

    /// \fn void setResamplingThreshold(int resamplingThreshold);
    /// \param[in] resamplingThreshold Treshold for resampling decision
    void setResamplingThreshold(int resamplingThreshold);

    /// \fn void setResamplingMethod(std::string resampleingMethod);
    /// \brief To select which resampling method to use
    /// \param[in] resampleingMethod name of the choosen method
    /// Avialable methods are:
    ///             "ResampleMultinomial"
    ///             "ResampleResidual"
    ///             "ResampleStratified"
    ///             "ResampleSystematic"
    void setResamplingMethod(std::string resampleingMethod);

private:
   blPfModelInterface *m_particleModel;
   //int m_Ns, m_Nth;
   std::vector<blPfTrack*> m_tracks;

   // input parameters
   int m_particleNumber, m_resamplingThreshold, m_startingFrameIdx;
   std::string m_resamplingMethod;
   float m_alreadyUsedNeighborThreshold;

   // internal methods
   std::vector<blPfTrack*> runMultiplePFonFrame(int frameIdx);
   std::vector<int> findObject(blPfState *state);
};
