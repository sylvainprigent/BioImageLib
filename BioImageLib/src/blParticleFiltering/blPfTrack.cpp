#include "blPfTrack.h"

#include <iostream>
using namespace std;

// ////////////////////////////////////////////// //
//                  blPfTrack
// ////////////////////////////////////////////// //
blPfTrack::blPfTrack(){

}

blPfTrack::~blPfTrack(){
    for (unsigned int i = 0 ; i < m_states.size() ; ++i){
        delete m_states[i];
    }
}

void blPfTrack::addState(blPfState* state){
    m_states.push_back(state->copy());
}

vector<blPfState*> blPfTrack::getStates(){
    return m_states;
}

int blPfTrack::getTrackSize(){
    return m_states.size();
}

blPfState* blPfTrack::getStateAt(int i){
    //cout << "blPfTrack::getStateAt " << i;
    //cout << ", blPfTrack state is  " << m_states[i]->getStateAt(0) << ", " << m_states[i]->getStateAt(1) << ", " << m_states[i]->getStateAt(2) << endl;
    return m_states[i];
}

blPfState* blPfTrack::getLastState(){
    return m_states[m_states.size()-1];
}

void blPfTrack::setStateAt(int i, blPfState* state){
    m_states[i] = state;
}
