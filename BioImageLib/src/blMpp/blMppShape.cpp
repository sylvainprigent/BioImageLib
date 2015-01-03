/// \file blMppShape.cpp
/// \brief blMppShape class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppShape.h"
#include <iostream>

using namespace std;

blMppShape::blMppShape(){

    m_maxDistanceToBarycenter = 0;
    m_dataTerm = 0;
    m_insidePixels = NULL;
    m_borderPixels = NULL;
    m_normalsBorder = NULL;

    m_insidePixelsCalculated = false;
    m_borderPixelsCalculated = false;
    m_normalsCalculated = false;

    m_centerSetted = false;
}

blMppShape::~blMppShape(){
    if (m_insidePixelsCalculated)
        delete m_insidePixels;
    if (m_borderPixelsCalculated)
        delete m_borderPixels;
    if (m_normalsCalculated)
        delete m_normalsBorder;

}

std::string blMppShape::name(){
    return "blMppShape";
}

void blMppShape::setCenter(blPixel center){
    m_center = center;
    m_centerSetted = true;
}

blPixel blMppShape::getCenter(){
    return m_center;
}

void blMppShape::setDataTerm(float dataTerm){
    m_dataTerm = dataTerm;
}

float blMppShape::dataTerm(){
    return m_dataTerm;
}

void blMppShape::setInsidePixels(std::vector<blPixel> *insidePixels){
    m_insidePixelsCalculated = true;
    m_insidePixels = insidePixels;
}

std::vector<blPixel> * blMppShape::insidePixels(){
    if (!m_insidePixelsCalculated){
        computeInsidePixels();
    }
    return m_insidePixels;
}

void blMppShape::setBorderPixels(std::vector<blPixel> *borderPixels){
    m_borderPixelsCalculated = true;
    m_borderPixels = borderPixels;
}

std::vector<blPixel> * blMppShape::borderPixels(){
    return m_borderPixels;
}

void blMppShape::setNormalsBorder(std::vector<std::vector<float> > *normalsBorder){
    m_normalsCalculated = true;
    m_normalsBorder = normalsBorder;
}

std::vector<std::vector<float> > * blMppShape::normalsBorder(){
    return m_normalsBorder;
}

float blMppShape::maxDistanceToBarycenter(){
    return m_maxDistanceToBarycenter;
}

void blMppShape::setMaxDistanceToBarycenter(float value){
    m_maxDistanceToBarycenter = value;
}

std::vector<float> blMppShape::getParameters(){
    vector<float> params; params.resize(3);
    params[0] = m_center.x();
    params[1] = m_center.y();
    params[2] = m_center.z();
    return params;
}

std::vector<std::string> blMppShape::parametersDescription(){
    std::vector<std::string> params; params.resize(4);
    params[0] = "center x";
    params[1] = "center y";
    params[2] = "center z";
    return params;
}
