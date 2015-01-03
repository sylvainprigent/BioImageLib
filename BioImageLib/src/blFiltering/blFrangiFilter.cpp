/// \file blFrangiFilter.cpp
/// \brief blFrangiFilter class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blFrangiFilter.h"
#include <blCastStacks>

blFrangiFilter::blFrangiFilter() : blFilter(){
    m_alpha = 0.5;
    m_beta = 1.0;
    m_gamma = 5.0;
    m_brightObject = true;
    m_sigmaMinimum = 1.0;
    m_sigmaMaximum = 10.0;
    m_numberOfSigmaSteps = 10;
}

blFrangiFilter::~blFrangiFilter(){

}

std::string blFrangiFilter::name(){
    return "blFrangiFilter";
}

void blFrangiFilter::setAlpha( float alpha){
    m_alpha = alpha;
}

void blFrangiFilter::setBeta( float beta){
    m_beta = beta;
}

void blFrangiFilter::setGamma( float gamma){
    m_gamma = gamma;
}

void blFrangiFilter::setBrightObject(bool brightObject){
    m_brightObject = brightObject;
}

void blFrangiFilter::setSigmaMinimum( float sigmaMinimum ){
    m_sigmaMinimum = sigmaMinimum;
}

void blFrangiFilter::setSigmaMaximum( float sigmaMaximum ){
    m_sigmaMaximum = sigmaMaximum;
}

void blFrangiFilter::setNumberOfSigmaSteps( unsigned int numberOfSigmaSteps ){
    m_numberOfSigmaSteps = numberOfSigmaSteps;
}

void blFrangiFilter::run(){

    std::cout << "frangi input type = " << m_inputImage->imageType() << std::endl;

    if (m_inputImage->imageType() == blImage::TypeInt2D
            || m_inputImage->imageType() == blImage::TypeInt3D){

        // cast image to float
        blCastImageToFloat cast;
        cast.setInput(m_inputImage);
        cast.run();
        blImage* castInput = cast.output();

        // Calculate frangi
        blFrangiFilter filter;
        filter.setInput(castInput);
        filter.setAlpha( m_alpha);
        filter.setBeta( m_beta);
        filter.setGamma( m_gamma);
        filter.setBrightObject(m_brightObject);
        filter.setSigmaMinimum( m_sigmaMinimum );
        filter.setSigmaMaximum( m_sigmaMaximum );
        filter.setNumberOfSigmaSteps( m_numberOfSigmaSteps );
        filter.run();
        m_outputImage = filter.output();
        delete castInput;
    }
    else if(m_inputImage->imageType() == blImage::TypeFloat2D){

        m_outputImage = frangiFilter<Float2DImage, Hessian2DImageType, Float2DImage>(m_inputImage->itkFloat2DImagePointer());
    }
    else if(m_inputImage->imageType() == blImage::TypeFloat3D){

        m_outputImage = frangiFilter<Float3DImage, Hessian3DImageType, Float3DImage>(m_inputImage->itkFloat3DImagePointer());

    }
    else if(m_inputImage->imageType() == blImage::TypeIntColor2D || m_inputImage->imageType() == blImage::TypeFloatColor2D
            || m_inputImage->imageType() == blImage::TypeIntColor3D || m_inputImage->imageType() == blImage::TypeFloatColor3D){

        // Calculate gradient
        blImage* compRed = frangiOnOneComponent(m_inputImage, 0);
        blImage* compGreen = frangiOnOneComponent(m_inputImage, 1);
        blImage* compBlue = frangiOnOneComponent(m_inputImage, 2);

        // compose images
        blComposeColorImage composer;
        composer.setComposant1(compRed);
        composer.setComposant2(compGreen);
        composer.setComposant3(compBlue);
        composer.run();
        m_outputImage = composer.output();
    }
    else{
        std::string message = "The image format " + m_inputImage->imageType() + " is not suported by blFrangiFilter";
        throw blException(message.c_str());
    }
}

blImage* blFrangiFilter::frangiOnOneComponent(blImage* input, unsigned int index){

    // extract component
    blExtractComponentColorImage extractor;
    extractor.setInput(input);
    extractor.setComponentIndex(index);
    extractor.run();
    blImage* comp = extractor.output();

    // Calculate frangi
    blFrangiFilter filter;
    filter.setInput(comp);
    filter.setAlpha( m_alpha);
    filter.setBeta( m_beta);
    filter.setGamma( m_gamma);
    filter.setBrightObject(m_brightObject);
    filter.setSigmaMinimum( m_sigmaMinimum );
    filter.setSigmaMaximum( m_sigmaMaximum );
    filter.setNumberOfSigmaSteps( m_numberOfSigmaSteps );
    filter.run();
    return filter.output();
}
