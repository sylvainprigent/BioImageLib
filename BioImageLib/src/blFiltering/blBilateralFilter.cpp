/// \file blBilateralFilter.cpp
/// \brief blBilateralFilter class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blBilateralFilter.h"
#include <blCastStacks>

blBilateralFilter::blBilateralFilter() : blFilter(){
    m_domainSigma = 2.0;
    m_rangeSigma = 2.0;
}

blBilateralFilter::~blBilateralFilter(){

}

std::string blBilateralFilter::name(){
    return "blBilateralFilter";
}

void blBilateralFilter::setDomainSigma(float domainSigma){
    m_domainSigma = domainSigma;
}

void blBilateralFilter::setRangeSigma(float rangeSigma){
    m_rangeSigma = rangeSigma;
}


void blBilateralFilter::run(){

    if (m_inputImage->imageType() == blImage::TypeInt2D){
        m_outputImage = bilateral<Int2DImage, Float2DImage>(m_inputImage->itkInt2DImagePointer());
    }
    else if ( m_inputImage->imageType() == blImage::TypeInt3D){
        m_outputImage = bilateral<Int3DImage, Float3DImage>(m_inputImage->itkInt3DImagePointer());
    }
    else if(m_inputImage->imageType() == blImage::TypeFloat2D){
        m_outputImage = bilateral<Float2DImage, Float2DImage>(m_inputImage->itkFloat2DImagePointer());
    }
    else if(m_inputImage->imageType() == blImage::TypeFloat3D){
        m_outputImage = bilateral<Float3DImage, Float3DImage>(m_inputImage->itkFloat3DImagePointer());
    }
    else if(m_inputImage->imageType() == blImage::TypeIntColor2D || m_inputImage->imageType() == blImage::TypeFloatColor2D
            || m_inputImage->imageType() == blImage::TypeIntColor3D || m_inputImage->imageType() == blImage::TypeFloatColor3D){

        // Calculate gradient
        blImage* compRed = bilateralOnOneComponent(m_inputImage, 0);
        blImage* compGreen =bilateralOnOneComponent(m_inputImage, 1);
        blImage* compBlue = bilateralOnOneComponent(m_inputImage, 2);

        // compose images
        blComposeColorImage composer;
        composer.setComposant1(compRed);
        composer.setComposant2(compGreen);
        composer.setComposant3(compBlue);
        composer.run();
        m_outputImage = composer.output();
    }
    else{
        std::string message = "The image format " + m_inputImage->imageType() + " is not suported by blBilateralFilter filter";
        throw blException(message.c_str());
    }
}

blImage* blBilateralFilter::bilateralOnOneComponent(blImage* input, unsigned int index){

    // extract component
    blExtractComponentColorImage extractor;
    extractor.setInput(input);
    extractor.setComponentIndex(index);
    extractor.run();
    blImage* comp = extractor.output();

    // Calculate gradient
    blBilateralFilter filter;
    filter.setInput(comp);
    filter.setDomainSigma(m_domainSigma);
    filter.setRangeSigma(m_rangeSigma);
    filter.run();
    return filter.output();
}

