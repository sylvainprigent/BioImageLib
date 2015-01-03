/// \file blMatrix.cpp
/// \brief blMatrix class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blMatrix.h"
#include <iostream>


std::string blMatrix::name(){
    return "blMatrix";
}



blMatrix::blMatrix() : blData(){
    reset();
}

blMatrix::blMatrix(unsigned int rowNum, unsigned int columnNum)  : blData(){
    reset();
    allocate(rowNum, columnNum);
}

blMatrix::blMatrix(float * data, unsigned int rowNum, unsigned int columnNum)  : blData(){
    m_rowNum = rowNum;
    m_columnNum = columnNum;
    m_data = data;
    m_isAlreadyAllocated = true;
}

blMatrix::blMatrix(const blMatrix &matrix)  : blData(){
    reset();
    (*this) = matrix ;
}


blMatrix::~blMatrix(){
    if (m_isAlreadyAllocated){
        free();
    }
}

// getters
unsigned int blMatrix::numberOfRows() const{
    return m_rowNum;
}
unsigned int blMatrix::numberOfColumns() const{
    return m_columnNum;
}

//accessors to values
float& blMatrix::operator()(unsigned int i, unsigned int j){
    return m_data[i * m_columnNum + j];
}

float blMatrix::operator()(unsigned int i, unsigned int j) const
{
    return m_data[i * m_columnNum + j];
}

float& blMatrix::operator [](unsigned int i){
    return m_data[i];
}

float blMatrix::operator [](unsigned int i) const{
    return m_data[i];
}


// operations
blMatrix blMatrix::operator +(const blMatrix &matrix) const{
    return blMatrix( *this ) += matrix;
}
blMatrix blMatrix::operator +(const float &value) const{
    return blMatrix( *this ) += value;
}
blMatrix blMatrix::operator -(const blMatrix &matrix) const{
    return blMatrix( *this ) -= matrix;
}
blMatrix blMatrix::operator -(const float &value) const{
    return blMatrix( *this ) -= value;
}
blMatrix blMatrix::operator *(const blMatrix &matrix) const{

    unsigned int rowProd = matrix.numberOfColumns();
    blMatrix prod(m_rowNum, rowProd);

    float inter;
    for (unsigned int i = 0 ; i < m_rowNum ; ++i){
        for (unsigned int j = 0 ; j < rowProd ; ++j){
            inter = 0.0;
            for (unsigned int k = 0 ; k < m_columnNum ; ++k){
                inter += m_data[i*m_columnNum + k]*matrix.m_data[k*rowProd + j];
            }
            prod.m_data[i*rowProd + j] = inter;
        }
    }
    return prod;
}
blMatrix blMatrix::operator *(const float &value) const{
    return blMatrix( *this ) *= value;

}
blMatrix blMatrix::operator /(const float &value) const{
    return blMatrix( *this ) /= value;
}

// self operations
blMatrix& blMatrix::operator =(const blMatrix &matrix){

    if (m_isAlreadyAllocated){
        free();
    }
    allocate(matrix.m_rowNum, matrix.m_columnNum);

    for (unsigned int i = 0 ; i < m_rowNum*m_columnNum ; ++i){
        m_data[i] = matrix.m_data[i];
    }

    return *this;
}

blMatrix& blMatrix::operator +=(const blMatrix &matrix){

    for (unsigned int i = 0 ; i < m_rowNum*m_columnNum ; ++i){
        m_data[i] += matrix.m_data[i];
    }
    return *this;
}

blMatrix& blMatrix::operator +=(const float &value){

    for (unsigned int i = 0 ; i < m_rowNum*m_columnNum ; ++i){
        m_data[i] += value;
    }
    return *this;
}

blMatrix& blMatrix::operator -=(const blMatrix &matrix){

    for (unsigned int i = 0 ; i < m_rowNum*m_columnNum ; ++i){
        m_data[i] -= matrix.m_data[i];
    }
    return *this;
}

blMatrix& blMatrix::operator -=(const float &value){

    for (unsigned int i = 0 ; i < m_rowNum*m_columnNum ; ++i){
        m_data[i] -= value;
    }
    return *this;
}

blMatrix& blMatrix::operator *=(const float &value){

    for (unsigned int i = 0 ; i < m_rowNum*m_columnNum ; ++i){
        m_data[i] *= value;
    }
    return *this;
}

blMatrix& blMatrix::operator /=(const float &value){

    for (unsigned int i = 0 ; i < m_rowNum*m_columnNum ; ++i){
        m_data[i] /= value;
    }
    return *this;
}

// comparisons
int blMatrix::operator ==(const blMatrix &matrix) const{

    if ((m_rowNum != matrix.m_rowNum) || (m_columnNum != matrix.m_columnNum)){
        return 0;
    }

    for (unsigned int i = 0 ; i < m_rowNum*m_columnNum ; ++i){
        if ( m_data[i] != matrix.m_data[i]){
            return 0;
        }
    }
    return 1;
}

int blMatrix::operator !=(const blMatrix &matrix) const{

    return !( (*this) == matrix );
}

// internal functions
void blMatrix::reset(){
    m_rowNum = 0;
    m_columnNum = 0;
    m_data = NULL;
    m_isAlreadyAllocated = false;
}

void blMatrix::allocate(unsigned int rowNum, unsigned int rowColumn){
    if (m_isAlreadyAllocated){
        free();
    }
    m_rowNum = rowNum;
    m_columnNum = rowColumn;
    m_data = new float[rowNum*rowColumn];
    m_isAlreadyAllocated = true;
}

void blMatrix::free(){
    delete[] m_data;
    reset();
}
