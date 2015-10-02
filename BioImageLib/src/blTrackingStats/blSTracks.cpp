/// \file blSTracks.cpp
/// \brief blSTracks class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#include "blSTracks.h"
#include <iostream>
#include <sstream>
#include <fstream>

#include <blCore>

using namespace std;

blSTracks::blSTracks(){

}

blSTracks::blSTracks(std::vector<blSTrack*> tracks){
    m_tracks = tracks;
}

// setters
void blSTracks::addTrack(blSTrack* track){
    m_tracks.push_back(track);
}

void blSTracks::setTracks(std::vector<blSTrack*> tracks){
    m_tracks = tracks;
}


std::vector<blSTrack*> blSTracks::tracks(){
    return m_tracks;
}

int blSTracks::tracksNumber(){
    return m_tracks.size();
}

std::vector<float> blSTracks::averageDistance(int startFrameIdx, int endFrameIdx){

    std::vector<float> average; average.resize(endFrameIdx- startFrameIdx +1);
    std::vector<float> count; count.resize(endFrameIdx- startFrameIdx +1);
    for(int f = 0 ; f < endFrameIdx- startFrameIdx +1 ; ++f ){
        count[f] = 0;
    }
    for(int i=0 ; i < m_tracks.size() ; ++i){
        blSTrack* track = m_tracks[i];
        std::cout << "blSTracks::averageDistance loop " << i << std::endl;
        track->calculateTotalDistance(startFrameIdx, endFrameIdx);
        std::cout << "blSTracks::averageDistance loop total dist " << i << std::endl;
        for(int f=startFrameIdx ; f <= endFrameIdx ; ++f){

            // search a point at the frame idx f, and add it to the calculation if exists
            std::cout << "curent track size = " <<  track->size()  << std::endl;
            for (int p = 0 ; p < track->size() ; ++p){

                blSPoint* point = track->pointAt(p);
                if (point->frameIdx() == f){
                    float dist = point->distance();
                    if (dist >= 0){
                        average[f-startFrameIdx] += dist;
                        count[f-startFrameIdx] ++;
                    }
                    break;
                }
            }
        }
    }
    for(int f = 0 ; f < endFrameIdx- startFrameIdx +1 ; ++f ){
        average[f] /= count[f];
    }
    return average;
}

std::vector<float> blSTracks::averageSpeed(int startFrameIdx, int endFrameIdx, float frameRate){
    std::vector<float> average; average.resize(endFrameIdx- startFrameIdx +1);
    std::vector<float> count; count.resize(endFrameIdx- startFrameIdx +1);
    for(int f = 0 ; f < endFrameIdx- startFrameIdx +1 ; ++f ){
        count[f] = 0;
    }
    for(int i=0 ; i < m_tracks.size() ; ++i){
        blSTrack* track = m_tracks[i];
        track->calculateAverageSpeed(startFrameIdx, endFrameIdx, frameRate);
        for(int f=startFrameIdx ; f <= endFrameIdx ; ++f){
            // search a point at the frame idx f, and add it to the calculation if exists
            for (int p = 0 ; p < track->size() ; ++p){
                blSPoint* point = track->pointAt(p);
                if (point->frameIdx() == f){
                    float dist = point->localSpeed();
                    if (dist >= 0){
                        average[f-startFrameIdx] += dist;
                        count[f-startFrameIdx] ++;
                    }
                    break;
                }
            }
        }
    }
    for(int f = 0 ; f < endFrameIdx- startFrameIdx +1 ; ++f ){
        average[f] /= count[f];
    }
    return average;
}

std::vector<float> blSTracks::averageDistanceToRef(float refX, float refY, int startFrameIdx, int endFrameIdx){
    std::vector<float> average; average.resize(endFrameIdx- startFrameIdx +1);
    std::vector<float> count; count.resize(endFrameIdx- startFrameIdx +1);
    for(int f = 0 ; f < endFrameIdx- startFrameIdx +1 ; ++f ){
        count[f] = 0;
    }
    for(int i=0 ; i < m_tracks.size() ; ++i){
        blSTrack* track = m_tracks[i];
        track->calculateAverageDistanceToRefPoint(refX, refY, startFrameIdx, endFrameIdx);
        for(int f=startFrameIdx ; f <= endFrameIdx ; ++f){
            // search a point at the frame idx f, and add it to the calculation if exists
            for (int p = 0 ; p < track->size() ; ++p){
                blSPoint* point = track->pointAt(p);
                if (point->frameIdx() == f){
                    float dist = point->distanceToRefPoint();
                    if (dist >= 0){
                        average[f-startFrameIdx] += dist;
                        count[f-startFrameIdx] ++;
                    }
                    break;
                }
            }
        }
    }
    for(int f = 0 ; f < endFrameIdx- startFrameIdx +1 ; ++f ){
        average[f] /= count[f];
    }
    return average;
}


