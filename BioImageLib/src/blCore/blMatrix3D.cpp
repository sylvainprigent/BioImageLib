/// \file blMatrix3D.cpp
/// \brief blMatrix3D class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blMatrix3D.h"
#include <iostream>


std::string blMatrix3D::name(){
    return "blMatrix3D";
}

blMatrix3D::blMatrix3D() : blData(){
    reset();
}

blMatrix3D::blMatrix3D(unsigned int dimX, unsigned int dimY, unsigned int dimZ)  : blData(){
    reset();
    allocate(dimX, dimY, dimZ);
}

blMatrix3D::blMatrix3D(float * data, unsigned int dimX, unsigned int dimY, unsigned int dimZ)  : blData(){
    m_dimX = dimX;
    m_dimY = dimY;
    m_dimZ = dimZ;
    m_data = data;
    m_isAlreadyAllocated = true;
}

blMatrix3D::blMatrix3D(const blMatrix3D &matrix)  : blData(){
    reset();
    (*this) = matrix ;
}


blMatrix3D::~blMatrix3D(){
    if (m_isAlreadyAllocated){
        free();
    }
}

// getters
unsigned int blMatrix3D::dimX() const{
    return m_dimX;
}
unsigned int blMatrix3D::dimY() const{
    return m_dimY;
}
unsigned int blMatrix3D::dimZ() const{
    return m_dimZ;
}

//accessors to values
float& blMatrix3D::operator()(unsigned int i, unsigned int j, unsigned int k){
    return m_data[i * m_dimY + j + m_dimY*m_dimZ*k];
}

float blMatrix3D::operator()(unsigned int i, unsigned int j, unsigned int k) const
{
    return m_data[i * m_dimY + j + m_dimY*m_dimZ*k];
}

float& blMatrix3D::operator [](unsigned int i){
    return m_data[i];
}

float blMatrix3D::operator [](unsigned int i) const{
    return m_data[i];
}


// operations
blMatrix3D blMatrix3D::operator +(const blMatrix3D &matrix) const{
    return blMatrix3D( *this ) += matrix;
}
blMatrix3D blMatrix3D::operator +(const float &value) const{
    return blMatrix3D( *this ) += value;
}
blMatrix3D blMatrix3D::operator -(const blMatrix3D &matrix) const{
    return blMatrix3D( *this ) -= matrix;
}
blMatrix3D blMatrix3D::operator -(const float &value) const{
    return blMatrix3D( *this ) -= value;
}
blMatrix3D blMatrix3D::operator *(const float &value) const{
    return blMatrix3D( *this ) *= value;

}
blMatrix3D blMatrix3D::operator /(const float &value) const{
    return blMatrix3D( *this ) /= value;
}

// self operations
blMatrix3D& blMatrix3D::operator =(const blMatrix3D &matrix){

    if (m_isAlreadyAllocated){
        free();
    }
    allocate(matrix.m_dimX, matrix.m_dimY, matrix.m_dimZ);

    for (unsigned int i = 0 ; i < m_dimX*m_dimY*m_dimZ ; ++i){
        m_data[i] = matrix.m_data[i];
    }

    return *this;

}

blMatrix3D& blMatrix3D::operator +=(const blMatrix3D &matrix){

    for (unsigned int i = 0 ; i < m_dimX*m_dimY*m_dimZ ; ++i){
        m_data[i] += matrix.m_data[i];
    }
    return *this;
}

blMatrix3D& blMatrix3D::operator +=(const float &value){

    for (unsigned int i = 0 ; i < m_dimX*m_dimY*m_dimZ ; ++i){
        m_data[i] += value;
    }
    return *this;
}

blMatrix3D& blMatrix3D::operator -=(const blMatrix3D &matrix){

    for (unsigned int i = 0 ; i < m_dimX*m_dimY*m_dimZ ; ++i){
        m_data[i] -= matrix.m_data[i];
    }
    return *this;
}

blMatrix3D& blMatrix3D::operator -=(const float &value){

    for (unsigned int i = 0 ; i < m_dimX*m_dimY*m_dimZ ; ++i){
        m_data[i] -= value;
    }
    return *this;
}

blMatrix3D& blMatrix3D::operator *=(const float &value){

    for (unsigned int i = 0 ; i < m_dimX*m_dimY*m_dimZ ; ++i){
        m_data[i] *= value;
    }
    return *this;
}

blMatrix3D& blMatrix3D::operator /=(const float &value){

    for (unsigned int i = 0 ; i < m_dimX*m_dimY*m_dimZ ; ++i){
        m_data[i] /= value;
    }
    return *this;
}

// comparisons
int blMatrix3D::operator ==(const blMatrix3D &matrix) const{

    if ((m_dimX != matrix.m_dimX) || (m_dimY != matrix.m_dimY) || (m_dimZ != matrix.m_dimZ)){
        return 0;
    }

    for (unsigned int i = 0 ; i < m_dimX*m_dimY*m_dimZ ; ++i){
        if ( m_data[i] != matrix.m_data[i]){
            return 0;
        }
    }
    return 1;
}

int blMatrix3D::operator !=(const blMatrix3D &matrix) const{

    return !( (*this) == matrix );
}

// internal functions
void blMatrix3D::reset(){
    m_dimX = 0;
    m_dimY = 0;
    m_dimZ = 0;
    m_data = NULL;
    m_isAlreadyAllocated = false;
}

void blMatrix3D::allocate(unsigned int dimX, unsigned int dimY, unsigned int dimZ){
    if (m_isAlreadyAllocated){
        free();
    }
    m_dimX = dimX;
    m_dimY = dimY;
    m_dimZ = dimZ;
    m_data = new float[m_dimX*m_dimY*m_dimZ];
    m_isAlreadyAllocated = true;
}

void blMatrix3D::free(){
    delete m_data;
    reset();
}
