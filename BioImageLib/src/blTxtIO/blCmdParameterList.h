/// \file blCmdParameterList.h
/// \brief blCmdParameterList class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2018

#pragma once

#include "blCmdParameter.h"
#include <vector>
#include "blTxtIOExport.h"

/// \class blCmdParameterList
/// \brief class that define a container for unix shell single parameter
class BLTXTIO_EXPORT blCmdParameterList{

public:
    blCmdParameterList();
    ~blCmdParameterList();

public:
    void add(blCmdParameter* input);
    void setProgramName(std::string name);
    void setDescription(std::string description);
    void setWrapperName(std::string name);
    void setToolboxName(std::string name);
    void setHelp(std::string help);
    void setWrapperPath(std::string path);
    std::string getProgramName();
    std::string getDescription();
    std::string getWrapperName();
    std::string getToolboxName();
    std::string getHelp();
    std::string getWrapperPath();
    bool exists(std::string parameterName);

public:
    blCmdParameter* get(std::string name);
    std::vector<blCmdParameter*> getInputImages();
    std::vector<blCmdParameter*> getOutputImages();


    blCmdParameter* at(int i);
    int size();
    std::string man();

protected:
    std::vector< blCmdParameter* > m_parameters;
    std::string m_programName;
    std::string m_wrapperName;
    std::string m_wrapperPath;
    std::string m_description;
    std::string m_toolboxName;
    std::string m_help;
};
