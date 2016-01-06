/// \file blRATState.cpp
/// \brief blRATState class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#include "blRATState.h"

blRATState::blRATState(){

}

blRATState::blRATState(std::vector<float> states){
    m_states = states;
}

blRATState::~blRATState(){

}

int blRATState::size(){
    return int(m_states.size());
}

void blRATState::setStates(std::vector<float> states){
    m_states = states;
}

void blRATState::setStateAt(int idx, float value){
    m_states[idx] = value;
}

float blRATState::getStateAt(int idx){
    return m_states[idx];
}

void blRATState::resize(int size){
    m_states.resize(size);
}
