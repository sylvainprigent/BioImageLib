/// \file blMedianFilter.cpp
/// \brief blMedianFilter class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blMedianFilter.h"
#include <blCastStacks>

blMedianFilter::blMedianFilter() : blFilter(){
    m_radius = 2;
}

blMedianFilter::~blMedianFilter(){

}

std::string blMedianFilter::name(){
    return "blMedianFilter";
}

void blMedianFilter::setRadius(float radius){
    m_radius = radius;
}

void blMedianFilter::run(){
    if (m_inputImage->imageType() == blImage::TypeInt2D){

        m_outputImage = medianFilter<Int2DImage>(m_inputImage->itkInt2DImagePointer());
    }
    else if(m_inputImage->imageType() == blImage::TypeFloat2D){

        m_outputImage = medianFilter<Float2DImage>(m_inputImage->itkFloat2DImagePointer());
    }
    else if (m_inputImage->imageType() == blImage::TypeInt3D){

        m_outputImage = medianFilter<Int3DImage>(m_inputImage->itkInt3DImagePointer());
    }
    else if(m_inputImage->imageType() == blImage::TypeFloat3D){

        m_outputImage = medianFilter<Float3DImage>(m_inputImage->itkFloat3DImagePointer());

    }
    else if(m_inputImage->imageType() == blImage::TypeIntColor2D || m_inputImage->imageType() == blImage::TypeFloatColor2D
            || m_inputImage->imageType() == blImage::TypeIntColor3D || m_inputImage->imageType() == blImage::TypeFloatColor3D){

        // Calculate gradient
        blImage* compRed = medianOnOneComponent(m_inputImage, 0);
        blImage* compGreen = medianOnOneComponent(m_inputImage, 1);
        blImage* compBlue = medianOnOneComponent(m_inputImage, 2);

        // compose images
        blComposeColorImage composer;
        composer.setComposant1(compRed);
        composer.setComposant2(compGreen);
        composer.setComposant3(compBlue);
        composer.run();
        m_outputImage = composer.output();
    }
    else{
        std::string message = "The image format " + m_inputImage->imageType() + " is not suported by blMedianFilter";
        throw blException(message.c_str());
    }
}

blImage* blMedianFilter::medianOnOneComponent(blImage* input, unsigned int index){

    // extract component
    blExtractComponentColorImage extractor;
    extractor.setInput(input);
    extractor.setComponentIndex(index);
    extractor.run();
    blImage* comp = extractor.output();

    // Calculate gradient
    blMedianFilter median;
    median.setInput(comp);
    median.setRadius(m_radius);
    median.run();
    return median.output();
}
