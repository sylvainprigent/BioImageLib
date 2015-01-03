/// \file blMppDataTerm.cpp
/// \brief blMppDataTerm class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppDataTerm.h"

using namespace std;

// //////////////////////////////////////////////////////////// //
//						blMppDataTerm
// /////////////////////////////////////////////////////////// //
blMppDataTerm::blMppDataTerm() : blProcess() {
    initAttributs();
}

blMppDataTerm::blMppDataTerm(blImage* image) : blProcess(){
    initAttributs();
    this->setImage(image);
}

void blMppDataTerm::initAttributs(){
    m_threshold = 5.0;
    m_borderWidth = 1;
    m_image = NULL;
    m_generatorName = "";
}

blMppDataTerm::~blMppDataTerm(){

}

// blProcess methods
string blMppDataTerm::name(){
    return "blMppDataTerm";
}

// inputs
void blMppDataTerm::setImage(blImage* image){
    m_image = image;
}

std::vector<std::string> blMppDataTerm::compatibleShapes(){
    return m_compatibleShapesList;
}

bool blMppDataTerm::isCompatible(string shapeGenerator){

    for (unsigned int i = 0 ; i < m_compatibleShapesList.size() ; ++i){
        if (m_compatibleShapesList[i] == shapeGenerator || m_compatibleShapesList[i] == "all"){
            return true;
        }
    }
    return false;
}

// parameters
void blMppDataTerm::setThreshold(float threshold){
    m_threshold = threshold;
}

void blMppDataTerm::setBorderWidth(int width){
    m_borderWidth = width;
    if (m_borderWidth < 1){
        throw( blException("blMppDataTerm: border width must be al least one"));
    }
}

int blMppDataTerm::borderWidth(){
    return m_borderWidth;
}

blImage* blMppDataTerm::image(){
    return m_image;
}

// common functions
void blMppDataTerm::insideAndBorderValues(blMppShape* shape, std::vector<float> &insideValues, std::vector<float> &borderValues){

    /// \todo extend it to 3D images
    Float2DImage::Pointer image = m_image->itkFloat2DImagePointer();
    Float2DImage::IndexType pixelIndex;

    // inside
    std::vector<blPixel> *insidePixels = shape->insidePixels();
    insideValues.resize(insidePixels->size());

    std::vector<blPixel>::iterator itPixel;
    std::vector<float>::iterator itValue;
    for (itPixel = insidePixels->begin(), itValue = insideValues.begin(); (itPixel != insidePixels->end()) && (itValue != insideValues.end()); ++itPixel, ++itValue) {
        pixelIndex[0] = (*itPixel).x();
        pixelIndex[1] = (*itPixel).y();
        *itValue = image->GetPixel(pixelIndex);
    }

    // border
    std::vector<blPixel> *borderPixels = shape->borderPixels();
    borderValues.resize(borderPixels->size());
    for (itPixel = borderPixels->begin(), itValue = borderValues.begin(); (itPixel != borderPixels->end()) && (itValue != borderValues.end()); ++itPixel, ++itValue) {
        pixelIndex[0] = (*itPixel).x();
        pixelIndex[1] = (*itPixel).y();
        *itValue = image->GetPixel(pixelIndex);
    }
}

void blMppDataTerm::setGeneratorName(std::string name){
    m_generatorName = name;
}
