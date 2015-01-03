/// \file blVectorString.cpp
/// \brief blVectorString class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blVectorString.h"

blVectorString::blVectorString(){

}

blVectorString::~blVectorString(){

}

std::string blVectorString::name(){
    return "blVectorString";
}

void blVectorString::setData(const std::vector<std::string> &data){
    m_data = data;
}

std::vector<std::string>& blVectorString::data(){
    return m_data;
}
