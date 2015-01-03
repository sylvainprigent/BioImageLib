/// \file blRecursiveGaussianFilter.cpp
/// \brief blRecursiveGaussianFilter class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blRecursiveGaussianFilter.h"

blRecursiveGaussianFilter::blRecursiveGaussianFilter() : blFilter(){
    m_sigma = 1.0;
}

blRecursiveGaussianFilter::~blRecursiveGaussianFilter(){

}

std::string blRecursiveGaussianFilter::name(){
    return "blRecursiveGaussianFilter";
}

void blRecursiveGaussianFilter::setSigma(float sigma){
    m_sigma = sigma;
}


void blRecursiveGaussianFilter::run(){


    if (m_inputImage->imageType() == blImage::TypeInt2D){
        m_outputImage = recursiveGaussian<Int2DImage>(m_inputImage->itkInt2DImagePointer());
    }
    else if ( m_inputImage->imageType() == blImage::TypeInt3D){
        m_outputImage = recursiveGaussian<Int3DImage>(m_inputImage->itkInt3DImagePointer());
    }
    else if(m_inputImage->imageType() == blImage::TypeFloat2D){
        m_outputImage = recursiveGaussian<Float2DImage>(m_inputImage->itkFloat2DImagePointer());
    }
    else if(m_inputImage->imageType() == blImage::TypeFloat3D){
        m_outputImage = recursiveGaussian<Float3DImage>(m_inputImage->itkFloat3DImagePointer());
    }
    else if(m_inputImage->imageType() == blImage::TypeIntColor2D){
        m_outputImage = recursiveGaussian<IntColor2DImage>(m_inputImage->itkIntColor2DImagePointer());
    }
    else if (m_inputImage->imageType() == blImage::TypeFloatColor2D){
        m_outputImage = recursiveGaussian<FloatColor2DImage>(m_inputImage->itkFloatColor2DImagePointer());
    }
    else if (m_inputImage->imageType() == blImage::TypeIntColor3D){
        m_outputImage = recursiveGaussian<IntColor3DImage>(m_inputImage->itkIntColor3DImagePointer());
    }
    else if (m_inputImage->imageType() == blImage::TypeFloatColor3D){
        m_outputImage = recursiveGaussian<FloatColor3DImage>(m_inputImage->itkFloatColor3DImagePointer());
    }
    else{
        std::string message = "The image format " + m_inputImage->imageType() + " is not suported by blRecursiveGaussianFilter filter";
        throw blException(message.c_str());
    }
}

