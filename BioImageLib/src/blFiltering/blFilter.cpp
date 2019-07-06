/// \file blFilter.cpp
/// \brief blFilter class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blFilter.h"

blFilter::blFilter() : blProcess(){
    m_inputImage = nullptr;
    m_outputImage = nullptr;
}

blFilter::~blFilter(){
}

std::string blFilter::name(){
    return "blFilter";
}

void blFilter::setInput(blImage* inputImage){
    m_inputImage = inputImage;
}

blImage* blFilter::output(){
    return m_outputImage;
}
