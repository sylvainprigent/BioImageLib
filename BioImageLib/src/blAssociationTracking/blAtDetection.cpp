/// \file blAtDetection.cpp
/// \brief blAtDetection class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2013

#include "blAtDetection.h"

using namespace std;

blAtDetection::blAtDetection(vector<float> state, int frameIdx, int objectIdx){
    m_state = state;
    m_frameIdx = frameIdx;
    m_objectIdx = objectIdx;
}

vector<float> blAtDetection::state(){
    return m_state;
}

int blAtDetection::frameIdx(){
    return m_frameIdx;
}

int blAtDetection::objectIdx(){
    return m_objectIdx;
}
