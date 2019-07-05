/// \file blCmdParser2.cpp
/// \brief blCmdParser2 class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2018

#include "blCmdParser2.h"
#include "blCore/blException.h"
#include <iostream>


blCmdParser2::blCmdParser2(int argc, char *argv[])
{
    m_argc = argc;
    m_argv = argv;
    m_args.setProgramName(argv[0]);
}

void blCmdParser2::addParameter(std::string name, std::string description, std::string defaultValue, std::string type,  bool isAdvanced)
{
    blCmdParameter *arginput = new blCmdParameter(name, description);
    arginput->setIO("input");
    arginput->setType(type);
    arginput->setDefault(defaultValue);
    arginput->setIsAdvanced(isAdvanced);
    m_args.add(arginput);
}

void blCmdParser2::addInputImage(std::string name, std::string description)
{
    blCmdParameter *arginput = new blCmdParameter(name, description);
    arginput->setIO("input");
    arginput->setType("image");
    m_args.add(arginput);
}

void blCmdParser2::addOutputImage(std::string name, std::string description)
{
    blCmdParameter *arginput = new blCmdParameter(name, description);
    arginput->setIO("output");
    arginput->setType("image");
    m_args.add(arginput);
}

void blCmdParser2::setMan(std::string man)
{
    m_man = man;
}

// parse
void blCmdParser2::parse(int minArgc)
{

    if (m_argc == 2 && ( std::string(m_argv[1]) == "-h" || std::string(m_argv[1]) == "--help"))
    {

        std::string man = "OVERVIEW: " +  m_man + "\n\n";
        man += "USAGE: "+std::string(m_argv[0])+" [parameters] \n\n";
        man += "PARAMETERS:\n";
        man += m_args.man();
        throw blException(man.c_str());
    }

    if (m_argc <= minArgc)
    {
        throw blException(("ERROR: "+std::string(m_argv[0])+"  needs at least " + std::to_string(minArgc) + " arguments. \n Call it with -h or --help to read the man page").c_str());
    }
    else
    {
        for (int i = 1; i < m_argc; ++i)
        {
            std::string argName = std::string(m_argv[i]);
            if (i + 1 < m_argc && !argName.compare(0, 1, "-"))
            {
                i++;
                this->setArgValue(argName, std::string(m_argv[i]));
            }
            else
            {
                throw blException((argName + " option require one argument").c_str());
            }
        }
    }
}

void blCmdParser2::print()
{
    for (int i = 0; i < m_args.size(); i++)
    {
        std::cout << m_args.at(i)->getName() << " " << m_args.at(i)->getValue() << std::endl;
    }
}

void blCmdParser2::setArgValue(std::string name, std::string value)
{
    bool found = false;
    for (int j = 0; j < m_args.size(); ++j)
    {
        if (m_args.at(j)->getName() == name)
        {
            m_args.at(j)->setValue(value);
            found = true;
            break;
        }
    }
    if (!found)
    {
        throw blException((name + " option is unknown").c_str());
    }
}

bool blCmdParser2::getParameterBool(std::string name){
    std::string value = this->getParameterString(name);
    if ( value == "true" || value == "True" || value == "TRUE" ){
        return true;
    }
    return false;
}

int blCmdParser2::getParameterInt(std::string name)
{
    return std::stoi(this->getParameterString(name));
}
float blCmdParser2::getParameterFloat(std::string name)
{
    return std::stof(this->getParameterString(name));
}
double blCmdParser2::getParameterDouble(std::string name)
{
    return std::stod(this->getParameterString(name));
}
std::string blCmdParser2::getParameterString(std::string name)
{
    blCmdParameter* param = m_args.get(name);
    if (param){
        return param->getValue();
    }
    return "";
}
const char* blCmdParser2::getParameterChar(std::string name)
{
    return this->getParameterString(name).c_str();
}

std::string blCmdParser2::getImageUrl(std::string name){
    return this->getParameterString(name);
}

blProcessParameters* blCmdParser2::toProcessParameters(){

    blProcessParameters* processParameters = new blProcessParameters();
    for (int i = 0 ; i < m_args.size() ; i++){
        processParameters->setValueOfKey(m_args.at(i)->getName(), m_args.at(i)->getValue());
    }
    return processParameters;
}
