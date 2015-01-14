/// \file blMinMaxCurvatureFlowFilter.cpp
/// \brief blMinMaxCurvatureFlowFilter class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blMinMaxCurvatureFlowFilter.h"
#include <blCastStacks>

blMinMaxCurvatureFlowFilter::blMinMaxCurvatureFlowFilter() : blFilter(){
    m_iterations = 5;
    m_timeStep = 0.125;
}

blMinMaxCurvatureFlowFilter::~blMinMaxCurvatureFlowFilter(){

}

std::string blMinMaxCurvatureFlowFilter::name(){
    return "blMinMaxCurvatureFlowFilter";
}

void blMinMaxCurvatureFlowFilter::setNumberOfIterations(int iterations){
    m_iterations = iterations;
}

void blMinMaxCurvatureFlowFilter::setTimeStep(float timeStep){
    m_timeStep = timeStep;
}


void blMinMaxCurvatureFlowFilter::run(){

    if (m_inputImage->imageType() == blImage::TypeInt2D){
        m_outputImage = curvature<Int2DImage, Float2DImage>(m_inputImage->itkInt2DImagePointer());
    }
    else if ( m_inputImage->imageType() == blImage::TypeInt3D){
        m_outputImage = curvature<Int3DImage, Float3DImage>(m_inputImage->itkInt3DImagePointer());
    }
    else if(m_inputImage->imageType() == blImage::TypeFloat2D){
        m_outputImage = curvature<Float2DImage, Float2DImage>(m_inputImage->itkFloat2DImagePointer());
    }
    else if(m_inputImage->imageType() == blImage::TypeFloat3D){
        m_outputImage = curvature<Float3DImage, Float3DImage>(m_inputImage->itkFloat3DImagePointer());
    }
    else if(m_inputImage->imageType() == blImage::TypeIntColor2D || m_inputImage->imageType() == blImage::TypeFloatColor2D
            || m_inputImage->imageType() == blImage::TypeIntColor3D || m_inputImage->imageType() == blImage::TypeFloatColor3D){

        // Calculate gradient
        blImage* compRed = curvatureOnOneComponent(m_inputImage, 0);
        blImage* compGreen =curvatureOnOneComponent(m_inputImage, 1);
        blImage* compBlue = curvatureOnOneComponent(m_inputImage, 2);

        // compose images
        blComposeColorImage composer;
        composer.setComposant1(compRed);
        composer.setComposant2(compGreen);
        composer.setComposant3(compBlue);
        composer.run();
        m_outputImage = composer.output();
    }
    else{
        std::string message = "The image format " + m_inputImage->imageType() + " is not suported by blMinMaxCurvatureFlowFilter filter";
        throw blException(message.c_str());
    }
}

blImage* blMinMaxCurvatureFlowFilter::curvatureOnOneComponent(blImage* input, unsigned int index){

    // extract component
    blExtractComponentColorImage extractor;
    extractor.setInput(input);
    extractor.setComponentIndex(index);
    extractor.run();
    blImage* comp = extractor.output();

    // Calculate gradient
    blMinMaxCurvatureFlowFilter filter;
    filter.setInput(comp);
    filter.setNumberOfIterations(m_iterations);
    filter.setTimeStep(m_timeStep);
    filter.run();
    return filter.output();
}
