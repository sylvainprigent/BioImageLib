#include "blMppDataTermBhattacharyya.h"
#include <blMath>

using namespace std;

blMppDataTermBhattacharyya::blMppDataTermBhattacharyya() : blMppDataTerm(){
    m_D = 100;
    m_is2DImage = true;
    m_gradientDirection = 1;
}

blMppDataTermBhattacharyya::blMppDataTermBhattacharyya(blImage* image)
    : blMppDataTerm(image){
    m_D = 100;
    m_is2DImage = true;
    m_gradientDirection = 1;
}

blMppDataTermBhattacharyya::~blMppDataTermBhattacharyya(){

}

// blProcess methods
std::string blMppDataTermBhattacharyya::name(){
    return "blMppDataTermBhattacharyya";
}


// setters parameters
void blMppDataTermBhattacharyya::setD(float D){
    m_D = D;
}

void blMppDataTermBhattacharyya::setGradientDirection(int gradientDirection){
    m_gradientDirection = gradientDirection;
    if ( gradientDirection != 0 && gradientDirection != -1 && gradientDirection != 1){
        throw( blException("blMppDataTermBhattacharyya: gradient direction must be -1, 0, or 1"));
    }
}

// dataTerm methods
void blMppDataTermBhattacharyya::computeStats2D(blMppShape *shape, float &meanIn, float &meanBorder, float &varIn, float &varBorder){

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
    for (itPixel = borderPixels->begin() ; (itPixel != borderPixels->end()) ; ++itPixel) {
        pixelIndex[0] = (*itPixel).x();
        pixelIndex[1] = (*itPixel).y();
        val = image->GetPixel(pixelIndex);
        meanBorder += val;
        varBorder += val*val;
    }
    n = borderPixels->size();
    meanBorder /= n;
    varBorder=(varBorder-meanBorder*meanBorder*n)/(n-1);
}

void blMppDataTermBhattacharyya::computeStats3D(blMppShape *shape, float &meanIn, float &meanBorder, float &varIn, float &varBorder){

    // get statistics
    Float3DImage::Pointer image = m_image->itkFloat3DImagePointer();
    Float3DImage::IndexType pixelIndex;

    // inside mean and var
    std::vector<blPixel> *insidePixels = shape->insidePixels();
    std::vector<blPixel>::iterator itPixel;
    meanIn = 0;
    varIn = 0;
    float val = 0;
    for (itPixel = insidePixels->begin(); (itPixel != insidePixels->end()) ; ++itPixel) {
        pixelIndex[0] = (*itPixel).x();
        pixelIndex[1] = (*itPixel).y();
        pixelIndex[2] = (*itPixel).z();

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
    for (itPixel = borderPixels->begin() ; (itPixel != borderPixels->end()) ; ++itPixel) {
        pixelIndex[0] = (*itPixel).x();
        pixelIndex[1] = (*itPixel).y();
        pixelIndex[2] = (*itPixel).z();
        val = image->GetPixel(pixelIndex);
        meanBorder += val;
        varBorder += val*val;
    }
    n = borderPixels->size();
    meanBorder /= n;
    varBorder=(varBorder-meanBorder*meanBorder*n)/(n-1);
}

void blMppDataTermBhattacharyya::initialize(){
    if (m_image->imageType() == blImage::TypeFloat2D){
        m_is2DImage = true;
    }
    else if (m_image->imageType() == blImage::TypeFloat3D){
        m_is2DImage = false;
    }
    else{
        throw blException("blMppDataTermBhattacharyya can process only images of type Float2DImage");
    }
}

float blMppDataTermBhattacharyya::compute(blMppShape* shape){

    //std::cout << "blMppDataTermBhattacharyya::compute begin" << std::endl;
    float dB;
    float meanIn, meanBorder, varIn, varBorder;
    //std::cout << "blMppDataTermBhattacharyya compute stats" << std::endl;
    if (m_is2DImage){
        computeStats2D(shape, meanIn, meanBorder, varIn, varBorder);
    }
    else{
        computeStats3D(shape, meanIn, meanBorder, varIn, varBorder);
    }
    float gap = meanIn - meanBorder;

    // Test gradient direction
    //std::cout << "blMppDataTermBhattacharyya test gradient" << std::endl;
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
        //std::cout << "vars = " << varIn << ", " << varBorder << std::endl;
        if (varIn < epsilon || blMath::isNan(varIn))
        {
            varIn = epsilon;
            return 1;
        }
        if (varBorder < epsilon || blMath::isNan(varBorder))
        {
            varBorder = epsilon;
            return 1;
        }
        //std::cout << "vars = " << varIn << ", " << varBorder << std::endl;
        dB =  pow(meanIn - meanBorder,2)/(4*sqrt(varIn+varBorder)) + 0.5* log((varIn+varBorder) / (2*sqrt(sqrt(varIn)*sqrt(varBorder))));
    }

    //cout << "dB = " << dB << endl;
    //std::cout << "blMppDataTermBhattacharyya::compute end" << std::endl;
    if (blMath::isNan(dB)){return 1;}
    if (dB < m_threshold){
        return (1- dB/m_threshold);
    }
    else{
        return exp(-(dB - m_threshold)/m_D)-1;
    }
}

void blMppDataTermBhattacharyya::createCompatibleShapesList(){
    m_compatibleShapesList.push_back("all");
}

bool blMppDataTermBhattacharyya::useInsidePixels(){
    return true;
}

bool blMppDataTermBhattacharyya::useBorderPixels(){
    return true;
}

bool blMppDataTermBhattacharyya::useNormals(){
    return false;
}
