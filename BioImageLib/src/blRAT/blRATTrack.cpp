/// \file blRATTrack.cpp
/// \brief blRATTrack class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#include "blRATTrack.h"

blRATTrack::blRATTrack(){

}

blRATTrack::~blRATTrack(){
    for (int i = 0 ; i < m_states.size(); ++i){
        delete m_states[i];
    }
}

blRATTrack::blRATTrack(blRATState *initialState, int frameIdx){
    m_states.push_back(initialState);
    m_framesIdx.push_back(frameIdx);
}


void blRATTrack::addState(blRATState *state, int frameIdx){
    m_states.push_back(state);
    m_framesIdx.push_back(frameIdx);
}

blRATState* blRATTrack::getLastState(){
    return m_states[m_states.size()-1];
}

blRATState* blRATTrack::getStateAt(int idx){
    return m_states[idx];
}

blRATState* blRATTrack::getStateAtFrame(int frameIdx){
    for (int i = 0 ; i < m_framesIdx.size() ; ++i){
        if (m_framesIdx[i] == frameIdx){
            return m_states[i];
        }
    }
    return 0;
}

unsigned int blRATTrack::getTrackSize(){
    return unsigned int(m_states.size());
}

unsigned int blRATTrack::getFrameIdxAt(unsigned int idx){
    return m_framesIdx[idx];
}

