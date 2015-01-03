/// \file blMppInteraction.cpp
/// \brief blMppInteraction class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppInteraction.h"

blMppInteraction::blMppInteraction() : blProcess(){
    m_generatorName = "";
    m_isGenerator3D = false;
}

std::string blMppInteraction::name(){
    return "blMppInteraction";
}

void blMppInteraction::setGeneratorName(std::string name){
    m_generatorName = name;
}

void blMppInteraction::setGeneratorType(bool is3D){
    m_isGenerator3D = is3D;
}

void blMppInteraction::initialize(){

}
