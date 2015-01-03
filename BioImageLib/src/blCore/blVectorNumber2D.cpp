/// \file blVectorNumber2D.cpp
/// \brief blVectorNumber2D class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blVectorNumber2D.h"

blVectorNumber2D::blVectorNumber2D(){

}

blVectorNumber2D::~blVectorNumber2D(){

}

std::string blVectorNumber2D::name(){
    return "blVectorNumber2D";
}

void blVectorNumber2D::setData(const std::vector< std::vector<float> > &data){
    m_data = data;
}

const std::vector< std::vector<float> >& blVectorNumber2D::data(){
    return m_data;
}
