/// \file blColorImageToGray.cpp
/// \brief blColorImageToGray class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blColorImageToGray.h"
#include "blColorImageToLuminance.h"
#include "blExtractComponentColorImage.h"


const std::string blColorImageToGray::Luminance = "Luminance";
const std::string blColorImageToGray::Red = "Red";
const std::string blColorImageToGray::Green = "Green";
const std::string blColorImageToGray::Blue = "Blue";


blColorImageToGray::blColorImageToGray() : blProcess(){
    m_inputImage = NULL;
    m_outputImage = NULL;
}

blColorImageToGray::~blColorImageToGray(){

}

std::string blColorImageToGray::name(){
    return "blColorImageToGray";
}

void blColorImageToGray::setInput(blImage* image){
    m_inputImage = image;
}

void blColorImageToGray::run(){

    if (m_transformationName == blColorImageToGray::Luminance){
        blColorImageToLuminance converter;
        converter.setInput(m_inputImage);
        converter.run();
        m_outputImage = converter.output();
    }
    else if (m_transformationName == blColorImageToGray::Red){
        blExtractComponentColorImage converter;
        converter.setInput(m_inputImage);
        converter.setComponentIndex(0);
        converter.run();
        m_outputImage = converter.output();
    }
    else if (m_transformationName == blColorImageToGray::Green){
        blExtractComponentColorImage converter;
        converter.setInput(m_inputImage);
        converter.setComponentIndex(1);
        converter.run();
        m_outputImage = converter.output();
    }
    else if (m_transformationName == blColorImageToGray::Blue){
        blExtractComponentColorImage converter;
        converter.setInput(m_inputImage);
        converter.setComponentIndex(2);
        converter.run();
        m_outputImage = converter.output();
    }
    else{
        throw blException("blColorImageToGray: the specified transformation does not exists");
    }
}

blImage* blColorImageToGray::output(){
    return m_outputImage;
}

void blColorImageToGray::setTransformation(std::string transformationName){
    m_transformationName = transformationName;
}
