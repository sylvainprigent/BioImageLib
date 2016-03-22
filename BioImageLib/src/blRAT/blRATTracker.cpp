/// \file blRATTracker.cpp
/// \brief blRATTracker class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#include "blRATTracker.h"

#include <lemon/preflow.h>
#include <lemon/smart_graph.h>
#include <lemon/lgf_reader.h>
#include <lemon/lgf_writer.h>
#include <lemon/capacity_scaling.h>
#include <lemon/list_graph.h>
#include "blRand/blRandomGeneratorPM.h"
#include "blCore/blCast.h"

using namespace std;

blRATTracker::blRATTracker(blRATCostManager *costManager, unsigned int framesNumber, blRATObjectsManager *objectsManager){
    m_costManager = costManager;
    m_framesNumber = framesNumber;
    m_objectsManager = objectsManager;
}

blRATTracker::~blRATTracker(){

}

void blRATTracker::initialize(){

}

void blRATTracker::setMaxMove(float maxMove){
    m_maxMove = maxMove;
}

void blRATTracker::setIntegerCoeff(float integerCoeff){
    m_integerCoeff = integerCoeff;
}


void blRATTracker::setIterationsVerboseRatio(int iterationsVerboseRatio){
    m_iterationsVerboseRatio = iterationsVerboseRatio;
}

std::vector<blRATTrack*> blRATTracker::getTracks(){
    return m_tracks;
}

void blRATTracker::run(){

    for(int frame = 1 ; frame < m_framesNumber ; ++frame){

        cout << "process frame " << frame << endl;
        // get the optimal set of connections for the curent frame
        oneStep(frame);

        cout << "addConnectionsToTracks " << frame << endl;
        // copy the selected connections to the tracks list
        this->addConnectionsToTracks(m_config, frame);
    }

}

void blRATTracker::saveTracksToTxtFile(std::string fileURL){

    // 1- Open file
    ofstream file(fileURL.c_str(), ios::out | ios::trunc);
    if(file)
    {
        // 2- Save each track
        for (unsigned int i = 0 ; i < m_tracks.size() ; ++i){
            file << "track " << i << endl;
            for (unsigned int st = 0 ; st < m_tracks[i]->getTrackSize() ; ++st ){
                blRATState *state = m_tracks[i]->getStateAt(st);

                int frameIdx = m_tracks[i]->getFrameIdxAt(st);
                for (unsigned int s = 0 ; s < state->size() ; ++s){
                    file << state->getStateAt(s) << "\t";
                }
                file << frameIdx << endl;
            }
        }
        file.close();
    }
    else{
        std::cerr << "Impossible to open the file !" << fileURL << std::endl;
    }
}

void blRATTracker::addConnectionsToTracks(std::vector<blRATConnection*> connections, unsigned int frameIdx){

    bool found;
    for (int i = 0 ; i < connections.size() ; ++i){

        blRATState* connState = connections[i]->getStartState();
        found = false;
        for (int t = 0 ; t < m_tracks.size() ; ++t){
            blRATState* trackstate = m_tracks[t]->getLastState();
            if (trackstate->getStateAt(0) == connState->getStateAt(0) && trackstate->getStateAt(1) == connState->getStateAt(1)){
                m_tracks[t]->addState(connections[i]->getEndState(), frameIdx);
                found = true;
                break;
            }
        }
        if (!found){
            blRATTrack* newTrack = new blRATTrack(connState, frameIdx-1);
            newTrack->addState(connections[i]->getEndState(), frameIdx);
            m_tracks.push_back(newTrack);
        }
    }
}

blRATTrack* blRATTracker::findAssociatedTrack(blRATConnection* newConnection){

    blRATState* connState = newConnection->getStartState();
    for (int t = 0 ; t < m_tracks.size() ; ++t){
        blRATState* trackstate = m_tracks[t]->getLastState();
        if (trackstate->getStateAt(0) == connState->getStateAt(0) && trackstate->getStateAt(1) == connState->getStateAt(1)){
            return m_tracks[t];
        }
    }
   return NULL;
}

void blRATTracker::oneStep(unsigned int frameIdx){

    //cout << "oneStep begin" << endl;
    this->findAllPossibleConnections(frameIdx);

    unsigned int maxIter = 1000;
    unsigned int iter = 0;
    unsigned int nbConnections = 50;
    while (iter < maxIter){
        this->generateNewRandConfiguration(nbConnections);
        this->cut(iter, maxIter);
    }
    this->addConnectionsToTracks(m_config, frameIdx);
    //cout << "oneStep end" << endl;
}

void blRATTracker::findAllPossibleConnections(unsigned int frameIdx){

    //cout << "findAllPossibleConnections begin" << endl;
    m_candidatedConnections.clear();
    std::vector<blRATState*> objectsPrevious = m_objectsManager->getObjectsAt(frameIdx-1);
    std::vector<blRATState*> objects = m_objectsManager->getObjectsAt(frameIdx);
    //std::cout << "objects numbers previous " << objectsPrevious.size() << std::endl;
    //std::cout << "objects numbers this " << objects.size() << std::endl;
    float xp, yp, x, y, euclidean;
    for(int op = 0 ; op < objectsPrevious.size() ; ++op){
        xp = objectsPrevious[op]->getStateAt(0);
        yp = objectsPrevious[op]->getStateAt(1);
        for(int o = 0 ; o < objects.size() ; ++o ){
            x = objects[o]->getStateAt(0);
            y = objects[o]->getStateAt(1);

            euclidean = sqrt( pow(xp-x, 2) + pow(yp - y, 2));
            if (euclidean < m_maxMove){
                blRATConnection* newConnection = new blRATConnection;
                newConnection->setStartState(objectsPrevious[op], frameIdx-1);
                newConnection->setEndState(objects[o], frameIdx);
                m_costManager->calculateCost(newConnection, this->findAssociatedTrack(newConnection));
                /// \todo maybe the cost should be calculated here
                m_candidatedConnections.push_back(newConnection);
            }
        }
    }
    //cout << "findAllPossibleConnections end" << endl;
}

