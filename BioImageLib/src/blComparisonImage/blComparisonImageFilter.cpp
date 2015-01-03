/// \file blComparisonImageFilter.cpp
/// \brief blComparisonImageFilter class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blComparisonImageFilter.h"
#include <blCastStacks>

blComparisonImageFilter::blComparisonImageFilter() : blProcess(){
    m_referenceImage = NULL;
    m_testImage = NULL;
    m_numberOfPixelsWithDifferences = 999999;

    //parameters
    m_intensityTolerance = 0.01;
    m_radiusTolerance = 0;
}

blComparisonImageFilter::~blComparisonImageFilter(){

}

std::string blComparisonImageFilter::name(){
    return "blComparisonImageFilter";
}

// inputs
void blComparisonImageFilter::setReferenceImage(blImage* image){
    m_referenceImage = image;
}

void blComparisonImageFilter::setTestImage(blImage* image){
    m_testImage = image;
}

// run
void blComparisonImageFilter::run(){
    if (m_referenceImage->imageType() == blImage::TypeInt2D && m_testImage->imageType() == blImage::TypeInt2D){

        compareImages<Int2DImage>(m_referenceImage->itkInt2DImagePointer(), m_testImage->itkInt2DImagePointer());
    }
    else if(m_referenceImage->imageType() == blImage::TypeFloat2D && m_testImage->imageType() == blImage::TypeFloat2D){

        compareImages<Float2DImage>(m_referenceImage->itkFloat2DImagePointer(), m_testImage->itkFloat2DImagePointer());
    }
    else if (m_referenceImage->imageType() == blImage::TypeInt3D && m_testImage->imageType() == blImage::TypeInt3D){

        compareImages<Int3DImage>(m_referenceImage->itkInt3DImagePointer(), m_testImage->itkInt3DImagePointer());
    }
    else if(m_referenceImage->imageType() == blImage::TypeFloat3D && m_testImage->imageType() == blImage::TypeFloat3D){

        compareImages<Float3DImage>(m_referenceImage->itkFloat3DImagePointer(), m_testImage->itkFloat3DImagePointer());
    }
    else if( (m_referenceImage->imageType() == blImage::TypeIntColor2D && m_testImage->imageType() == blImage::TypeIntColor2D)
            || (m_referenceImage->imageType() == blImage::TypeFloatColor2D && m_testImage->imageType() == blImage::TypeFloatColor2D)
            || (m_referenceImage->imageType() == blImage::TypeIntColor3D && m_testImage->imageType() == blImage::TypeIntColor3D)
            || (m_referenceImage->imageType() == blImage::TypeFloatColor3D && m_testImage->imageType() == blImage::TypeFloatColor3D)){

        // Calculate gradient
        unsigned short numberOfPixelsWithDifferencesRed = compareOneComponent(m_referenceImage, m_testImage, 0);
        unsigned short numberOfPixelsWithDifferencesGreen =compareOneComponent(m_referenceImage, m_testImage, 1);
        unsigned short numberOfPixelsWithDifferencesBlue = compareOneComponent(m_referenceImage, m_testImage, 2);

        // compose images
        m_numberOfPixelsWithDifferences = numberOfPixelsWithDifferencesRed
                + numberOfPixelsWithDifferencesGreen
                + numberOfPixelsWithDifferencesBlue;
    }
    else{
        std::string message = "blComparisonImageFilter: The image format " + m_referenceImage->imageType() + " is not suported or the "
                " image test image is not of the same type than the reference image";
        throw blException(message.c_str());
    }
}

// outputs
unsigned long blComparisonImageFilter::numberOfPixelsWithDifferences(){
    return m_numberOfPixelsWithDifferences;
}

// parameters
void blComparisonImageFilter::setDifferenceThreshold( float intensityTolerance ){
    m_intensityTolerance = intensityTolerance;
}

void blComparisonImageFilter::setToleranceRadius( int radiusTolerance ){
    m_radiusTolerance = radiusTolerance;
}

unsigned short blComparisonImageFilter::compareOneComponent(blImage* refImage, blImage* testImage, unsigned int index){

    // extract components
    blExtractComponentColorImage extractorRef;
    extractorRef.setInput(refImage);
    extractorRef.setComponentIndex(index);
    extractorRef.run();
    blImage* refImageComp = extractorRef.output();

    blExtractComponentColorImage extractorTest;
    extractorTest.setInput(testImage);
    extractorTest.setComponentIndex(index);
    extractorTest.run();
    blImage* testImageComp = extractorTest.output();

    // Calculate gradient
    blComparisonImageFilter diff;
    diff.setReferenceImage(refImageComp);
    diff.setTestImage(testImageComp);
    diff.setDifferenceThreshold( m_intensityTolerance );
    diff.setToleranceRadius( m_radiusTolerance );
    diff.run();
    return diff.numberOfPixelsWithDifferences();
}
