/// \file blPixel.cpp
/// \brief blPixel class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blPixel.h"

blPixel::blPixel(){
    m_x = 0;
    m_y = 0;
    m_z = 0;
}

blPixel::blPixel(int x, int y){
    m_x = x;
    m_y = y;
    m_z = 0;
}

blPixel::blPixel(int x, int y, int z){
    m_x = x;
    m_y = y;
    m_z = z;
}

int blPixel::x(){
    return m_x;
}

int blPixel::y(){
    return m_y;
}

int blPixel::z(){
    return m_z;
}

void blPixel::setX(int x){
    m_x = x;
}

void blPixel::setY(int y){
    m_y = y;
}

void blPixel::setZ(int z){
    m_z = z;
}