bool blRATTracker::isConflict(blRATConnection* conn1, blRATConnection* conn2){

    if (conn1->getStartState()->getStateAt(0) == conn2->getStartState()->getStateAt(0)
            && conn1->getStartState()->getStateAt(1) == conn2->getStartState()->getStateAt(1)){
        return true;
    }
    if (conn1->getEndState()->getStateAt(0) == conn2->getEndState()->getStateAt(0)
            && conn1->getEndState()->getStateAt(1) == conn2->getEndState()->getStateAt(1)){
        return true;
    }
    return false;
}

bool blRATTracker::isPrimConflict(blRATConnection* connection){

    for (int i = 0 ; i < m_configPrim.size() ; ++i){
        if (this->isConflict(connection, m_configPrim[i])){
            return true;
        }
    }
    return false;
}


void blRATTracker::generateNewRandConfiguration(unsigned int nbConnections){
    int k = 0,n = 0, n_max = nbConnections;

    //std::cout << "blRATTracker::generateNewRandConfiguration begin" << std::endl;
    float energy;
    while (k < nbConnections && n<n_max){
        //1- Generate rand shape
        //std::cout << "1- Generate rand shape" << std::endl;
        //std::cout << "m_candidatedConnections.size()=" << m_candidatedConnections.size() << std::endl;
        int randPos = int(blRandomGeneratorPM::rand()*float(m_candidatedConnections.size()));
        //std::cout << "1- Generate rand shape rand pos = " << randPos << std::endl;
        blRATConnection* newConnection = m_candidatedConnections[randPos];

        // 2- Calculate data term
        //std::cout << "2- Calculate data term" << std::endl;
        energy = newConnection->energy();

        //std::cout << std::endl << "2- Calculate data term done" << std::endl;

        if (energy > 0){
            //std::cout << "data term negative" << std::endl;
            n++;
            //std::cout << "data term negative done" << std::endl;
        }
        else{
            //std::cout << "is interaction" << std::endl;
            if (this->isPrimConflict(newConnection)){
                //std::cout << "delete" << std::endl;
                n++;
            }
            else{
                //std::cout << "add" << std::endl;
                m_configPrim.push_back(newConnection);
                n=0;
                k++;
            }
        }
    }
    //std::cout << "blMppAlgorithmMBCR::generateNewRandConfiguration end" << std::endl;
}

void blRATTracker::cut(unsigned int iter, unsigned int maxIter){

    // create graph
    lemon::ListDigraph g;
    lemon::ListDigraph::ArcMap<int> cap(g);

    // add source and sink
    vector<int> configIdxs;
    lemon::ListDigraph::Node source, sink;
    lemon::ListDigraph::Arc arc;
    source = g.addNode();
    sink = g.addNode();
    int countor = -1;

    // Add conf Shapes to graph
    lemon::ListDigraph::Node curentNode;
    configIdxs.resize(m_config.size());
    float w;
    for (int i = 0 ; i < int(m_config.size()) ; ++i){
        countor++;
        configIdxs[i] = countor;
        curentNode = g.addNode();
        // capacity to sink
        w = -m_config[i]->energy();
        arc = g.addArc(curentNode, sink);
        cap[arc] = int(m_integerCoeff*(w));
    }
    int m_configSize = countor+1;

    // Add conf prim Shapes to graph
    int countorPrim = -1;
    lemon::ListDigraph::Node nodeConf, nodeConfPrim;
    for (int j=0 ; j < int(m_configPrim.size()) ; ++j){
        countorPrim++;
        // add the node with weigth
        g.addNode();
        // capacity to sink
        nodeConfPrim = g.nodeFromId( countorPrim + countor+1+2 );
        w = -m_configPrim[j]->energy();
        arc = g.addArc(source, nodeConfPrim);
        cap[arc] = int(m_integerCoeff*(w));

        // add weigth with overlaping Shapes of m_ShapesStates
        float interactionValue = 9999.9;
        for (int o = 0 ; o < m_config.size() ; ++o){
            if (this->isConflict(m_configPrim[j], m_config[o])){
                nodeConf = g.nodeFromId( configIdxs[o]+2 );
                arc = g.addArc(nodeConfPrim, nodeConf);
                cap[arc] = int(-float(m_integerCoeff)*interactionValue);
            }
        }
    }

    // verbose
    if (iter%m_iterationsVerboseRatio == 0){
        this->notify("Shape count = " + blCast::to_string(m_configSize));
        this->notifyProgress( int(100*(float(iter)/float(maxIter) )) );
    }

    // optimisation
    lemon::Preflow<lemon::ListDigraph> preflow(g, cap, source, sink);
    preflow.runMinCut();

    // get output
    int m_countorConf = -1;
    std::vector<blRATConnection*> newConfig;
    for (unsigned int i = 0 ; i < m_config.size() ; ++i){
        m_countorConf++;
        if ( !preflow.minCut (g.nodeFromId(m_countorConf+2)) ){
            newConfig.push_back(m_config[i]);
        }
    }

    for (unsigned int j = 0 ; j < m_configPrim.size() ; ++j){
        m_countorConf++;
        if ( preflow.minCut (g.nodeFromId(m_countorConf+2)) ){
            newConfig.push_back(m_configPrim[j]);
        }
    }
    m_countorConf++;

    m_config.clear();
    m_config = newConfig;
    m_configPrim.clear();
}
