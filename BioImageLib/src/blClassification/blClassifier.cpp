/// \file blClassifier.cpp
/// \brief blClassifier class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2013

#include "blClassifier.h"

blClassifier::blClassifier() : blProcess(){
    m_inputImage = NULL;
    m_outputImage = NULL;
}

blClassifier::~blClassifier(){

}

std::string blClassifier::name(){
    return "blClassifier";
}

void blClassifier::setInput(blImage* inputImage){
    m_inputImage = inputImage;
}

blImage* blClassifier::output(){
    return m_outputImage;
}
