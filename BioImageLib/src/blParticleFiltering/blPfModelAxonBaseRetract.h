#pragma once

#include "blPfModelInterface.h"

#include <vector>
#include "blParticleFilteringExport.h"

/// \class blPfModelAxonRetraction
/// \brief Class defining the model for axon tracking in 3D
/// using particle filtering. The state model is:
/// state = [frameIdx, x, y, z, vx, vy, vz, theta] with
/// (x,y,z): position in 3D
/// (vx, vy, vz): velocity in 3D
/// theta: velocity angle in the x-y plane
class BLPARTICLEFILTERING_EXPORT blPfModelAxonBaseRetract : public blPfModelInterface{
public:
    /// \fn blPfModelAxonBaseRetract(blPfDataManager* dataManager);
    /// \brief Constructor
    /// \param[in] dataManager Pointer to the particle filtering data manager
    /// \param[in] paramManager Pointer tot he parameters manager
    blPfModelAxonBaseRetract(blPfDataManager* dataManager);

    // Virtual functions to implement for a specific model
    virtual void init();
    virtual void setCurentFrameIdx(int frameId);
    virtual std::vector<blPfParticle*> getInitialStates(int particleNumber, int startingFrameIdx, int objectIdx);
    virtual void particlePropagate(blPfParticle* particle, blPfState *selectedPreviousState);
    virtual float calculateParticuleWeight(blPfParticle* particle, float previousWeight, blPfTrack *previousSelectedStates);
    virtual blPfState* selectAndUpdateParticle(std::vector<blPfParticle*> particles, std::vector<float> weights);
    virtual void setCurentObjectNeighborhood(blPfState* selectedPreviousState);

    // parameters setters
    /// \fn void setSigmaPredictionNoise(float sigmaX, float sigmaY, float sigmaZ );
    /// \brief set the standard deviation for the prediction gaussian noise in the 3 direction
    /// \param[in] sigmaX standard int X direction
    /// \param[in] sigmaY standard int Y direction
    /// \param[in] sigmaZ standard int Y direction
    void setSigmaPredictionNoise(float sigmaX, float sigmaY, float sigmaZ );
    /// \fn void setModelParameters(float alpha, float beta, float gamma);
    /// \brief set the model parameters
    /// \param[in] alpha Weight of the observation likelyhood
    /// \param[in] beta Weight of the vesselness term
    /// \param[in] gamma Weight of the direction term
    void setModelParameters(float alpha, float beta, float gamma);
    /// \fn void setPredictionParameters(float proportionStatic, float proportionDynamic, float proportionDetection);
    /// \brief Set the prediction proportion to how particle should be spread
    /// \param[in] proportionStatic proportion of spreading around the curent position
    /// \param[in] proportionDynamic proportion of spreading around the curent position plus velocity
    /// \param[in] proportionDynamicReverse proportion of spreading around the curent position minus velocity
    /// \param[in] proportionDetection proportion of spreading around detections
    /// The sum of the three parameters MUST be equal to 1
    void setPredictionParameters(float proportionStatic, float proportionDynamic, float proportionDynamicReverse, float proportionDetection);
    /// \fn void setMaxMove(int m_maxMoveX, int m_maxMoveY, int m_maxMoveZ);
    /// \brief Set the windows size where the axon is expected to move.
    /// a particle out of this boundary is set to 0
    /// \param[in] maxMoveX Size of the window in the X direction (total size will be 2*sizeX+1)
    /// \param[in] maxMoveY Size of the window in the Y direction (total size will be 2*sizeY+1)
    /// \param[in] maxMoveZ Size of the window in the Z direction (total size will be 2*sizeZ+1)
    void setMaxMove(int maxMoveX, int maxMoveY, int maxMoveZ);
    /// \fn void setPredictionWindowSize(int x, int y, int z)
    /// \param[in] x Likelihood Window Size X
    /// \param[in] y Likelihood Window Size Y
    /// \param[in] z Likelihood Window Size Z
    void setPredictionWindowSize(int x, int y, int z);
    /// \fn void setVesselnessImagesPath(std::string path)
    /// \param[in] URL of the folder containing the precalculated vesselness images
    void setVesselnessImagesPath(std::string path);

private:
    // Parameters
    float m_SigmaPredictionNoiseX, m_SigmaPredictionNoiseY, m_SigmaPredictionNoiseZ;
    int m_initX, m_initY, m_initZ;
    int m_maxMoveX, m_maxMoveY, m_maxMoveZ;
    float m_alpha, m_beta, m_gamma;
    float m_PredictionProportionStatic, m_PredictionProportionDynamic, m_PredictionProportionDetection, m_proportionDynamicReverse;
    int m_likelihoodWindowSizeX, m_likelihoodWindowSizeY, m_likelihoodWindowSizeZ;
    // internal data
    int m_stateSize;
    Float3DImage::Pointer m_vesselnessImage, m_vesselnessImagePrevious;
    Float3DImage::Pointer m_mask;
    int m_minMinor, m_maxMinor, m_minMajor, m_maxMajor;
    int m_radius;
    std::vector<std::vector<float> > m_neighborObjects;
    int m_PreviousFrameId;
    float m_initMinor, m_initMajor, m_initTheta;
    int m_startingFrameIdx;
    std::string m_vesselnessImagesPath;


    // internal methods
    float calculateObservationLikelihoodBhattacharyyaEllipse(blPfParticle *particle, float minor, float major);
    void getInsideEllipsePixelsValues(const std::vector<float> object, std::vector<float> &insideValues);
    float calculateObservationLikelihood(blPfParticle* particle);
    float calculateVesselness(blPfParticle* particle);
    float calculateMotionModel(blPfParticle* particle);
    void partitionEllipseIn3D( std::vector<float> object, std::vector<std::vector<int> > borderPixels, std::vector<float> outsideValues, std::vector<float> &vhead1, std::vector<float> &vhead2, std::vector<float> &vside1, std::vector<float> &vside2);
    void getEllipseBorderPixelsCoordinates(std::vector<float> object, std::vector<std::vector<int> > &outsidePixels);
};
