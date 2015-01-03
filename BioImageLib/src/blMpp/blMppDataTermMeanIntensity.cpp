/// \file blMppDataTermMeanIntensity.cpp
/// \brief blMppDataTermMeanIntensity class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppDataTermMeanIntensity.h"

using namespace std;
#include <blMath>

blMppDataTermMeanIntensity::blMppDataTermMeanIntensity() : blMppDataTerm(){
    m_D = 100;
}

blMppDataTermMeanIntensity::blMppDataTermMeanIntensity(blImage* image)
    : blMppDataTerm(image){
    m_D = 100;
}

blMppDataTermMeanIntensity::~blMppDataTermMeanIntensity(){

}

// blProcess methods
std::string blMppDataTermMeanIntensity::name(){
    return "blMppDataTermMeanIntensity";
}


// setters parameters
void blMppDataTermMeanIntensity::setD(float D){
    m_D = D;
}


// dataTerm methods
void blMppDataTermMeanIntensity::computeStats(blMppShape *shape, float &meanIn){

    // get statistics
    Float2DImage::Pointer image = m_image->itkFloat2DImagePointer();
    Float2DImage::IndexType pixelIndex;

    // inside mean and var
    std::vector<blPixel> *insidePixels = shape->insidePixels();
    std::vector<blPixel>::iterator itPixel;
    meanIn = 0;
    float val;
    for (itPixel = insidePixels->begin(); (itPixel != insidePixels->end()) ; ++itPixel) {
        pixelIndex[0] = (*itPixel).x();
        pixelIndex[1] = (*itPixel).y();
        val = image->GetPixel(pixelIndex);
        meanIn += val;
    }
    int n = insidePixels->size();
    meanIn /= n;
}

void blMppDataTermMeanIntensity::initialize(){

}

float blMppDataTermMeanIntensity::compute(blMppShape* shape){

    float meanIn;
    computeStats(shape, meanIn);

    if (meanIn < m_threshold){
        return (1- meanIn/m_threshold);
    }
    else{
        return exp(-(meanIn - m_threshold)/m_D)-1;
    }
}

void blMppDataTermMeanIntensity::createCompatibleShapesList(){
    m_compatibleShapesList.push_back("all");
}

bool blMppDataTermMeanIntensity::useInsidePixels(){
    return true;
}

bool blMppDataTermMeanIntensity::useBorderPixels(){
    return true;
}

bool blMppDataTermMeanIntensity::useNormals(){
    return false;
}
