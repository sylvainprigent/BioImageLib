/// \file blExtractComponentColorImage.cpp
/// \brief blExtractComponentColorImage class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blExtractComponentColorImage.h"

blExtractComponentColorImage::blExtractComponentColorImage() : blProcess(){

}

blExtractComponentColorImage::~blExtractComponentColorImage(){

}

std::string blExtractComponentColorImage::name(){
    return "blExtractComponentColorImage";
}

void blExtractComponentColorImage::setInput(blImage* image){
    m_inputImage = image;
}

void blExtractComponentColorImage::setComponentIndex(unsigned int index){
    m_index = index;
}

void blExtractComponentColorImage::run(){

    if (m_inputImage->imageType() == blImage::TypeIntColor2D){
        m_outputImage = extractComponent<IntColor2DImage, Int2DImage>(m_inputImage->itkIntColor2DImagePointer(), m_index);
    }
    else if (m_inputImage->imageType() == blImage::TypeFloatColor2D){
        m_outputImage = extractComponent<FloatColor2DImage, Float2DImage>(m_inputImage->itkFloatColor2DImagePointer(), m_index);
    }
    else if (m_inputImage->imageType() == blImage::TypeIntColor3D){
        m_outputImage = extractComponent<IntColor3DImage, Int3DImage>(m_inputImage->itkIntColor3DImagePointer(), m_index);
    }
    else if (m_inputImage->imageType() == blImage::TypeFloatColor3D){
        m_outputImage = extractComponent<FloatColor3DImage, Float3DImage>(m_inputImage->itkFloatColor3DImagePointer(), m_index);
    }
}

blImage* blExtractComponentColorImage::output(){
    return m_outputImage;
}
