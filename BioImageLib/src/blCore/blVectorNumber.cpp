/// \file blVectorNumber.cpp
/// \brief blVectorNumber class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blVectorNumber.h"

blVectorNumber::blVectorNumber(){

}

blVectorNumber::~blVectorNumber(){

}

std::string blVectorNumber::name(){
    return "blVectorNumber";
}

void blVectorNumber::setData(const std::vector<float> &data){
    m_data = data;
}

const std::vector<float>& blVectorNumber::data(){
    return m_data;
}
