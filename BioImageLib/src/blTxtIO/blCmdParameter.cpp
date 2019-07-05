/// \file blCmdParameter.cpp
/// \brief blCmdParameter class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2018

#include "blCmdParameter.h"

const std::string blCmdParameter::TypeImage = "image";
const std::string blCmdParameter::TypeNumber = "number";
const std::string blCmdParameter::TypeString = "string";
const std::string blCmdParameter::TypeSelect = "select";
const std::string blCmdParameter::TypeBoolean = "boolean";
const std::string blCmdParameter::TypeHidden = "hidden";

const std::string blCmdParameter::IOInput = "input";
const std::string blCmdParameter::IOOutput = "output";

blCmdParameter::blCmdParameter(std::string name, std::string description)
{
    m_name = name;
    m_description = description;
    m_isAdvanced = false;
}

std::string blCmdParameter::getName()
{
    return m_name;
}

std::string blCmdParameter::getValueSuffix(){
    return m_valueSuffix;
}

std::string blCmdParameter::getDescription()
{
    return m_description;
}

std::string blCmdParameter::getValue()
{
    return m_value;
}

std::string blCmdParameter::getType()
{
    return m_type;
}

std::string blCmdParameter::getIO()
{
    return m_io;
}

std::string blCmdParameter::getDefault()
{
    return m_defaultValue;
}

bool blCmdParameter::getIsAdvanced()
{
    return m_isAdvanced;
}

blCmdSelect blCmdParameter::getSelectInfo(){
    return m_selectInfo;
}

void blCmdParameter::setDescription(std::string description)
{
    m_description = description;
}

void blCmdParameter::setType(std::string type)
{
    m_type = type;
}

void blCmdParameter::setIO(std::string io)
{
    m_io = io;
}

void blCmdParameter::setValue(std::string value)
{
    m_value = value;
}

void blCmdParameter::setDefault(std::string value)
{
    m_defaultValue = value;
}

void blCmdParameter::setIsAdvanced(bool value)
{
    m_isAdvanced = value;
}

void blCmdParameter::setValueSuffix(std::string suffix)
{
    m_valueSuffix = suffix;
}

void blCmdParameter::setSelectInfo(blCmdSelect select){
    m_selectInfo = select;
}
