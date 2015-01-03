/// \file blBinomialBlurFilter.cpp
/// \brief blBinomialBlurFilter class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blBinomialBlurFilter.h"
#include <blCastStacks>

blBinomialBlurFilter::blBinomialBlurFilter() : blFilter(){
    m_repetitions = 1;
}

blBinomialBlurFilter::~blBinomialBlurFilter(){

}

std::string blBinomialBlurFilter::name(){
    return "blBinomialBlurFilter";
}

void blBinomialBlurFilter::setRepetitions(int repetitions){
    m_repetitions = repetitions;
}


void blBinomialBlurFilter::run(){

    if (m_inputImage->imageType() == blImage::TypeInt2D){
        m_outputImage = binomialBlur<Int2DImage>(m_inputImage->itkInt2DImagePointer());
    }
    else if ( m_inputImage->imageType() == blImage::TypeInt3D){
        m_outputImage = binomialBlur<Int3DImage>(m_inputImage->itkInt3DImagePointer());
    }
    else if(m_inputImage->imageType() == blImage::TypeFloat2D){
        m_outputImage = binomialBlur<Float2DImage>(m_inputImage->itkFloat2DImagePointer());
    }
    else if(m_inputImage->imageType() == blImage::TypeFloat3D){
        m_outputImage = binomialBlur<Float3DImage>(m_inputImage->itkFloat3DImagePointer());
    }
    else if(m_inputImage->imageType() == blImage::TypeIntColor2D || m_inputImage->imageType() == blImage::TypeFloatColor2D
            || m_inputImage->imageType() == blImage::TypeIntColor3D || m_inputImage->imageType() == blImage::TypeFloatColor3D){

        // Calculate gradient
        blImage* compRed = binomialBlurOnOneComponent(m_inputImage, 0);
        blImage* compGreen =binomialBlurOnOneComponent(m_inputImage, 1);
        blImage* compBlue = binomialBlurOnOneComponent(m_inputImage, 2);

        // compose images
        blComposeColorImage composer;
        composer.setComposant1(compRed);
        composer.setComposant2(compGreen);
        composer.setComposant3(compBlue);
        composer.run();
        m_outputImage = composer.output();
    }
    else{
        std::string message = "The image format " + m_inputImage->imageType() + " is not suported by blBinomialBlurFilter filter";
        throw blException(message.c_str());
    }
}

blImage* blBinomialBlurFilter::binomialBlurOnOneComponent(blImage* input, unsigned int index){

    // extract component
    blExtractComponentColorImage extractor;
    extractor.setInput(input);
    extractor.setComponentIndex(index);
    extractor.run();
    blImage* comp = extractor.output();

    // Calculate gradient
    blBinomialBlurFilter filter;
    filter.setInput(comp);
    filter.setRepetitions(m_repetitions);
    filter.run();
    return filter.output();
}

