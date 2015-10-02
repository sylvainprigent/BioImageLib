/// \file blThreshold.cpp
/// \brief blThreshold class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blThreshold.h"


blThreshold::blThreshold() : blClassifier(){
    m_lowerThreshold = 0;
    m_upperThreshold = 125;
    m_insideValue = 255;
    m_outsideValue = 0;
}

blThreshold::~blThreshold(){

}

std::string blThreshold::name(){
    return "blThreshold";
}


void blThreshold::run(){

    if (m_lowerThreshold >= m_upperThreshold){
        throw blException("blThreshold: lower threshold cannot be greater than upper threshold");
    }

    if (m_inputImage->imageType() == blImage::TypeInt2D){
        m_outputImage = threshold<Int2DImage, Int2DImage>(m_inputImage->itkInt2DImagePointer());
    }
    else if ( m_inputImage->imageType() == blImage::TypeInt3D){
        m_outputImage = threshold<Int3DImage, Int3DImage>(m_inputImage->itkInt3DImagePointer());
    }
    else if(m_inputImage->imageType() == blImage::TypeFloat2D){
        m_outputImage = threshold<Float2DImage, Int2DImage>(m_inputImage->itkFloat2DImagePointer());
    }
    else if(m_inputImage->imageType() == blImage::TypeFloat3D){
        m_outputImage = threshold<Float3DImage, Int3DImage>(m_inputImage->itkFloat3DImagePointer());
    }
    else if(m_inputImage->imageType() == blImage::TypeIntColor2D
             || m_inputImage->imageType() == blImage::TypeFloatColor2D
             || m_inputImage->imageType() == blImage::TypeIntColor3D
             || m_inputImage->imageType() == blImage::TypeFloatColor3D){
        std::string message = "blConnectedComponentAnalysis can only process gray scaled images. Your image is: " + m_inputImage->imageType();
        throw blException(message.c_str());
    }
    else{
        std::string message = "The image format " + m_inputImage->imageType() + " is not suported by blConnectedComponentAnalysis filter";
        throw blException(message.c_str());
    }
}

// parameters
void blThreshold::setLowBoundary(const float& min){
    m_lowerThreshold = min;
}

void blThreshold::setHighBoundary(const float& max){
    m_upperThreshold = max;
}

void blThreshold::setBoundaries(const float& min,const float& max){
    m_lowerThreshold = min;
    m_upperThreshold = max;
}

void blThreshold::setInsideValue(const float& inside){
    m_insideValue = inside;
}

void blThreshold::setOutsideValue(const float& outside){
    m_outsideValue = outside;
}

void blThreshold::setValues(const float& inside, const float& outside){
    m_insideValue = inside;
    m_outsideValue = outside;
}
