/// \file blAtTrack.cpp
/// \brief blAtTrack class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2013

#include "blAtTrack.h"

blAtTrack::blAtTrack()
{
}

blAtTrack::~blAtTrack(){
    for (int i = 0 ; i < m_connections.size(); ++i){
        delete m_connections[i];
    }
}

blAtTrack::blAtTrack(blAtConnection *initialConnection){
    m_connections.push_back(initialConnection);
}

void blAtTrack::addConnection(blAtConnection *connection){
    m_connections.push_back(connection);
}

unsigned int blAtTrack::getTrackSize(){
    return m_connections.size();
}

blAtConnection* blAtTrack::getConnectionAt(int index){
    return m_connections[index];
}

blAtConnection* blAtTrack::getLastConnection(){
    return m_connections[m_connections.size()-1];
}

void blAtTrack::clear(){
    m_connections.clear();
}


