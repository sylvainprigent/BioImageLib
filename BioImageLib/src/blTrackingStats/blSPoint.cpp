/// \file blSPoint.h
/// \brief blSPoint class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#include "blSPoint.h"
#include <math.h>

blSPoint::blSPoint(int frameIdx, float x, float y){
    m_x = x;
    m_y = y;
    m_frameIdx = frameIdx;

    m_distance = 0.0;
    m_localSpeed = 0.0;
    m_distanceToRefPoint = 0.0;
    m_refPointX = 0.0;
    m_refPointY = 0.0;
}

// calculation
float blSPoint::calculateLocalSpeed(blSPoint* pointBefore, float frameRate){

    m_localSpeed = float(sqrt( pow(double(pointBefore->x() - m_x),2.0) + pow(double(pointBefore->y() - m_y),2.0)) / frameRate);
    return m_localSpeed;
}

float blSPoint::calculateDistance(blSPoint *pointBefore){

    m_distance = float(sqrt( pow(double(pointBefore->x() - m_x),2.0) + pow(double(pointBefore->y() - m_y),2.0)));
    return m_distance;
}

float blSPoint::calculateDistanceToRefPoint(float refX, float refY){
    m_refPointX = refX;
    m_refPointY = refY;

    m_distanceToRefPoint = float(sqrt( pow( double(m_y - refY), 2.0) + pow( double(m_x - refX), 2.0) ));
    return m_distanceToRefPoint;
}

// setters
void blSPoint::setDistanceToOthers(float dist){
    m_distanceToOthers = dist;
}

// getters
float blSPoint::distance(){
    return m_distance;
}

float blSPoint::localSpeed(){
    return m_localSpeed;
}

float blSPoint::distanceToRefPoint(){
    return m_distanceToRefPoint;
}

float blSPoint::refPointX(){
    return m_refPointX;
}

float blSPoint::refPointY(){
    return m_refPointY;
}

float blSPoint::x(){
    return m_x;
}

float blSPoint::y(){
    return m_y;
}

int blSPoint::frameIdx(){
    return m_frameIdx;
}

float blSPoint::distanceToOthers(){
    return m_distanceToOthers;
}
