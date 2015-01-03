/// \file blProjectionImage.cpp
/// \brief blZProjection class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blProjectionImage.h"
#include <blMath>
#include <blCastImageToFloat.h>
#include "blComposeColorImage.h"
#include "blExtractComponentColorImage.h"

using namespace std;

// images types
const std::string blZProjection::Maximum = "Maximum";
const std::string blZProjection::Minimum = "Minimum";
const std::string blZProjection::Mean = "Mean";
const std::string blZProjection::StandardDeviation = "StandardDeviation";
const std::string blZProjection::Median = "Median";



blZProjection::blZProjection(){
    m_inputImage = NULL;
    m_outputImage = NULL;
}

blZProjection::~blZProjection(){

}

std::string blZProjection::name(){
    return "blZProjection";
}

void blZProjection::setInput(blImage* image){
    m_inputImage = image;
}

void blZProjection::setProjectionType(std::string projectionType){
    m_projectionType = projectionType;
}

void blZProjection::run(){

    if (m_inputImage->imageType() == blImage::TypeInt2D
            || m_inputImage->imageType() == blImage::TypeFloat2D
            || m_inputImage->imageType() == blImage::TypeIntColor2D
            || m_inputImage->imageType() == blImage::TypeFloatColor2D){

        throw blException("blZProjection need a 3D image as an input");
    }

    if (m_inputImage->imageType() == blImage::TypeInt3D){

        // cast image to float
        blCastImageToFloat cast;
        cast.setInput(m_inputImage);
        cast.run();
        blImage* castInput = cast.output();

        // apply convolutin
        blZProjection process;
        process.setInput(castInput);
        process.setProjectionType(m_projectionType);
        process.run();
        m_outputImage = process.output();
        delete castInput;
    }
    else if(m_inputImage->imageType() == blImage::TypeFloat3D){

        if (m_projectionType == blZProjection::Maximum){
            m_outputImage = new blImage(MaximumZ(m_inputImage->itkFloat3DImagePointer()));
        }
        else if (m_projectionType == blZProjection::Minimum) {
            m_outputImage = new blImage(MinimumZ(m_inputImage->itkFloat3DImagePointer()));
        }
        else if (m_projectionType == blZProjection::Mean) {
            m_outputImage = new blImage(MeanZ(m_inputImage->itkFloat3DImagePointer()));
        }
        else if (m_projectionType == blZProjection::StandardDeviation) {
            m_outputImage = new blImage(StdZ(m_inputImage->itkFloat3DImagePointer()));
        }
        else if (m_projectionType == blZProjection::Median) {
            m_outputImage = new blImage(MedianZ(m_inputImage->itkFloat3DImagePointer()));
        }
    }
    else if(m_inputImage->imageType() == blImage::TypeIntColor3D
            || m_inputImage->imageType() == blImage::TypeFloatColor3D){

        // Calculate gradient
        blImage* compRed = ProjectOneComponent(m_inputImage, 0);
        blImage* compGreen =ProjectOneComponent(m_inputImage, 1);
        blImage* compBlue = ProjectOneComponent(m_inputImage, 2);

        // compose images
        blComposeColorImage composer;
        composer.setComposant1(compRed);
        composer.setComposant2(compGreen);
        composer.setComposant3(compBlue);
        composer.run();
        m_outputImage = composer.output();
    }
    else{
        std::string message = "The image format " + m_inputImage->imageType() + " is not suported by blZProjection";
        throw blException(message.c_str());
    }
}

blImage* blZProjection::output(){
    return m_outputImage;
}

Float2DImage::Pointer blZProjection::MaximumZ(Float3DImage::Pointer image){

    int nl = image->GetLargestPossibleRegion().GetSize()[0];
    int nc = image->GetLargestPossibleRegion().GetSize()[1];
    int nz = image->GetLargestPossibleRegion().GetSize()[2];

    Float2DImage::Pointer output_image = blImageCreate::createNewFloat2DImage(nl,nc);

    Float3DImage::IndexType pixelIndex;
    Float2DImage::IndexType outputIndex;
    vector<float> values; values.resize(nz);
    float maxi;
    for (int x = 0 ; x < nl ; ++x){
        for (int y = 0 ; y < nc ; ++y){
            for (int z = 0 ; z < nz ; ++z){
                pixelIndex[0] = x; pixelIndex[1] = y; pixelIndex[2] = z;
                values[z] = image->GetPixel(pixelIndex);
            }
            maxi = blMathVector::max(values);
            outputIndex[0] = x;
            outputIndex[1] = y;
            output_image->SetPixel(outputIndex, maxi);
        }
    }
    return output_image;
}

