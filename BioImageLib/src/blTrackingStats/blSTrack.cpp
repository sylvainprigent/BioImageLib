/// \file blSTrack.h
/// \brief blSTrack class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#include "blSTrack.h"

blSTrack::blSTrack(){

}

blSTrack::blSTrack(std::vector<blSPoint*> points){
    m_track = points;
}

// getters
void blSTrack::addPoint(blSPoint* point){
    m_track.push_back(point);
}

void blSTrack::setPoints(std::vector<blSPoint*> points){
    m_track = points;
}

blSPoint* blSTrack::pointAt(int i){
    return m_track[i];
}

// calculation
float blSTrack::calculateTotalDistance(int startFrame, int endFrame){

    float distance = 0.0;
    if (endFrame >=  m_track.size()){
        endFrame = m_track.size()-1;
    }
    if (startFrame >= m_track.size()){
        return 0.0;
    }
    if(endFrame > startFrame){
        for(int p = startFrame ; p < endFrame ; ++p){
            distance += m_track[p+1]->calculateDistance(m_track[p]);
        }
        return distance;
    }
    else{
        return 0.0;
    }
}

float blSTrack::calculateTotalDistance(){

    return this->calculateTotalDistance(0, int(m_track.size())-1);
}

float blSTrack::calculateStartEndDistance(int startFrame, int endFrame){

    if (endFrame >=  m_track.size()){
        endFrame = m_track.size()-1;
    }
    if (startFrame >= m_track.size()){
        return 0.0;
    }

    if (endFrame > startFrame){
        blSPoint* pointB = m_track[startFrame];
        blSPoint* point = m_track[endFrame];
        return float(sqrt( pow( double(pointB->x() - point->x() ),2.0 ) + pow(double(pointB->y() - point->y()),2.0)));
    }
    else{
        return 0.0;
    }
}

float blSTrack::calculateStartEndDistance(){

    return this->calculateStartEndDistance(0, int(m_track.size())-1);

}

float blSTrack::calculateAverageSpeed(float frameRate){

    return this->calculateAverageSpeed(0, int(m_track.size())-1, frameRate);
}

float blSTrack::calculateAverageSpeed(int startFrame, int endFrame, float frameRate){

    if (endFrame >=  m_track.size()){
        endFrame = m_track.size()-1;
    }
    if (startFrame >= m_track.size()){
        return 0.0;
    }
    float speed = 0.0;
    if(endFrame > startFrame){
        for(int p = startFrame ; p < endFrame ; ++p){
            speed += m_track[p+1]->calculateLocalSpeed(m_track[p], frameRate);
        }
        return speed;
    }
    else{
        return 0.0;
    }
}

float blSTrack::calculateAverageDistanceToRefPoint(float refX, float refY){
    return this->calculateAverageDistanceToRefPoint(refX, refY, 0, int(m_track.size())-1);
}

float blSTrack::calculateAverageDistanceToRefPoint(float refX, float refY, int startFrame, int endFrame){

    if (endFrame >=  m_track.size()){
        endFrame = m_track.size()-1;
    }
    if (startFrame >= m_track.size()){
        return 0.0;
    }

    float distance = 0.0;
    if(endFrame > startFrame){
       for(int p = startFrame ; p <= endFrame ; ++p){
            distance += m_track[p]->calculateDistanceToRefPoint(refX, refY);
       }
       return distance;
    }
    else{
        return 0.0;
    }
}

int blSTrack::size(){
    return int(m_track.size());
}
