/// \file blMeanFilter.cpp
/// \brief blMeanFilter class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blMeanFilter.h"
#include <blCastStacks>

blMeanFilter::blMeanFilter() : blFilter(){

}

blMeanFilter::~blMeanFilter(){

}

std::string blMeanFilter::name(){
    return "blMeanFilter";
}

void blMeanFilter::setRadius(int radius){
    m_radius = radius;
}

void blMeanFilter::run(){
    if (m_inputImage->imageType() == blImage::TypeInt2D){

        m_outputImage = meanFilter<Int2DImage>(m_inputImage->itkInt2DImagePointer());
    }
    else if(m_inputImage->imageType() == blImage::TypeFloat2D){

        m_outputImage = meanFilter<Float2DImage>(m_inputImage->itkFloat2DImagePointer());
    }
    else if (m_inputImage->imageType() == blImage::TypeInt3D){

        m_outputImage = meanFilter<Int3DImage>(m_inputImage->itkInt3DImagePointer());
    }
    else if(m_inputImage->imageType() == blImage::TypeFloat3D){

        m_outputImage = meanFilter<Float3DImage>(m_inputImage->itkFloat3DImagePointer());

    }
    else if(m_inputImage->imageType() == blImage::TypeIntColor2D || m_inputImage->imageType() == blImage::TypeFloatColor2D
            || m_inputImage->imageType() == blImage::TypeIntColor3D || m_inputImage->imageType() == blImage::TypeFloatColor3D){

        // Calculate gradient
        blImage* compRed = meanOnOneComponent(m_inputImage, 0);
        blImage* compGreen = meanOnOneComponent(m_inputImage, 1);
        blImage* compBlue = meanOnOneComponent(m_inputImage, 2);

        // compose images
        blComposeColorImage composer;
        composer.setComposant1(compRed);
        composer.setComposant2(compGreen);
        composer.setComposant3(compBlue);
        composer.run();
        m_outputImage = composer.output();
    }
    else{
        std::string message = "The image format " + m_inputImage->imageType() + " is not suported by blMeanFilter";
        throw blException(message.c_str());
    }
}

blImage* blMeanFilter::meanOnOneComponent(blImage* input, unsigned int index){

    // extract component
    blExtractComponentColorImage extractor;
    extractor.setInput(input);
    extractor.setComponentIndex(index);
    extractor.run();
    blImage* comp = extractor.output();

    // Calculate gradient
    blMeanFilter mean;
    mean.setInput(comp);
    mean.run();
    return mean.output();
}
