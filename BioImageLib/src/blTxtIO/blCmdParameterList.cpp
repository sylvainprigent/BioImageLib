/// \file blCmdParameterList.cpp
/// \brief blCmdParameterList class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2018

#include "blCmdParameterList.h"

blCmdParameterList::blCmdParameterList()
{
}

blCmdParameterList::~blCmdParameterList()
{
    for (int i = 0; i < m_parameters.size(); i++)
    {
        //delete m_parameters[i];
    }
}

void blCmdParameterList::add(blCmdParameter *paramater)
{
    m_parameters.push_back(paramater);
}

void blCmdParameterList::setProgramName(std::string name)
{
    m_programName = name;
}

void blCmdParameterList::setDescription(std::string description){
    m_description = description;
}

void blCmdParameterList::setWrapperName(std::string name){
    m_wrapperName = name;
}

void blCmdParameterList::setWrapperPath(std::string path){
    m_wrapperPath = path;
}

void blCmdParameterList::setToolboxName(std::string name){
    m_toolboxName = name;
}

std::string blCmdParameterList::getDescription(){
    return m_description;
}

std::string blCmdParameterList::getProgramName(){
    return m_programName;
}

std::string blCmdParameterList::getWrapperName(){
    return m_wrapperName;
}

std::string blCmdParameterList::getWrapperPath(){
    return m_wrapperPath;
}

std::string blCmdParameterList::getToolboxName(){
    return m_toolboxName;
}

void blCmdParameterList::setHelp(std::string help){
    m_help = help;
}

std::string blCmdParameterList::getHelp(){
    return m_help;
}

bool blCmdParameterList::exists(std::string parameterName){

    for (int i = 0; i < m_parameters.size(); i++)
    {
        if ( m_parameters[i]->getName() == parameterName){
            return true;
        }
    }
    return false;
}

std::string blCmdParameterList::man()
{
    std::string txt = this->getProgramName() + ": " + this->getDescription() + "\n";
    for (int i = 0; i < m_parameters.size(); i++)
    {
        txt += "\t" + m_parameters[i]->getName() + "\t" + m_parameters[i]->getDefault() + "\t" + m_parameters[i]->getDescription() + "\n";
    }
    return txt;
}

int blCmdParameterList::size()
{
    return m_parameters.size();
}

blCmdParameter *blCmdParameterList::at(int i)
{
    return m_parameters[i];
}

blCmdParameter *blCmdParameterList::get(std::string name)
{
    for (int i = 0; i < m_parameters.size(); i++)
    {
        if (m_parameters[i]->getName() == name)
        {
            return m_parameters[i];
        }
    }
    return NULL;
}

std::vector<blCmdParameter*> blCmdParameterList::getInputImages(){
    std::vector<blCmdParameter*> inputImages;
    for(int i = 0 ; i < m_parameters.size() ; i++){
        if ( m_parameters[i]->getType() == "image" && m_parameters[i]->getIO() == "input"){
            inputImages.push_back(m_parameters[i]);        
        } 
    }
    return inputImages;
}

std::vector<blCmdParameter*> blCmdParameterList::getOutputImages(){
    std::vector<blCmdParameter*> outputImages;
    for(int i = 0 ; i < m_parameters.size() ; i++){
        if ( m_parameters[i]->getType() == "image" && m_parameters[i]->getIO() == "output"){
            outputImages.push_back(m_parameters[i]);        
        } 
    }
    return outputImages;
}
