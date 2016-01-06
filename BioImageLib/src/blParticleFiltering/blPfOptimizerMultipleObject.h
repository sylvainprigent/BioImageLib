#pragma once

#include "blPfOptimizerSingleObject.h"
#include "blParticleFilteringExport.h"

/// \class blPfOptimizerMultipleObject
/// \brief Class the allows to do tracking on several object using particle filtering.
/// This class just call blPfSingleObject for each of the input object individually
class BLPARTICLEFILTERING_EXPORT blPfOptimizerMultipleObject : public blProcess{

public:
    /// \fn blPfOptimizerMultipleObject(blPfModelInterface *particleModel, int startingFrameIdx = 0);
    /// \brief Constructor
    /// \param[in] particleModel Pointer to the class containing the model
    /// \param[in] startingFrameIdx Index of the frame where to start the tracking
    blPfOptimizerMultipleObject(blPfModelInterface *particleModel, int startingFrameIdx = 0);

    /// \fn blPfOptimizerMultipleObject(blPfModelInterface *particleModel, int particleNumber, int resamplingThreshold, std::string resamplingMethod, int startingFrameIdx = 0);
    /// \brief Constructor
    /// \param[in] particleModel Pointer to the class containing the model
    /// \param[in] particleNumber Number of particules to use
    /// \param[in] resamplingThreshold Treshold for resampling decision
    /// \param[in] resamplingMethod name of the choosen method
    /// \param[in] startingFrameIdx Index of the frame where to start the tracking
    blPfOptimizerMultipleObject(blPfModelInterface *particleModel, int particleNumber, int resamplingThreshold, std::string resamplingMethod, int startingFrameIdx = 0);

    /// \fn void run();
    /// \brief Run the calculation
    void run();

    /// \fn  std::vector<blPfTrack*> GetTracks();
    /// \brief Function that return the obtained tracks
    /// \return the obtained tracks
    std::vector<blPfTrack*> getTracks();

    void setTracks(std::vector<blPfTrack*> tracks);

    void saveTrackRepresentation(std::string imagePath);
    void saveTrackInTxt(std::string fileName);

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
};
