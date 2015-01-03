/// \file blVectorGradientAnisotropicDiffusionFilter.cpp
/// \brief blVectorGradientAnisotropicDiffusionFilter class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blVectorGradientAnisotropicDiffusionFilter.h"
#include <blCastStacks>

blVectorGradientAnisotropicDiffusionFilter::blVectorGradientAnisotropicDiffusionFilter() : blFilter(){
    m_iterations = 10;
    m_timeStep = 0.125;
    m_conductanceParameter = 1.5;
}

blVectorGradientAnisotropicDiffusionFilter::~blVectorGradientAnisotropicDiffusionFilter(){

}

std::string blVectorGradientAnisotropicDiffusionFilter::name(){
    return "blVectorGradientAnisotropicDiffusionFilter";
}

void blVectorGradientAnisotropicDiffusionFilter::setNumberOfIterations(int iterations){
    m_iterations = iterations;
}

void blVectorGradientAnisotropicDiffusionFilter::setTimeStep(float timeStep){
    m_timeStep = timeStep;
}

void blVectorGradientAnisotropicDiffusionFilter::setConductanceParameter(float conductanceParameter){
    m_conductanceParameter = conductanceParameter;
}

void blVectorGradientAnisotropicDiffusionFilter::run(){
    if ( m_inputImage->imageType() == blImage::TypeInt2D
         || m_inputImage->imageType() == blImage::TypeFloat2D
         || m_inputImage->imageType() == blImage::TypeInt3D
         || m_inputImage->imageType() == blImage::TypeFloat3D){

        std::string message = "blVectorGradientAnisotropicDiffusionFilter can be applied only on vector images. You image is of type " + m_inputImage->imageType();
        throw blException(message.c_str());
    }
    else if( m_inputImage->imageType() == blImage::TypeIntColor2D){
        m_outputImage = anisotropicDiffusion<IntColor2DImage, FloatColor2DImage>(m_inputImage->itkIntColor2DImagePointer());
    }
    else if( m_inputImage->imageType() == blImage::TypeFloatColor2D){
        m_outputImage = anisotropicDiffusion<FloatColor2DImage, FloatColor2DImage>(m_inputImage->itkFloatColor2DImagePointer());
    }
    else if( m_inputImage->imageType() == blImage::TypeIntColor3D) {
        m_outputImage = anisotropicDiffusion<IntColor3DImage, FloatColor3DImage>(m_inputImage->itkIntColor3DImagePointer());
    }
    else if( m_inputImage->imageType() == blImage::TypeFloatColor3D){
        m_outputImage = anisotropicDiffusion<FloatColor3DImage, FloatColor3DImage>(m_inputImage->itkFloatColor3DImagePointer());
    }
    else{
        std::string message = "The image format " + m_inputImage->imageType() + " is not suported by blVectorGradientAnisotropicDiffusionFilter filter";
        throw blException(message.c_str());
    }
}
