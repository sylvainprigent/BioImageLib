#pragma once

#include <blCore/blProcess.h>
#include "blPfModelInterface.h"
#include "blParticleFilteringExport.h"

/// \class blPfOptimizerInterface
/// \brief Class that implement common functions for partcicle filtering
/// Several resampling methods are avialable:
///             "ResampleMultinomial"
///             "ResampleResidual"
///             "ResampleStratified"
///             "ResampleSystematic"
class BLPARTICLEFILTERING_EXPORT blPfOptimizerInterface : public blProcess{

public:
    static const std::string ResampleMultinomial;
    static const std::string ResampleResidual;
    static const std::string ResampleStratified;
    static const std::string ResampleSystematic;

public:
    /// \fn blPfOptimizerInterface(blPfModelInterface *particleModel, int startingFrameIdx = 0, int objectIdx = 0);
    /// \brief Constructor
    /// \param[in] particleModel Pointer to the class containing the model
    /// \param[in] paramManager Pointer to the class managing the paramaters
    /// \param[in] objectIdx Index of the object to consider in the object list
    /// \param[in] startingFrameIdx Index of the frame where to start the tracking
    blPfOptimizerInterface(blPfModelInterface *particleModel, int startingFrameIdx = 0, int objectIdx = 0);

    /// \fn blPfInterface(blPfModelInterface *particleModel, int particleNumber, int resamplingThreshold, string resamplingMethod, int startingFrameIdx = 0, int objectIdx = 0);
    /// \brief Constructor
    /// \param[in] particleModel Pointer to the class containing the model
    /// \param[in] particleNumber Number of particules to use
    /// \param[in] resamplingThreshold Treshold for resampling decision
    /// \param[in] resamplingMethod name of the choosen method
    /// \param[in] startingFrameIdx index of the frame the calculation start
    /// \param[in] objectIdx index of the object to consider in the starting frame
    blPfOptimizerInterface(blPfModelInterface *particleModel, int particleNumber, int resamplingThreshold, std::string resamplingMethod, int startingFrameIdx = 0, int objectIdx = 0);

    virtual ~blPfOptimizerInterface();
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

    /// \fn blPfState* oneStepParticleFilter(int curentFrameIdx)
    /// \brief function that run one step of particle filtering
    /// One step meaning moving to the next frame
    /// \param[in] curentFrameIdx Index of the frame where to compute the step
    /// \return The selected state
    blPfState* oneStepParticleFilter(int frameIdx);

    /// \fn blPfState* oneStepParticleFilter(int curentFrameIdx)
    /// \brief function that run one step of particle filtering
    /// One step meaning moving to the next frame
    /// \return The selected state
    blPfState* oneStepParticleFilter();

    /// \fn void useSaveParticlesStatesRepresentation(std::string outputFolderPath);
    /// \brief Call this function after blPfInterface to save the curent states (only position) of the particles
    /// \param[in] outputFolderPath Path of the folder where to save the intermediate
    /// particles states representation
    void useSaveParticlesStatesRepresentation(std::string outputFolderPath);

    blPfTrack* getTrack();

    /// \fn void removeParticleFirstState();
    /// \brief Call this function after OneStepParticleFilter to save memory
    /// I removes the first particles states in the particle pile
    void removeParticleFirstState();

public:
    /// \brief void Initialization(bool useFileParam);
    /// \brief Initialize the particle filtering
    void initialization();

protected:
    /// \fn void SaveParticlesStatesRepresentation();
    /// \brief Save the curent states (only position) of the particles
    /// at the curent frame
    void saveParticlesStatesRepresentation();
    /// \fn void ResampleMultinomial();
    /// \brief implement the multinomial resampling
    void resampleMultinomial();
    /// \fn void ResampleResidual();
    /// \brief implement the residual resampling
    void resampleResidual();
    /// \fn void ResampleStratified();
    /// \brief implement the stratified resampling
    void resampleStratified();
    /// \fn void ResampleSystematic();
    /// \brief implement the systematic resampling
    void resampleSystematic();

protected:
    typedef void (blPfOptimizerInterface::*Resampling)(); ///< Type for a resampling function pointer
    Resampling ResamplingPtr; ///< Pointer to resampling function

protected:
    blPfTrack* m_track; ///< Calculated track
    int m_curentFrameIdx; ///< index of the curent frames
    std::string m_resamplingMethod; ///< Name of the selected resampling method
    blPfModelInterface* m_particleModel; ///< Pointer to the particle filtering model
    std::vector<blPfParticle*> m_particles; ///< Set of particles
    std::vector<float> m_curentweights; ///< Weights for each particles
    int m_startingFrameIdx; ///< index of the frame the calculation start
    int m_objectIdx; ///< index of the object to consider in the starting frame

    int m_iter;
    int m_SizeParticleFrameInMemory;

    // save intermediate results
    std::string m_interOutputFolderPath;
    bool m_useSaveInter;
    blPfState* m_selectedParticleState;

    // parameters
    int m_Ns; ///< particle number
    float m_Nth; ///< resampling threshold
};
