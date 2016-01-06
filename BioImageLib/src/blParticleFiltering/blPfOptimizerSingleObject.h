#pragma once

#include "blPfOptimizerInterface.h"
#include "blParticleFilteringExport.h"

/// \class blPfOptimizerSingleObject
/// \brief Class the allows to do tracking using particle filtering.
/// It track only one object
/// Several resampling methods are avialable:
///             "ResampleMultinomial"
///             "ResampleResidual"
///             "ResampleStratified"
///             "ResampleSystematic"
class BLPARTICLEFILTERING_EXPORT blPfOptimizerSingleObject : public blPfOptimizerInterface{

public:
    /// \fn blPfOptimizerSingleObject(blPfModel *particleModel, blProcessParameters* paramManager, int startingFrameIdx = 0, int objectIdx = 0);
    /// \brief Constructor
    /// \param[in] particleModel Pointer to the model
    /// \param[in] startingFrameIdx Index of the frame when to start the tracking
    /// \param[in] objectIdx index of the object to track from frame startingFrameIdx
    blPfOptimizerSingleObject(blPfModelInterface *particleModel, int startingFrameIdx = 0, int objectIdx = 0);

    /// \fn blPfOptimizerSingleObject(blPfModel *particleModel, int particleNumber, int resamplingThreshold, std::string resamplingMethod, int startingFrameIdx = 0, int objectIdx = 0);
    /// \brief Constructor
    /// \param[in] particleModel Pointer to the model
    /// \param[in] particleNumber number of particles to use in the filter
    /// \param[in] resamplingThreshold Threshold used for resampling
    /// \param[in] resamplingMethod Name og the resampling method to use
    /// \param[in] startingFrameIdx Index of the frame when to start the tracking
    /// \param[in] objectIdx index of the object to track from frame startingFrameIdx
    blPfOptimizerSingleObject(blPfModelInterface *particleModel, int particleNumber, int resamplingThreshold, std::string resamplingMethod, int startingFrameIdx = 0, int objectIdx = 0);

    /// \fn void run();
    /// \brief Run the calculation
    void run();

    /// \fn std::vector<std::vector<float> > GetTrack();
    /// \brief Function that return the obtained track
    /// \return the obtained track
    blPfTrack *getTrack();

    // visualization and intermediate results
    void saveParticulesPositions(blPfState *selectedParticle);
    void saveTrackRepresentation(std::string imagePath);
};