std::vector<float> blSTracks::groupeDistanceDist(int startFrame, int endFrame){
    std::vector<float> samples; samples.resize(m_tracks.size());
    for(int t = 0 ; t < m_tracks.size() ; ++t){
        samples[t] = m_tracks[t]->calculateTotalDistance(startFrame, endFrame);
    }
    return samples;
}

blDistributionInfo blSTracks::groupeDistanceStats(int startFrame, int endFrame){

    std::vector<float> samples = this->groupeDistanceDist(startFrame, endFrame);

    blDistributionInfo info;
    info.setSamples(samples);
    info.run();
    return info;
}

std::vector<float> blSTracks::groupeStartToEndDistanceDist(int startFrame, int endFrame){
    std::vector<float> samples; samples.resize(m_tracks.size());
    for(int t = 0 ; t < m_tracks.size() ; ++t){
        samples[t] = m_tracks[t]->calculateStartEndDistance(startFrame, endFrame);
    }
    return samples;
}

blDistributionInfo blSTracks::groupeStartToEndDistanceStats(int startFrame, int endFrame){

    std::vector<float> samples = this->groupeStartToEndDistanceDist(startFrame, endFrame);

    blDistributionInfo info;
    info.setSamples(samples);
    info.run();
    return info;
}

std::vector<float> blSTracks::groupeSpeedDist(int startFrame, int endFrame, float frameRate){
    std::vector<float> samples; samples.resize(m_tracks.size());
    for(int t = 0 ; t < m_tracks.size() ; ++t){
        samples[t] = m_tracks[t]->calculateAverageSpeed(startFrame, endFrame, frameRate);
    }
    return samples;
}

blDistributionInfo blSTracks::groupeSpeedStats(int startFrame, int endFrame, float frameRate){

    std::vector<float> samples = this->groupeSpeedDist(startFrame, endFrame, frameRate);

    blDistributionInfo info;
    info.setSamples(samples);
    info.run();
    return info;
}

std::vector<float> blSTracks::groupeDistanceToRefDist(float refX, float refY, int startFrame, int endFrame){
    std::vector<float> samples; samples.resize(m_tracks.size());
    for(int t = 0 ; t < m_tracks.size() ; ++t){
        samples[t] = m_tracks[t]->calculateAverageDistanceToRefPoint(refX, refY, startFrame, endFrame);
    }
    return samples;
}

blDistributionInfo blSTracks::groupeDistanceToRefStats(float refX, float refY, int startFrame, int endFrame){

    std::vector<float> samples = this->groupeDistanceToRefDist(refX, refY, startFrame, endFrame);

    blDistributionInfo info;
    info.setSamples(samples);
    info.run();
    return info;
}

void blSTracks::averageDistanceToEachOthers(int startFrameIdx, int endFrameIdx){


    for(int i=0 ; i < m_tracks.size() ; ++i){

        blSTrack* curentTrack = m_tracks[i];
        if (curentTrack->size() > startFrameIdx){
            int eFrameIdx = endFrameIdx;
            if (endFrameIdx >= curentTrack->size()){
                eFrameIdx = curentTrack->size()-1;
            }
            this->averageDistanceToOthers(curentTrack, startFrameIdx, eFrameIdx);
        }
    }
}

std::vector<float> blSTracks::averageDistanceToOthers(blSTrack* curentTrack, int startFrameIdx, int endFrameIdx){


    std::vector<float> average; average.resize(endFrameIdx- startFrameIdx +1);

    for(int f = startFrameIdx ; f <= endFrameIdx ; ++f){

        blSPoint *curentPoint = curentTrack->pointAt(f);
        int frameIdx = curentPoint->frameIdx();
        float dist = 0;
        int count = 0;
        // get the distance to each other tracks at the same frame idx
        for(int i=0 ; i < m_tracks.size() ; ++i){
            blSTrack* track = m_tracks[i];
            for (int p = 0 ; p < track->size() ; ++p){
                blSPoint* point = track->pointAt(p);
                if (point->frameIdx() == frameIdx){
                    dist += sqrt( pow(point->x() - curentPoint->x(),2.0) + pow(point->y() - curentPoint->y(),2.0));
                    count++;
                    break;
                }
            }
        }
        dist = dist/float(count-1);
        curentPoint->setDistanceToOthers(dist);
        average[f-startFrameIdx] = dist;
    }

    return average;
}

// partition groups
vector<blSTracks*> blSTracks::makeGroups(vector<vector<float> > gpoints){

    std::vector<blSTracks*> tracksGroups; tracksGroups.resize(gpoints.size());
    for (int g = 0 ; g < gpoints.size() ; ++g){
        tracksGroups[g] = new blSTracks();
    }

    for (int t = 0 ; t < m_tracks.size() ; ++t){
        //std::cout << "sort track: " << t << std::endl;
        // get track first point
        blSTrack *track = m_tracks[t];
        std::cout << "get point: " << std::endl;
        blSPoint* point = track->pointAt(0);

        // calculate closest group
        int minPos = 0;
        float minDist = float(999999999999.0);
        float dist;
        for (int g = 0 ; g < gpoints.size() ; ++g ){
            dist = ( pow(point->y() - gpoints[g][1],2) + pow(point->x() - gpoints[g][0],2));
            if (dist < minDist){
                minDist = dist;
                minPos = g;
            }
        }
        // add the track to the closest group
        tracksGroups[minPos]->addTrack(track);
    }
    return tracksGroups;
}