Float2DImage::Pointer blZProjection::MinimumZ(Float3DImage::Pointer image){

    int nl = image->GetLargestPossibleRegion().GetSize()[0];
    int nc = image->GetLargestPossibleRegion().GetSize()[1];
    int nz = image->GetLargestPossibleRegion().GetSize()[2];

    Float2DImage::Pointer output_image = blImageCreate::createNewFloat2DImage(nl,nc);

    Float3DImage::IndexType pixelIndex;
    Float2DImage::IndexType outputIndex;
    vector<float> values; values.resize(nz);
    float maxi;
    for (int x = 0 ; x < nl ; ++x){
        for (int y = 0 ; y < nc ; ++y){
            for (int z = 0 ; z < nz ; ++z){
                pixelIndex[0] = x; pixelIndex[1] = y; pixelIndex[2] = z;
                values[z] = image->GetPixel(pixelIndex);
            }
            maxi = blMathVector::min(values);
            outputIndex[0] = x;
            outputIndex[1] = y;
            output_image->SetPixel(outputIndex, maxi);
        }
    }
    return output_image;
}

Float2DImage::Pointer blZProjection::MeanZ(Float3DImage::Pointer image){

    int nl = image->GetLargestPossibleRegion().GetSize()[0];
    int nc = image->GetLargestPossibleRegion().GetSize()[1];
    int nz = image->GetLargestPossibleRegion().GetSize()[2];

    Float2DImage::Pointer output_image = blImageCreate::createNewFloat2DImage(nl,nc);

    Float3DImage::IndexType pixelIndex;
    Float2DImage::IndexType outputIndex;
    vector<float> values; values.resize(nz);
    float maxi;
    for (int x = 0 ; x < nl ; ++x){
        for (int y = 0 ; y < nc ; ++y){
            for (int z = 0 ; z < nz ; ++z){
                pixelIndex[0] = x; pixelIndex[1] = y; pixelIndex[2] = z;
                values[z] = image->GetPixel(pixelIndex);
            }
            maxi = blMathVector::mean(values);
            outputIndex[0] = x;
            outputIndex[1] = y;
            output_image->SetPixel(outputIndex, maxi);
        }
    }
    return output_image;
}

Float2DImage::Pointer blZProjection::StdZ(Float3DImage::Pointer image){

    int nl = image->GetLargestPossibleRegion().GetSize()[0];
    int nc = image->GetLargestPossibleRegion().GetSize()[1];
    int nz = image->GetLargestPossibleRegion().GetSize()[2];

    Float2DImage::Pointer output_image = blImageCreate::createNewFloat2DImage(nl,nc);

    Float3DImage::IndexType pixelIndex;
    Float2DImage::IndexType outputIndex;
    vector<float> values; values.resize(nz);
    float maxi;
    for (int x = 0 ; x < nl ; ++x){
        for (int y = 0 ; y < nc ; ++y){
            for (int z = 0 ; z < nz ; ++z){
                pixelIndex[0] = x; pixelIndex[1] = y; pixelIndex[2] = z;
                values[z] = image->GetPixel(pixelIndex);
            }
            maxi = blMathVector::var(values);
            maxi = sqrt(maxi);
            outputIndex[0] = x;
            outputIndex[1] = y;
            output_image->SetPixel(outputIndex, maxi);
        }
    }
    return output_image;
}

Float2DImage::Pointer blZProjection::MedianZ(Float3DImage::Pointer image){

    int nl = image->GetLargestPossibleRegion().GetSize()[0];
    int nc = image->GetLargestPossibleRegion().GetSize()[1];
    int nz = image->GetLargestPossibleRegion().GetSize()[2];

    Float2DImage::Pointer output_image = blImageCreate::createNewFloat2DImage(nl,nc);

    Float3DImage::IndexType pixelIndex;
    Float2DImage::IndexType outputIndex;
    vector<float> values; values.resize(nz);
    float maxi;
    for (int x = 0 ; x < nl ; ++x){
        for (int y = 0 ; y < nc ; ++y){
            for (int z = 0 ; z < nz ; ++z){
                pixelIndex[0] = x; pixelIndex[1] = y; pixelIndex[2] = z;
                values[z] = image->GetPixel(pixelIndex);
            }
            maxi = blMathVector::median(values);
            maxi = sqrt(maxi);
            outputIndex[0] = x;
            outputIndex[1] = y;
            output_image->SetPixel(outputIndex, maxi);
        }
    }
    return output_image;
}

blImage* blZProjection::ProjectOneComponent(blImage* input, unsigned int index){

    // extract component
    blExtractComponentColorImage extractor;
    extractor.setInput(input);
    extractor.setComponentIndex(index);
    extractor.run();
    blImage* comp = extractor.output();

    // Calculate gradient
    blZProjection process;
    process.setInput(comp);
    process.setProjectionType(m_projectionType);
    process.run();
    return process.output();
}
