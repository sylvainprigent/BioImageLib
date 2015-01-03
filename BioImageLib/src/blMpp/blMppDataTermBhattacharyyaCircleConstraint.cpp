/// \file blMppDataTermBhattacharyyaCircleConstraint.h
/// \brief blMppDataTermBhattacharyyaCircleConstraint class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppDataTermBhattacharyyaCircleConstraint.h"

using namespace std;
#include <blMath>

blMppDataTermBhattacharyyaCircleConstraint::blMppDataTermBhattacharyyaCircleConstraint() : blMppDataTerm(){
    m_D = 100;
    m_gradientDirection = 1;
    m_r_median = 10;
    m_lambda = 1;
}

blMppDataTermBhattacharyyaCircleConstraint::blMppDataTermBhattacharyyaCircleConstraint(blImage* image)
    : blMppDataTerm(image){
    m_D = 100;
    m_gradientDirection = 1;
    m_r_median = 10;
    m_lambda = 1;
}

blMppDataTermBhattacharyyaCircleConstraint::~blMppDataTermBhattacharyyaCircleConstraint(){

}

// blProcess methods
std::string blMppDataTermBhattacharyyaCircleConstraint::name(){
    return "blMppDataTermBhattacharyyaCircleConstraint";
}


// setters parameters
void blMppDataTermBhattacharyyaCircleConstraint::setD(float D){
    m_D = D;
}

void blMppDataTermBhattacharyyaCircleConstraint::setGradientDirection(int gradientDirection){
    m_gradientDirection = gradientDirection;
    if ( gradientDirection != 0 && gradientDirection != -1 && gradientDirection != 1){
        throw( blException("blMppDataTermBhattacharyyaCircleConstraint: gradient direction must be -1, 0, or 1"));
    }
}

void blMppDataTermBhattacharyyaCircleConstraint::setMedianRadius(float radius){
    m_r_median = radius;
}

void blMppDataTermBhattacharyyaCircleConstraint::setLambda(float lambda){
    m_lambda = lambda;
}

// dataTerm methods
void blMppDataTermBhattacharyyaCircleConstraint::computeStats(blMppShape *shape, float &meanIn, float &meanBorder, float &varIn, float &varBorder, float& r1, float& r2){

    float shapeCenterX = shape->getCenter().x();
    float shapeCenterY = shape->getCenter().y();

    // get statistics
    Float2DImage::Pointer image = m_image->itkFloat2DImagePointer();
    Float2DImage::IndexType pixelIndex;

    // inside mean and var
    std::vector<blPixel> *insidePixels = shape->insidePixels();
    std::vector<blPixel>::iterator itPixel;
    meanIn = 0;
    varIn = 0;
    float val = 0;
    for (itPixel = insidePixels->begin(); (itPixel != insidePixels->end()) ; ++itPixel) {
        pixelIndex[0] = (*itPixel).x();
        pixelIndex[1] = (*itPixel).y();
        val = image->GetPixel(pixelIndex);
        meanIn += val;
        varIn += val*val;
    }
    int n = insidePixels->size();
    meanIn /= n;
    varIn=(varIn-meanIn*meanIn*n)/(n-1);


    // border mean and var
    meanBorder = 0;
    varBorder = 0;
    val = 0;
    std::vector<blPixel> *borderPixels = shape->borderPixels();
    float maxDistance = 0.0;
    float minDistance = 999999.9;
    float dist;
    for (itPixel = borderPixels->begin() ; (itPixel != borderPixels->end()) ; ++itPixel) {
        pixelIndex[0] = (*itPixel).x();
        pixelIndex[1] = (*itPixel).y();
        val = image->GetPixel(pixelIndex);
        meanBorder += val;
        varBorder += val*val;

        dist = pow(shapeCenterX - pixelIndex[0],2) + pow(shapeCenterY - pixelIndex[1],2);
        if (dist > maxDistance){
            maxDistance = dist;
        }
        if (dist < minDistance){
            minDistance = dist;
        }
    }
    n = borderPixels->size();
    meanBorder /= n;
    varBorder=(varBorder-meanBorder*meanBorder*n)/(n-1);


    // inside and external radius
    r1 = sqrt(minDistance);
    r2 = sqrt(maxDistance);
}

void blMppDataTermBhattacharyyaCircleConstraint::initialize(){
    if (m_image->imageType() != blImage::TypeFloat2D){
        throw blException("blMppDataTermBhattacharyyaCircleConstraint can process only images of type Float2DImage");
    }
}

float blMppDataTermBhattacharyyaCircleConstraint::compute(blMppShape* shape){

    // 1- Compute bhattacharyya
    float dB;
    float meanIn, meanBorder, varIn, varBorder;
    float r1, r2;
    computeStats(shape, meanIn, meanBorder, varIn, varBorder, r1, r2);
    float gap = meanIn - meanBorder;

    // Test gradient direction
    bool decisionGradient;
    if (m_gradientDirection < 0 && gap > 0){
        decisionGradient = true;
    }
    else if (m_gradientDirection > 0 && gap < 0){
        decisionGradient = true;
    }
    else{
        decisionGradient = false;
    }

    if (decisionGradient){
        return 1;
    }
    else {
        float epsilon = 0.0000001;
        if (varIn < epsilon)
        {
            varIn += epsilon;
        }
        if (varBorder < epsilon)
        {
            varBorder += epsilon;
        }
        dB =  pow(meanIn - meanBorder,2)/(4*sqrt(varIn+varBorder)) + 0.5* log((varIn+varBorder) / (2*sqrt(sqrt(varIn)*sqrt(varBorder))));
    }

    float bhattacharyya;
    if (blMath::isNan(dB)){dB = 0;}
    if (dB < m_threshold){
        bhattacharyya = (1- dB/m_threshold);
    }
    else{
        bhattacharyya = exp(-(dB - m_threshold)/m_D)-1;
    }

    // 2- Compute the constraints data term
    float min;
    if(m_r_median/r2 < r2/m_r_median){
      min=(m_r_median/r2)*(m_r_median/r2);
    }
    else{
      min = (r2/m_r_median)*(r2/m_r_median);
    }
    if(bhattacharyya<0){
      return ((r1/r2) - (1-m_lambda))*((r1/r2) - (1+m_lambda))*min;
    }
    else{
      return bhattacharyya;
    }
}

void blMppDataTermBhattacharyyaCircleConstraint::createCompatibleShapesList(){
    m_compatibleShapesList.push_back("all");
}

bool blMppDataTermBhattacharyyaCircleConstraint::useInsidePixels(){
    return true;
}

bool blMppDataTermBhattacharyyaCircleConstraint::useBorderPixels(){
    return true;
}

bool blMppDataTermBhattacharyyaCircleConstraint::useNormals(){
    return false;
}