void blSTracks::saveTracksGroupsToFile(std::vector<blSTracks*> tracksGroups, std::string rootFileName){

    for (int g = 0 ; g < tracksGroups.size() ; ++g){
        std::cout << "save tracks to : " << rootFileName + "_" + blCast::to_string(g) + ".txt" << std::endl;
        this->saveTracksToTxtFile(tracksGroups[g]->tracks(), rootFileName + "_" + blCast::to_string(g) + ".txt" );
    }
}

// load
void blSTracks::loadTracksFromTxtFile(std::string fileName){


    // 1- Open file
    ifstream file( fileName.c_str() );

    if ( file )
    {
        string line;
        string track = "track";
        string readedword; float val;
        vector<vector<float> > curentpoints;
        int pass = -1;
        while ( std::getline( file, line ) )
        {
            pass++;
            std::size_t found = line.find(track);
            if (found!=std::string::npos){
                if (pass > 0){
                    addTrack(curentpoints);
                    curentpoints.clear();
                }
            }
            else{

                // split the line
                vector<float> inter;
                stringstream stream(line);
                while( getline(stream, readedword, '\t')){
                    val = float(std::stod(readedword));
                    //blCast::from_string(readedword, val);
                    inter.push_back(val);
                }
                curentpoints.push_back(inter);
            }
        }
        addTrack(curentpoints);
        file.close();
    }
    else{
        cerr << "blSTracks::loadTracksFromTxtFile: Unable to open the file !" << fileName << endl;
    }
}

void blSTracks::addTrack(std::vector<std::vector<float> > curentpoints){
    blSTrack *track = new blSTrack;
    float x, y;
    int t;
    for (unsigned int i = 0 ; i < curentpoints.size() ; ++i){

        if (curentpoints[i].size() >= 3){
            x = curentpoints[i][0];
            y = curentpoints[i][1];
            t = int(curentpoints[i][curentpoints[i].size()-1]);

            blSPoint* point = new blSPoint(t,x,y);
            track->addPoint(point);
        }
    }
    m_tracks.push_back(track);
}

void blSTracks::saveTracksAllInfoToTxtFile(std::vector<blSTrack *> tracks, string fileName){

    ofstream file(fileName.c_str(), ios::out | ios::trunc);
    if(file)
    {
        // 2- Save each track
        for (unsigned int i = 0 ; i < tracks.size() ; ++i){
            file << "track " << i << endl;
            for (int p = 0 ; p < tracks[i]->size() ; ++p ){
                blSPoint* pointInter = tracks.at(i)->pointAt(p);
                file << pointInter->x() << "\t" << pointInter->y() << "\t" << pointInter->frameIdx() << "\t"
                     << pointInter->distance() << "\t"
                     << pointInter->localSpeed() << "\t"
                     << pointInter->distanceToRefPoint() << "\t"
                     << pointInter->distanceToOthers() << "\t"
                     << endl;
            }
        }
        file.close();
    }
    else{
        cerr << "Impossible to open the file !" << fileName << endl;
    }
}

void blSTracks::saveTracksToTxtFile(string fileName){

    // 1- Open file
    ofstream file(fileName.c_str(), ios::out | ios::trunc);
    if(file)
    {
        // 2- Save each track
        for (unsigned int i = 0 ; i < m_tracks.size() ; ++i){
            file << "track " << i << endl;
            for (int p = 0 ; p < m_tracks[i]->size() ; ++p ){
                blSPoint* pointInter = m_tracks.at(i)->pointAt(p);
                file << pointInter->x() << "\t" << pointInter->y() << "\t" << pointInter->frameIdx() << endl;
            }
        }
        file.close();
    }
    else{
        cerr << "Impossible to open the file !" << fileName << endl;
    }
}

void blSTracks::saveTracksToTxtFile(std::vector<blSTrack *> tracks, string fileName){

    // 1- Open file
    ofstream file(fileName.c_str(), ios::out | ios::trunc);
    if(file)
    {
        // 2- Save each track
        for (unsigned int i = 0 ; i < tracks.size() ; ++i){
            file << "track " << i << endl;
            for (int p = 0 ; p < tracks[i]->size() ; ++p ){
                blSPoint* pointInter = tracks.at(i)->pointAt(p);
                file << pointInter->x() << "\t" << pointInter->y() << "\t" << pointInter->frameIdx() << endl;
            }
        }
        file.close();
    }
    else{
        cerr << "Impossible to open the file !" << fileName << endl;
    }
}
