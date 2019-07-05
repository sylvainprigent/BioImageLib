/// \file blCmdSelect.cpp
/// \brief blCmdSelect class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2018

#include "blCmdSelect.h"

blCmdSelect::blCmdSelect(){

}

int blCmdSelect::count(){
    return m_names.size();
}

void blCmdSelect::add(std::string name, std::string value){
    m_names.push_back(name);
    m_values.push_back(value);
}

std::string blCmdSelect::name(int i){
    return m_names[i];
}

std::string blCmdSelect::value(int i){
    return m_values[i];
}
