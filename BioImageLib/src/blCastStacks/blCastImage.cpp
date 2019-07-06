/// \file blCastImage.cpp
/// \brief blCastImage class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blCastImage.h"


blCastImage::blCastImage() : blProcess(){
    m_inputImage = nullptr;
    m_outputImage = nullptr;
}

blCastImage::~blCastImage(){

}

std::string blCastImage::name(){
    return "blCastImage";
}

void blCastImage::setInput(blImage *image){
    m_inputImage = image;
}

blImage* blCastImage::output(){
    return m_outputImage;
}
