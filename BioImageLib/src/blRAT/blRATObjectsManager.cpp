/// \file blRATObjectsManager.cpp
/// \brief blRATObjectsManager class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#include "blRATObjectsManager.h"

#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

blRATObjectsManager::blRATObjectsManager(){
    m_stateLength = 0;
}

blRATObjectsManager::~blRATObjectsManager(){

}

unsigned int blRATObjectsManager::stateLength(){
    return m_stateLength;
}

void blRATObjectsManager::setStateLength(unsigned int length){
    m_stateLength = length;
}

void blRATObjectsManager::loadFramesStates(std::vector<std::string> filesURL){
    for (int i = 0 ; i < filesURL.size() ; ++i){
        this->loadFrameStates(filesURL[i]);
    }
}

void blRATObjectsManager::loadFrameStates(std::string fileURL){

    std::vector<blRATState*> shapes;
    std::vector<float> states;
    ifstream file(fileURL.c_str(), ios::in);
    if(file)
    {
        string curentLine, readedword;
        double val;
        while(!file.eof()){
            // Read the line
            getline(file, curentLine);
            if (!curentLine.empty()){
                states.clear();
                // split the line
                stringstream stream(curentLine);
                //std::cout << "readed line = " << curentLine << std::endl;
                while( getline(stream, readedword, '\t')){
                    //std::cout << "readed word = " << readedword << std::endl;
                    val = std::stod(readedword);
                    //blCast::from_string(readedword, val);
                    states.push_back(float(val));
                }
                m_stateLength = unsigned int(states.size());
                blRATState * st = new blRATState(states);
                shapes.push_back(st);
            }
        }
        file.close();
    }
    else{
        cerr << "blRATObjectsManager -> Impossible to open the file " << fileURL << "!" << endl;
    }

    m_states.push_back(shapes);
}


std::vector<blRATState*> blRATObjectsManager::getObjectsAt(unsigned int frameIdx){
    return m_states[frameIdx];
}

