#pragma once

#include "blPfParticle.h"
#include "blPfTrack.h"
#include "blPfDataManager.h"
#include <blCore>
#include "blParticleFilteringExport.h"

/// \class blPfModelInterface
/// \brief Abstract class defining the prototype of a particle model for particle filtering
/// this class is used by 'ParticleFilter'
class BLPARTICLEFILTERING_EXPORT blPfModelInterface : public blProcess{
public:
    /// \fn blPfModelInterface(blPfDataManager* dataManager);
    /// \brief Constructor
    /// \param[in] dataManager Pointer to the data manager
    /// \param[in] paramManager pointer to the parameters manager
    blPfModelInterface(blPfDataManager* dataManager);

    /// \fn int getFrameNumber();
    /// \return the number of frames in the movie
    int getFrameNumber();

    /// \fn std::string getModelName();
    /// \return the nale of the object
    std::string getModelName();

    /// \fn blPfDataManager* getDataManager();
    /// \return the data manager
    blPfDataManager* getDataManager();

    /// \fn int getCurentFrameIdx();
    /// \return the curent frame index
    int getCurentFrameIdx();

    /// \fn bool isSelectionBeforeResampling();
    /// \brief Return the boolean that indicates if the particle selection
    /// must be done before (or after) the resampling
    bool isSelectionBeforeResampling();

    /// \fn void setSelectedPreviousState(blPfState* selectedPreviousState);
    /// \brief Method used by the particle filter to update the previously selected state to the model
    /// \param[in] selectedPreviousStates Previously selected state
    void setSelectedPreviousState(blPfState* selectedPreviousState);

    // Virtual functions to implement for a specific model
    /// \fn virtual void init() = 0;
    /// \brief All the calculation that need to be done before starting
    virtual void init() = 0;

    /// \fn virtual void setCurentFrameIdx(int frameId) = 0;
    /// \brief Function that allows to load the needed data for the
    /// particle filtering in the curent frames.
    virtual void setCurentFrameIdx(int frameId) = 0;
    /// \fn virtual std::vector<blPfParticle*> getInitialStates(int particleNumber, int startingFrameIdx, int objectIdx) = 0;
    /// \brief Function that generate the initial states of the particules
    /// \param[in] particleNumber Number of particles to generate
    /// \param[in] startingFrameIdx Index of the starting frame
    /// \param[in] objectIdx Index of the object to consider i the object list
    /// \return a std::vector containing the particules states
    virtual std::vector<blPfParticle*> getInitialStates(int particleNumber, int startingFrameIdx, int objectIdx) = 0;
    /// \fn virtual blPfParticle* particleSpread(blPfParticle* particle, blPfState *selectedPreviousState) = 0;
    /// \brief Function that update the state of a particule (motion model)
    /// \param[in] particle Curent particule
    /// \param[in] selectedPreviousState Pointer to the particle selected at the previous state
    /// \return the updated particule
    virtual void particlePropagate(blPfParticle* particle, blPfState *selectedPreviousState) = 0;
    /// \fn virtual float CalculateParticuleWeight(blPfParticle* particle, float previousWeight) = 0;
    /// \brief Function that cacluate the weight associated to a particle
    /// \param[in] particle Particule to consider
    /// \param[in] previousWeight Previous weight of the particle in the previous frame
    /// \return the calculated weight
    virtual float calculateParticuleWeight(blPfParticle* particle, float previousWeight, blPfTrack *previousSelectedStates) = 0;
    /// \fn virtual blPfState* SelectAndUpdateParticle(std::vector<blPfParticle*> particles, std::vector<float> weights) = 0;
    /// \brief Function that allows to select the particle for the curent frame
    /// and update the state of the other (calculate velocity for example)
    virtual blPfState* selectAndUpdateParticle(std::vector<blPfParticle*> particles, std::vector<float> weights) = 0;
    /// \fn virtual void setCurentObjectNeighborhood(blPfState* selectedPreviousState);
    /// \brief Reimplement this function to select objects in a specified neighborhood.
    /// This function is needed only when there is a detection on each frame
    virtual void setCurentObjectNeighborhood(blPfState* selectedPreviousState);

    /// \fn virtual int getOrderMarkovModel();
    /// \return the number of state (in time sequence) that are used bu the model
    /// This number is used to save memory. Use -1 for no memory to be free.
    virtual int getOrderMarkovModel();

protected:
    blPfDataManager* m_dataManager; ///< Pointer to the parameters manager
    std::string m_particleModelName; ///< Name of the particle filtering model
    int m_frameId; ///< index of thecurent frame
    bool m_selectBeforeResampling; ///< Allows to know if the particle selection must be done before or after resampling
    blPfState* m_selectedPreviousState; ///< Pointer to the selected particle at the prefious frame
    std::vector<std::vector<float> > m_neighborObjects; ///< List of neighboohood object in the case using detection
};
