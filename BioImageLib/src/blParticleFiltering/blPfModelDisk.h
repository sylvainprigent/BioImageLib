#pragma once

#include "blPfModelInterface.h"

#include <vector>
#include "blParticleFilteringExport.h"

/// \class blPfModelDisk
/// \brief Class defining the model for tracking
/// a disk in 2D images. the disk model is the distance between
/// the inside and the border of the object using Bhattacharyya distance
class BLPARTICLEFILTERING_EXPORT blPfModelDisk : public blPfModelInterface{

public:
    /// \fn blPfModelDisk(blPfDataManager* dataManager);
    /// \brief Constructor
    /// \param[in] dataManager Data manager
    blPfModelDisk(blPfDataManager* dataManager);

public:
    // Virtual functions to implement for a specific model
    virtual void init();
    virtual void setCurentFrameIdx(int frameId);
    virtual std::vector<blPfParticle*> getInitialStates(int particleNumber, int startingFrameIdx, int objectIdx);
    virtual void particlePropagate(blPfParticle* particle, blPfState *selectedPreviousState);
    virtual float calculateParticuleWeight(blPfParticle* particle, float previousWeight, blPfTrack *previousSelectedStates);
    virtual blPfState* selectAndUpdateParticle(std::vector<blPfParticle*> particles, std::vector<float> weights);
    virtual int getOrderMarkovModel();

public:
    // parameters setters
    /// \fn void SetSigmaPredictionX(int sigma);
    /// \brief set the standard deviation for the
    /// prediction gaussian noise in the X direction
    void setSigmaPredictionX(int sigma);
    /// \fn void SetSigmaPredictionY(int sigma);
    /// \brief set the standard deviation for the
    /// prediction gaussian noise in the Y direction
    void setSigmaPredictionY(int sigma);
    /// \fn void SetObjectRadius(int radius);
    /// \param[in] radius Radius od the observed object
    void setObjectRadius(int radius);
    /// \fn void setMaxMoveX(float maxMoveX);
    /// \param[in] maxMoveX Maximum possible move of a particle in the X direction
    void setMaxMoveX(float maxMoveX);
    /// \fn void setMaxMoveY(float maxMoveY);
    /// \param[in] maxMoveY Maximum possible move of a particle in the Y direction
    void setMaxMoveY(float maxMoveY);

private:
    // internal methods
    /// \fn void calculateInsideCircleCoordinates();
    /// \brief Calculate the coordinates of the pixels inside the disk
    void calculateInsideCircleCoordinates();
    /// \fn void calculateCircleBorderCoordinales();
    /// \brief Calculate the pixels in the border of the disk
    void calculateCircleBorderCoordinales();

private:
    // parameters
    float m_VarPreditionNoiseX;
    float m_VarPreditionNoiseY;
    int m_objectRadius, m_stateSize;
    int m_nl, m_nc;
    float m_maxMoveX, m_maxMoveY;
    int m_startingFrameIdx;

private:
    // internal data
    std::vector<std::vector<int> > m_insidePixels, m_borderPixels;
    int m_initX, m_initY;
    std::vector<Float2DImage::Pointer> m_dataFrames;
};
