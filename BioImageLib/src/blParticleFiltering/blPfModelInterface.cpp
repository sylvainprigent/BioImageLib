#include "blPfModelInterface.h"

using namespace std;

// ////////////////////////////////////////////// //
//                  blPfModelInterface
// ////////////////////////////////////////////// //
blPfModelInterface::blPfModelInterface(blPfDataManager *dataManager) : blProcess(){
    m_particleModelName = "ParticleModel";
    m_dataManager = dataManager;
    m_selectBeforeResampling = true;
}

string blPfModelInterface::getModelName(){
    return m_particleModelName;
}

int blPfModelInterface::getFrameNumber(){
    return m_dataManager->getTotalFrameNumber();
}

void blPfModelInterface::setSelectedPreviousState(blPfState* selectedPreviousState){
    m_selectedPreviousState = selectedPreviousState;
}

int blPfModelInterface::getCurentFrameIdx(){
    return m_frameId;
}

blPfDataManager* blPfModelInterface::getDataManager(){
    return m_dataManager;
}

bool blPfModelInterface::isSelectionBeforeResampling(){
    return m_selectBeforeResampling;
}

void blPfModelInterface::setCurentObjectNeighborhood(blPfState* selectedPreviousState){
    m_selectedPreviousState = selectedPreviousState;
}

int blPfModelInterface::getOrderMarkovModel(){
    return -1;
}
