/// \file blRATConnection.cpp
/// \brief blRATConnection class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#include "blRATConnection.h"

blRATConnection::blRATConnection(){
    m_startState = 0;
    m_endState = 0;
    m_startFrameIdx = -1;
    m_endFrameIdx = -1;
    m_energy = -1;
}

blRATConnection::~blRATConnection(){

}

void blRATConnection::setStartState(blRATState* state, unsigned int frameIdx){
    m_startState = state;
    m_startFrameIdx = frameIdx;
}

void blRATConnection::setEndState(blRATState* state, unsigned int frameIdx){
    m_endState = state;
    m_endFrameIdx = frameIdx;
}

blRATState* blRATConnection::getStartState(){
    return m_startState;
}

blRATState* blRATConnection::getEndState(){
    return m_endState;
}

unsigned int blRATConnection::getStartFrameIdx(){
    return m_startFrameIdx;
}

unsigned int blRATConnection::getEndFrameIdx(){
    return m_endFrameIdx;
}

void blRATConnection::setEnergy(float energy){
    m_energy = energy;
}

float blRATConnection::energy(){
    return m_energy;
}
