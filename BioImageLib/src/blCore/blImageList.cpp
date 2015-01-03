/// \file blImageList.cpp
/// \brief blImageList class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blImageList.h"

blImageList::blImageList(){

}

blImageList::~blImageList(){

}

std::string blImageList::name(){
    return "blImageList";
}

void blImageList::setData(const std::vector<blImage*> &data){
    m_data = data;
}

std::vector<blImage*>& blImageList::data(){
    return m_data;
}

unsigned int blImageList::length(){
    return m_data.size();
}

blImage* blImageList::imageAt(unsigned int i){
    return m_data[i];
}
