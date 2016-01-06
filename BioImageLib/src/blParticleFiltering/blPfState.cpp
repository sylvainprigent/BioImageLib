#include "blPfState.h"

using namespace std;

// ////////////////////////////////////////////// //
//                  blPfState
// ////////////////////////////////////////////// //
blPfState::blPfState(int stateSize){
    m_state.resize(stateSize);
    for (int i = 0 ; i < stateSize ; ++i){
        m_state[i] = -1;
    }
}

blPfState::blPfState(vector<float> stateData){
    m_state = stateData;
}

void blPfState::setState(vector<float> state){
    m_state = state;
}

vector<float> blPfState::getState(){
    return m_state;
}

void blPfState::setStateAt(int i, float stateEntry){
    m_state[i] = stateEntry;
}

float blPfState::getStateAt(int i){
    return m_state[i];
}

blPfState* blPfState::copy(){
    blPfState* state = new blPfState(m_state.size());
    for (unsigned int i = 0 ; i < m_state.size() ; ++i){
        state->setStateAt(i, m_state[i]);
    }
    return state;
}

int blPfState::size(){
    return m_state.size();
}
