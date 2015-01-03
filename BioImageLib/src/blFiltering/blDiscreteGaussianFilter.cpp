/// \file blDiscreteGaussianFilter.cpp
/// \brief blDiscreteGaussianFilter class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blDiscreteGaussianFilter.h"

blDiscreteGaussianFilter::blDiscreteGaussianFilter() : blFilter(){
    m_variance = 1.0;
}

blDiscreteGaussianFilter::~blDiscreteGaussianFilter(){

}

std::string blDiscreteGaussianFilter::name(){
    return "blDiscreteGaussianFilter";
}

void blDiscreteGaussianFilter::setVariance(float variance){
    m_variance = variance;
}


void blDiscreteGaussianFilter::run(){


    if (m_inputImage->imageType() == blImage::TypeInt2D){
        m_outputImage = discreteGaussian<Int2DImage>(m_inputImage->itkInt2DImagePointer());
    }
    else if ( m_inputImage->imageType() == blImage::TypeInt3D){
        m_outputImage = discreteGaussian<Int3DImage>(m_inputImage->itkInt3DImagePointer());
    }
    else if(m_inputImage->imageType() == blImage::TypeFloat2D){
        m_outputImage = discreteGaussian<Float2DImage>(m_inputImage->itkFloat2DImagePointer());
    }
    else if(m_inputImage->imageType() == blImage::TypeFloat3D){
        m_outputImage = discreteGaussian<Float3DImage>(m_inputImage->itkFloat3DImagePointer());
    }
    else{
        std::string message = "The image format " + m_inputImage->imageType() + " is not suported by blDiscreteGaussianFilter filter";
        throw blException(message.c_str());
    }
}

