/// \file blAtConnection.cpp
/// \brief blAtConnection class
/// \author Sylvain Prigent, Huei-Fang Yang
/// \version 0.1
/// \date 2013

#include "blAtConnection.h"

using namespace std;


blAtConnection::blAtConnection(vector<float> stateStart, vector<float> stateEnd,
                               int startFrameIdx, int endFrameIdx,
                               int startDetectionIdx, int endDetectionIdx){
    m_stateStart = stateStart;
    m_stateEnd = stateEnd;
    m_startFrameIdx = startFrameIdx;
    m_endFrameIdx = endFrameIdx;
    // hueifang: June 24, 2014
    m_startDetectionIdx = startDetectionIdx;
    m_endDetectionIdx = endDetectionIdx;
}

vector<float> blAtConnection::stateStart(){
    return m_stateStart;
}

vector<float> blAtConnection::stateEnd(){
    return m_stateEnd;
}

int blAtConnection::startFrameIdx(){
    return m_startFrameIdx;
}

int blAtConnection::endFrameIdx(){
    return m_endFrameIdx;
}

float blAtConnection::cost(){
    return m_cost;
}

void blAtConnection::setCost(float cost){
    m_cost = cost;
}

int blAtConnection::startDetectionIdx(){
    return m_startDetectionIdx;
}

int blAtConnection::endDetectionIdx(){
    return m_endDetectionIdx;
}


//blAtConnection::blAtConnection(vector<float> stateStart, vector<float> stateEnd, int startFrameIdx, int endFrameIdx){
//    m_stateStart = stateStart;
//    m_stateEnd = stateEnd;
//    m_startFrameIdx = startFrameIdx;
//    m_endFrameIdx = endFrameIdx;
//}

//vector<float> blAtConnection::stateStart(){
//    return m_stateStart;
//}

//vector<float> blAtConnection::stateEnd(){
//    return m_stateEnd;
//}

//int blAtConnection::startFrameIdx(){
//    return m_startFrameIdx;
//}

//int blAtConnection::endFrameIdx(){
//    return m_endFrameIdx;
//}

//float blAtConnection::cost(){
//    return m_cost;
//}

//void blAtConnection::setCost(float cost){
//    m_cost = cost;
//}
