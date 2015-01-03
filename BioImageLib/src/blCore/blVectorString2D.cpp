/// \file blVectorString2D.cpp
/// \brief blVectorString2D class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blVectorString2D.h"

blVectorString2D::blVectorString2D(){

}

blVectorString2D::~blVectorString2D(){

}

std::string blVectorString2D::name(){
    return "blVectorString2D";
}

void blVectorString2D::setData(const std::vector< std::vector<std::string> > &data){
    m_data = data;
}

std::vector< std::vector<std::string> >& blVectorString2D::data(){
    return m_data;
}
