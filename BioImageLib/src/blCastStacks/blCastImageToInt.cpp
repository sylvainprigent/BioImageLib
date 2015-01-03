/// \file blCastImageToInt.cpp
/// \brief blCastImageToInt class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blCastImageToInt.h"
#include "blComposeColorImage.h"
#include "blExtractComponentColorImage.h"

blCastImageToInt::blCastImageToInt() : blCastImage(){

}

blCastImageToInt::~blCastImageToInt(){

}

std::string blCastImageToInt::name(){
    return "blCastImageToInt";
}

void blCastImageToInt::run(){

    if ( m_inputImage->imageType() == blImage::TypeInt2D
         || m_inputImage->imageType() == blImage::TypeInt3D
         || m_inputImage->imageType() == blImage::TypeIntColor2D
         || m_inputImage->imageType() == blImage::TypeIntColor3D){

        std::string message = "blCastImageToInt: the input image format " + m_inputImage->imageType() + " is already an integer";
        throw blException(message.c_str());
    }

    if (m_inputImage->imageType() == blImage::TypeFloat2D ){

        typedef itk::CastImageFilter< Float2DImage, Int2DImage > CastFilterType;
        CastFilterType::Pointer castFilter = CastFilterType::New();
        castFilter->SetInput(m_inputImage->itkFloat2DImagePointer());
        castFilter->Update();
        m_outputImage = new blImage(castFilter->GetOutput());

    }
    else if (m_inputImage->imageType() == blImage::TypeFloat3D){

        typedef itk::CastImageFilter< Float3DImage, Int3DImage > CastFilterType;
        CastFilterType::Pointer castFilter = CastFilterType::New();
        castFilter->SetInput(m_inputImage->itkFloat3DImagePointer());
        castFilter->Update();
        m_outputImage = new blImage(castFilter->GetOutput());

    }
    else if(m_inputImage->imageType() == blImage::TypeFloatColor2D || m_inputImage->imageType() == blImage::TypeFloatColor3D){

        // Calculate gradient
        blImage* compRed = castOnOneComponent(m_inputImage, 0);
        blImage* compGreen =castOnOneComponent(m_inputImage, 1);
        blImage* compBlue = castOnOneComponent(m_inputImage, 2);

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

blImage* blCastImageToInt::castOnOneComponent(blImage* input, unsigned int index){
    // extract component
    blExtractComponentColorImage extractor;
    extractor.setInput(input);
    extractor.setComponentIndex(index);
    extractor.run();
    blImage* comp = extractor.output();

    // Calculate gradient
    blCastImageToInt cast;
    cast.setInput(comp);
    cast.run();
    return cast.output();
}
