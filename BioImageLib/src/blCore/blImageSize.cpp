/// \file blImageSize.cpp
/// \brief blImageSize class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blImageSize.h"

blImageSize::blImageSize(){

}

blImageSize::blImageSize(int xSize, int ySize, int colorSize){
    setSize(xSize, ySize, colorSize);
}

blImageSize::blImageSize(int xSize, int ySize, int zSize, int colorSize){
    setSize(xSize, ySize, zSize, colorSize);
}

void blImageSize::setSize(int xSize, int ySize, int zSize, int colorSize){
    m_xSize = xSize;
    m_ySize = ySize;
    m_zSize = zSize;
    m_colorSize = colorSize;
}

void blImageSize::setSize(int xSize, int ySize, int colorSize){
    m_xSize = xSize;
    m_ySize = ySize;
    m_zSize = 1;
    m_colorSize = colorSize;
}

int blImageSize::xSize(){
    return m_xSize;
}

int blImageSize::ySize(){
    return m_ySize;
}

int blImageSize::zSize(){
    return m_zSize;
}

int blImageSize::colorSize(){
    return m_colorSize;
}

void blImageSize::setXSize(int value){
    m_xSize = value;
}

void blImageSize::setYSize(int value){
    m_ySize = value;
}

void blImageSize::setZSize(int value){
    m_zSize = value;
}

void blImageSize::setColorSize(int value){
    m_colorSize = value;
}
