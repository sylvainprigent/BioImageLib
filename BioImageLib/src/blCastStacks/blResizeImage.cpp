/// \file blResizeImage.cpp
/// \brief blResizeImage class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blResizeImage.h"

blResizeImage::blResizeImage() : blProcess(){
    m_inputImage = NULL;
    m_outputImage = NULL;
}

blResizeImage::~blResizeImage(){

}

std::string blResizeImage::name(){
    return "blResizeImage";
}

void blResizeImage::setInput(blImage* image){
    m_inputImage = image;
}

void blResizeImage::run(){

    if (m_inputImage->imageType() == blImage::TypeInt2D){
        m_outputImage = resizeImage2D<Int2DImage>(m_inputImage->itkInt2DImagePointer());
    }
    else if(m_inputImage->imageType() == blImage::TypeFloat2D){
        m_outputImage = resizeImage2D<Float2DImage>(m_inputImage->itkFloat2DImagePointer());
    }
    else if (m_inputImage->imageType() == blImage::TypeIntColor2D){
        m_outputImage = resizeImage2D<IntColor2DImage>(m_inputImage->itkIntColor2DImagePointer());
    }
    else if (m_inputImage->imageType() == blImage::TypeFloatColor2D){
        m_outputImage = resizeImage2D<FloatColor2DImage>(m_inputImage->itkFloatColor2DImagePointer());
    }
    else{
        std::string message = "blResizeImage not implementer for " + m_inputImage->imageType();
        throw blException(message.c_str());
    }
}

blImage* blResizeImage::output(){
    return m_outputImage;
}

void blResizeImage::setSizeY(unsigned int sizeY){
    m_sizeY = sizeY;
}
