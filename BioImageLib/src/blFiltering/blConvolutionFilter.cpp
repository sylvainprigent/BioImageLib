/// \file blConvolutionFilter.cpp
/// \brief blConvolutionFilter class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blConvolutionFilter.h"
#include <blCastStacks>

blConvolutionFilter::blConvolutionFilter() : blFilter(){
    m_normalizeKernel = true;
    m_kernel = NULL;
}

blConvolutionFilter::~blConvolutionFilter(){

}

std::string blConvolutionFilter::name(){
    return "blConvolutionFilter";
}

void blConvolutionFilter::setKernel(blImage* kernel){
    m_kernel = kernel;
}

void blConvolutionFilter::run(){

    if (!(m_kernel->imageType() == blImage::TypeFloat2D
          || m_kernel->imageType() == blImage::TypeFloat3D )){
        std::string message = "Your kernel format is " + m_kernel->imageType() + " but blConvolutionFilter only support kernel types: " + blImage::TypeFloat2D + " and " + blImage::TypeFloat3D;
        throw blException(message.c_str());
    }

    if (m_inputImage->imageType() == blImage::TypeInt2D
            || m_inputImage->imageType() == blImage::TypeInt3D){

        // cast image to float
        blCastImageToFloat cast;
        cast.setInput(m_inputImage);
        cast.run();
        blImage* castInput = cast.output();

        // apply convolutin
        blConvolutionFilter conv;
        conv.setInput(castInput);
        conv.setKernel(m_kernel);
        conv.setNormalizeKernel(m_normalizeKernel);
        conv.run();
        m_outputImage = conv.output();
        delete castInput;
    }
    else if(m_inputImage->imageType() == blImage::TypeFloat2D){

        m_outputImage = convolution<Float2DImage>(m_inputImage->itkFloat2DImagePointer(), m_kernel->itkFloat2DImagePointer());
    }
    else if(m_inputImage->imageType() == blImage::TypeFloat3D){

        m_outputImage = convolution<Float3DImage>(m_inputImage->itkFloat3DImagePointer(), m_kernel->itkFloat3DImagePointer());

    }
    else if(m_inputImage->imageType() == blImage::TypeIntColor2D || m_inputImage->imageType() == blImage::TypeFloatColor2D
            || m_inputImage->imageType() == blImage::TypeIntColor3D || m_inputImage->imageType() == blImage::TypeFloatColor3D){

        // Calculate gradient
        blImage* compRed = convolutionOnOneComponent(m_inputImage, 0);
        blImage* compGreen =convolutionOnOneComponent(m_inputImage, 1);
        blImage* compBlue = convolutionOnOneComponent(m_inputImage, 2);

        // compose images
        blComposeColorImage composer;
        composer.setComposant1(compRed);
        composer.setComposant2(compGreen);
        composer.setComposant3(compBlue);
        composer.run();
        m_outputImage = composer.output();
    }
    else{
        std::string message = "The image format " + m_inputImage->imageType() + " is not suported by blGradientMagnitudeFiniteDiff filter";
        throw blException(message.c_str());
    }
}

// parameters
void blConvolutionFilter::setNormalizeKernel(bool value){
    m_normalizeKernel = value;
}

blImage* blConvolutionFilter::convolutionOnOneComponent(blImage* input, unsigned int index){

    // extract component
    blExtractComponentColorImage extractor;
    extractor.setInput(input);
    extractor.setComponentIndex(index);
    extractor.run();
    blImage* comp = extractor.output();

    // Calculate gradient
    blConvolutionFilter conv;
    conv.setInput(comp);
    conv.setKernel(m_kernel);
    conv.setNormalizeKernel(m_normalizeKernel);
    conv.run();
    return conv.output();
}
