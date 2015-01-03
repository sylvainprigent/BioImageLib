/// \file blColorImageToLuminance.cpp
/// \brief blColorImageToLuminance class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blColorImageToLuminance.h"

blColorImageToLuminance::blColorImageToLuminance() : blProcess(){
    m_inputImage = NULL;
    m_outputImage = NULL;
}

blColorImageToLuminance::~blColorImageToLuminance(){

}

std::string blColorImageToLuminance::name(){
    return "blColorImageToLuminance";
}

void blColorImageToLuminance::setInput(blImage* image){
    m_inputImage = image;
}

void blColorImageToLuminance::run(){

    if (m_inputImage->imageType() == blImage::TypeIntColor2D){
        m_outputImage = extractLuminance<IntColor2DImage, Int2DImage>(m_inputImage->itkIntColor2DImagePointer());
    }
    else if (m_inputImage->imageType() == blImage::TypeFloatColor2D){
        m_outputImage = extractLuminance<FloatColor2DImage, Float2DImage>(m_inputImage->itkFloatColor2DImagePointer());
    }
    else if (m_inputImage->imageType() == blImage::TypeIntColor3D){
        m_outputImage = extractLuminance<IntColor3DImage, Int3DImage>(m_inputImage->itkIntColor3DImagePointer());
    }
    else if (m_inputImage->imageType() == blImage::TypeFloatColor3D){
        m_outputImage = extractLuminance<FloatColor3DImage, Float3DImage>(m_inputImage->itkFloatColor3DImagePointer());
    }
    else{
        throw blException("blColorImageToLuminance needs a color image as an input");
    }
}

blImage* blColorImageToLuminance::output(){
    return m_outputImage;
}
